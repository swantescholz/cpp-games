#include "RoePhysicsModel.h"

#ifdef ROE_COMPILE_PHYSICS

#include "RoeTextfile.h"
#include "RoeException.h"
#include "RoeUtil.h"
#include "RoeCommon.h"

namespace Roe {
namespace Physics {
		
		std::map <std::string, S_PhysicsModelIntern>           PhysicsModel::s_mpPhysicsModel;
		std::map <std::string, S_PhysicsModelIntern>::iterator PhysicsModel::s_mpi;
		std::pair<std::string, S_PhysicsModelIntern>           PhysicsModel::s_pair;
		std::string PhysicsModel::s_sPathStart, PhysicsModel::s_sPathEnd = ".off";
		
		PhysicsModel::PhysicsModel(dWorldID world, dSpaceID space)
		{
			m_Body = dBodyCreate(world); // Create an object in the world
			setMass(1.0f);
			m_bIsStatic = m_bIsTriMesh = false;
		}
		PhysicsModel::PhysicsModel(dWorldID world, dSpaceID space,
		                           std::string sPhysicsModel, bool bCompleteThePath,
		                           const Model& gfxModel,
		                           float mass, Vector3 pos, Vector3 vel)
		{
			m_Body = dBodyCreate(world); // Create an object in the world
			m_bIsStatic = m_bIsTriMesh = false;
			load(world, space, sPhysicsModel, bCompleteThePath, gfxModel, mass<=0.0f);
			
			if (mass > 0.0f) {
				setMass(mass);
			}
			setPos(pos);
			setVel(vel);
		}
		
		PhysicsModel::~PhysicsModel() {
			if (m_sPhysicsPath.length() > 0) { //geometry already loaded
				dGeomDestroy(m_Geom);
				if (m_bIsTriMesh) dGeomTriMeshDataDestroy(m_TriMesh); //destroy trimesh data
			}
			dBodyDestroy(m_Body);
		}
		
		void PhysicsModel::load(dWorldID world, dSpaceID space,
		                        std::string sPhysicsModel, bool bCompleteThePath,
		                        const Model& gfxModel,
		                        bool bIsStatic) {
			//saving gfx model
			m_gfxModel = gfxModel;
			const Vector3 boxSize = m_gfxModel.getBoxMax() - m_gfxModel.getBoxMin();
			
			if (bCompleteThePath)
				sPhysicsModel = s_sPathStart + sPhysicsModel + s_sPathEnd;
			
			//checking for ODE std geometries
			if (sPhysicsModel == "SPHERE") { //std sphere
				m_Geom = dCreateSphere(space, m_gfxModel.getRadius());
			}
			else if (sPhysicsModel == "CAPSULE") { //std capsule
				m_Geom = dCreateCapsule (space, (boxSize.x+boxSize.z)/4.0f, boxSize.y);
			}
			else if (sPhysicsModel == "CYLINDER") { //std cylinder
				m_Geom = dCreateCylinder(space, boxSize.y, (boxSize.x+boxSize.z)/4.0f);
			}
			else if (Util::equalOR(sPhysicsModel,std::string("BOX"),std::string("CUBOID"),std::string("CUBE"))) { //std box
				m_Geom = dCreateBox(space, boxSize.x, boxSize.y, boxSize.z);
			}
			else { //trimesh
				s_mpi = s_mpPhysicsModel.find(sPhysicsModel);
				if (s_mpi == s_mpPhysicsModel.end()) {
					throw Exception("OFF model requested to be loaded does not exist: " + sPhysicsModel, "load", "RoePhysicsModel.cpp", ROE_LINE);
				}
				//create trimesh data for geometry
				m_bIsTriMesh = true;
				m_TriMesh = dGeomTriMeshDataCreate();
				dGeomTriMeshDataBuildSingle1(m_TriMesh, (dReal*)(s_mpPhysicsModel[sPhysicsModel].vertices), 12, s_mpPhysicsModel[sPhysicsModel].numVertices,
											(const dTriIndex*)(s_mpPhysicsModel[sPhysicsModel].indices) ,     s_mpPhysicsModel[sPhysicsModel].numIndices, 12,
													   (dReal*)s_mpPhysicsModel[sPhysicsModel].normals);
				m_Geom = dCreateTriMesh(space, m_TriMesh, NULL, NULL, NULL);
			}
			
			m_bIsStatic = bIsStatic;
			if (m_bIsStatic) { //connect with static enviroment:
				dGeomSetBody(m_Geom, 0);
			} else {
				dGeomSetBody(m_Geom, m_Body);
			}
			
			m_sPhysicsPath = sPhysicsModel;
		}
		
