#ifndef ROEPHYSICSMODEL_H_
#define ROEPHYSICSMODEL_H_

#include "RoeCommon.h"

#ifdef ROE_COMPILE_PHYSICS

#include <ode/ode.h>
#include "RoeModel.h"
#include "RoeVector3.h"

namespace Roe {
namespace Physics {
	
	struct S_PhysicsModelIntern { //triangle mesh for ode
		Vector3 *vertices; //has to be deleted
		Vector3 *normals;  //has to be deleted
		byte4   *indices;  //has to be deleted
		byte4    numVertices;
		byte4    numNormals;
		byte4    numIndices;
	};
	
	class PhysicsModel {
	public:
		PhysicsModel(dWorldID world, dSpaceID space);
		PhysicsModel(dWorldID world, dSpaceID space,
		             std::string sPhysicsModel, bool bCompleteThePath,
		             const Model& gfxModel,
		             float mass = 1.0f,
		             Vector3 pos = Vector3::ZERO, Vector3 vel = Vector3::ZERO); //negative mass results in static geometry
		PhysicsModel(const PhysicsModel& rhs);
		~PhysicsModel();
		PhysicsModel operator= (const PhysicsModel& rhs);
		
		void load(dWorldID world, dSpaceID space,
		          std::string sPhysicsModel, bool bCompleteThePath,
		          const Model& gfxModel,
		          bool bIsStatic = false); //loads this object from static class data
		
		void render(); //renders model
		void setMass   (float mass);    //sets directly the mass
		void setDensity(float density); //sets the density of this model
		void setPos (Vector3 v); // Translate the body somewhere
		void setVel (Vector3 v); // Set a speed
		Vector3 getPos() const;      //returns the current position
		Vector3 getVel() const;      //returns the current velocity
		Matrix  getRotation() const; //returns the rotation matrix
		
		
		// for the static variables
		static void init(std::string sStart, std::string sEnd)  {s_sPathStart = sStart; s_sPathEnd = sEnd;}
		static void setPathStart(std::string s)                 {s_sPathStart = s;}
		static void setPathEnd  (std::string s)                 {s_sPathEnd   = s;}
		static std::string getPathStart()                       {return s_sPathStart;}
		static std::string getPathEnd  ()                       {return s_sPathEnd;}
		static void quit();
		
		//loads/deletes OFF models physically and NOT graphically (calls NOT Model::loadOFF...)
		static void loadOFF(std::string sPath, bool bCompleteThePath = true, ECWCCW cwccw = CW_CCW,
		                    const Matrix& mTransform = Matrix::IDENTITY);
		static void deleteOFF(std::string sPath, bool bCompleteThePath = true); //deletes one model
		
	private:
		static std::map <std::string, S_PhysicsModelIntern>           s_mpPhysicsModel;   //map with all structs
		static std::map <std::string, S_PhysicsModelIntern>::iterator s_mpi;       //map iterator
		static std::pair<std::string, S_PhysicsModelIntern>           s_pair;      //pair object for the map
		static std::string s_sPathStart, s_sPathEnd;                        //standard path
		
		Model       m_gfxModel;       //graphic model
		std::string m_sPhysicsPath;   //complete path to physics model
		dBodyID m_Body;
		dMass   m_Mass;
		dGeomID m_Geom;
		dTriMeshDataID m_TriMesh; //trimesh id
		bool m_bIsStatic; //part of static enviroment?
		bool m_bIsTriMesh;
	};

} // namespace Physics
} // namespace Roe

#endif //ROE_COMPILE_PHYSICS
#endif /* ROEPHYSICSMODEL_H_ */