		void PhysicsModel::render() {
			const float *fOri = dGeomGetRotation(m_Geom);
			const float *fPos = dGeomGetPosition(m_Geom);
			Matrix mOrientation(fOri[0],fOri[4],fOri[8],0, fOri[1],fOri[5],fOri[9],0, fOri[2],fOri[6],fOri[10],0, 0,0,0,1); //correct now
			Matrix mTranslation(Matrix::translation(Vector3(fPos[0],fPos[1],fPos[2])));
			m_gfxModel.setTransformation(mOrientation*mTranslation);
			m_gfxModel.render();
		}
		void PhysicsModel::setMass(float mass) {
			dMassSetSphereTotal(&m_Mass, mass, m_gfxModel.getRadius());
			//dMassAdjust(&m_Mass, mass);
			dBodySetMass(m_Body, &m_Mass); // Apply the mass to the body
		}
		void PhysicsModel::setDensity(float density) {
			dMassSetSphere(&m_Mass, density, m_gfxModel.getRadius()); // Create a mass with the distribution of a sphere
			dBodySetMass(m_Body, &m_Mass); // Apply the mass to the body
		}
		void PhysicsModel::setPos (Vector3 v) { // Translate the body somewhere
			dGeomSetPosition (m_Geom, v.x, v.y, v.z);
			if (!dBodyIsEnabled(m_Body) && !m_bIsStatic)
				dBodyEnable(m_Body);
		}
		void PhysicsModel::setVel (Vector3 v) { // Set a speed
			dBodySetLinearVel(m_Body, v.x, v.y, v.z);
			if (!dBodyIsEnabled(m_Body))
				dBodyEnable(m_Body);
		}
		Vector3 PhysicsModel::getPos() const {const float *fPos = dGeomGetPosition(m_Geom); return Vector3(fPos[0],fPos[1],fPos[2]);}
		Vector3 PhysicsModel::getVel() const {const float *fVel = dGeomGetPosition(m_Geom); return Vector3(fVel[0],fVel[1],fVel[2]);}
		Matrix  PhysicsModel::getRotation() const {
			const float *fOri = dGeomGetRotation(m_Geom);
			return Matrix(fOri[0],fOri[4],fOri[8],0, fOri[1],fOri[5],fOri[9],0, fOri[2],fOri[6],fOri[10],0, 0,0,0,1); //correct now
		}
		
		void PhysicsModel::quit() {
			for(s_mpi=s_mpPhysicsModel.begin(); s_mpi!=s_mpPhysicsModel.end(); ++s_mpi) {
				delete[] s_mpi->second.vertices;
				delete[] s_mpi->second.normals;
				delete[] s_mpi->second.indices;
			}
			s_mpPhysicsModel.clear();
		}
		
		void PhysicsModel::loadOFF(std::string sPath, bool bCompleteThePath, ECWCCW cwccw,
		                           const Matrix& mTransform ) {
			
			if (bCompleteThePath)
				sPath = s_sPathStart + sPath + s_sPathEnd;
			
			s_mpi = s_mpPhysicsModel.find(sPath);
			if (s_mpi != s_mpPhysicsModel.end()) {
				throw Exception("OFF model requested to be loaded does already exist: " + sPath, "loadOFF", "RoePhysicsModel.cpp", ROE_LINE);
			}
			
			//parsing file, loading vertices and indices and normals statically:
			Textfile tf;
			tf.readFileAt(sPath);
			const int numLines = tf.getNumLines();
			std::string sLine;
			std::string *asLines = new std::string[numLines];
			tf.getFullTextAsArray(asLines);
			tf.clear();
			sLine = asLines[1];
			const int numVertices = Util::parseInt(Util::getFirstNumberOfStringAndErase(sLine));
			const int numFaces    = Util::parseInt(Util::getFirstNumberOfStringAndErase(sLine));
			Vector3 *aVertices = new Vector3[numVertices];
			int numTriangles = 0;
			int *numEdges  = new int[numFaces];
			int **polygons = new int*[numFaces];
			float x,y,z;
			for (int i = 0; i < numVertices; i++) { //get vertex data
				sLine = asLines[i+2];
				x = Util::parseFloat(Util::getFirstNumberOfStringAndErase(sLine));
				y = Util::parseFloat(Util::getFirstNumberOfStringAndErase(sLine));
				z = Util::parseFloat(Util::getFirstNumberOfStringAndErase(sLine));
				aVertices[i] = Matrix::transformCoords(Vector3(x,y,z), mTransform);
			}
			for (int i = 0; i < numFaces; i++) { //get vertex data
				sLine = asLines[i+2+numVertices];
				numEdges[i] = Util::parseInt(Util::getFirstNumberOfStringAndErase(sLine));
				if (numEdges[i] > 2)
					numTriangles += numEdges[i]-2;
				polygons[i] = new int[numEdges[i]];
				for (int j = 0; j < numEdges[i]; j++) {
					polygons[i][j] = Util::parseInt(Util::getFirstNumberOfStringAndErase(sLine));
				}
			}
			Vector3 *aNormals  = new Vector3[numTriangles];
			byte4 *indexData   = new byte4[numTriangles*3];
			int index = 0;
			for(int i = 0; i < numFaces; i++) { //create triangle index data
				for (int j = 0; j < numEdges[i]-2; j++) {
					indexData[index+j*3+0] = polygons[i][0];
					if (cwccw == CW_CCW) { //standard
						indexData[index+j*3+1] = polygons[i][j+1];
						indexData[index+j*3+2] = polygons[i][j+2];
					} else { //CW, therefore inverted
						indexData[index+j*3+1] = polygons[i][j+2];
						indexData[index+j*3+2] = polygons[i][j+1];
					}
				}
				index += (numEdges[i]-2)*3;
			}
			for (int i = 0; i < numTriangles; i++) { //create triangle normal data
				aNormals[i] = Util::calculateNormalOfTriangle(aVertices[indexData[i*3+0]],
				                                              aVertices[indexData[i*3+1]],
				                                              aVertices[indexData[i*3+2]]);
			}
			
			
			//save calculated data in static map
			S_PhysicsModelIntern tmp;
			tmp.vertices = aVertices;
			tmp.normals  = aNormals;
			tmp.indices  = indexData;
			tmp.numVertices = numVertices;
			tmp.numNormals  = numTriangles;
			tmp.numIndices  = numTriangles*3;
			s_pair = std::make_pair(sPath, tmp);
			s_mpPhysicsModel.insert(s_pair);
			
			//cleaning up
			for (int i = 0; i < numFaces; i++) {
				delete[] (polygons[i]);
			}
			delete[] polygons;
			delete[] numEdges;
			delete[] asLines;
			
			/*for (int i = 0; i< numTriangles; i++) {
				std::cout << indexData[i*3+0] << " ";
				std::cout << indexData[i*3+1] << " ";
				std::cout << indexData[i*3+2] << std::endl;
			}
			for (int i = 0; i< numVertices; i++) {
				std::cout << aVertices[i].toString() << std::endl;
			}*/
		}
		void PhysicsModel::deleteOFF(std::string sPath, bool bCompleteThePath) {
			Model::deleteOFF(sPath, bCompleteThePath); //delete graphically too
			if (bCompleteThePath)
				sPath = s_sPathStart + sPath + s_sPathEnd;
			
			s_mpi = s_mpPhysicsModel.find(sPath);
			if (s_mpi == s_mpPhysicsModel.end()) {
				throw Exception("OFF model to be deleted does not exist", "deleteOFF", "RoePhysicsModel.cpp", ROE_LINE);
			} else {
				delete[] s_mpi->second.vertices;
				delete[] s_mpi->second.normals;
				delete[] s_mpi->second.indices;
				s_mpPhysicsModel.erase(s_mpi);
			}
		}
		
} // namespace Physics
} // namespace Roe

#endif //ROE_COMPILE_PHYSICS
