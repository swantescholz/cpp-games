#include "RoeModel.h"
#include "RoeException.h"
#include "RoeCommon.h"
#include "RoeUtil.h"
#include "RoeTexture.h"
#include <iostream>
#include <fstream>

namespace Roe {
	
	std::map <std::string, S_ModelIntern> Model::s_mpModel;
	std::map <std::string, S_ModelIntern>::iterator Model::s_mpi;
	std::pair<std::string, S_ModelIntern> Model::s_pair;
	std::string Model::s_sPathStart, Model::s_sPathEnd = ".off";
	
	Model::Model() {
		m_mMatrix = Matrix::IDENTITY;
		m_vScale  = Vector3(1,1,1);
		m_mScale  = Matrix::IDENTITY;
	}
	Model::Model(const Model& rhs) {
		(*this) = rhs;
	}
	Model& Model::operator= (const Model& rhs) {
		m_sPath     = rhs.m_sPath;
		m_sName     = rhs.m_sName;
		m_mMatrix   = rhs.m_mMatrix;
		m_vScale    = rhs.m_vScale;
		m_mScale    = rhs.m_mScale;
		m_fRadius   = rhs.m_fRadius;
		m_vBoxMin   = rhs.m_vBoxMin;
		m_vBoxMax   = rhs.m_vBoxMax;
		m_vCenter   = rhs.m_vCenter;
		m_vXAxis = rhs.m_vXAxis;
		m_vYAxis = rhs.m_vYAxis;
		m_vZAxis = rhs.m_vZAxis;
		return (*this);
	}
	Model::~Model() {}
	
	Model::Model(const std::string& s, bool bCompleteThePath)
	{
		m_mMatrix = Matrix::IDENTITY;
		m_vScale  = Vector3(1,1,1);
		m_mScale  = Matrix::IDENTITY;
		
		load(s, bCompleteThePath);
	}
	
	void Model::load(const std::string& s, bool bCompleteThePath) {
		std::string sPath;
		if (bCompleteThePath)
			sPath = s_sPathStart + s + s_sPathEnd;
		else
			sPath = s;
		
		s_mpi = s_mpModel.find(sPath);
		if (s_mpi == s_mpModel.end()) {
			throw Exception("OFF model requested to be loaded does not exist: " + sPath, "load", "RoeModel.cpp", ROE_LINE);
		}
		
		m_sPath = sPath;
		m_sName = s;
		
		update();
	}
	
	void Model::update() {
		m_mScale    = Matrix::scaling(m_vScale);
		m_vCenter   = Matrix::transformCoords(Vector3(0,0,0),  m_mScale * m_mMatrix);
		m_vXAxis    = Vector3::normalize(Matrix::transformCoords(Vector3(1,0,0), m_mScale * m_mMatrix) - m_vCenter);
		m_vYAxis    = Vector3::normalize(Matrix::transformCoords(Vector3(0,1,0), m_mScale * m_mMatrix) - m_vCenter);
		m_vZAxis    = Vector3::normalize(Matrix::transformCoords(Vector3(0,0,1), m_mScale * m_mMatrix) - m_vCenter);
		m_fRadius   = (Matrix::transformCoords(s_mpModel[m_sPath].vFarthest, m_mScale * m_mMatrix) - m_vCenter).length();
	}
	
	void Model::transform(const Matrix& m) {
		m_mMatrix *= m;
		update();
	}
	void   Model::setTransformation(const Matrix& m) {m_mMatrix = m; update();}
	Matrix Model::getTransformation(const Matrix& m) {return m_mMatrix;}
	
	void Model::render() {
		glPushMatrix();
			glMultMatrixf((m_mScale * m_mMatrix).n);
			glCallList(s_mpModel[m_sPath].iDispList);
		glPopMatrix();
	}
	
	void Model::quit() {
		for(s_mpi=s_mpModel.begin(); s_mpi!=s_mpModel.end(); ++s_mpi) {
			glDeleteLists(s_mpi->second.iDispList, 1);
		}
		s_mpModel.clear();
	}
	void Model::deleteOFF (std::string sPath, bool bCompleteThePath){
		if (bCompleteThePath)
			sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpModel.find(sPath);
		if (s_mpi == s_mpModel.end()) {
			throw Exception("OFF model to be deleted does not exist", "deleteOFF", "RoeModel.cpp", ROE_LINE);
		} else {
			glDeleteLists(s_mpi->second.iDispList, 1);
			s_mpModel.erase(s_mpi);
		}
	}
	
	void Model::loadOFF(std::string sPath, bool bCompleteThePath, ECWCCW cwccw,
	                    const float pfSmoothingAngle, const Matrix& mTransform) {
		if (bCompleteThePath)
			sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpModel.find(sPath);
		if (s_mpi == s_mpModel.end()) {
			
			std::ifstream file_in(sPath.c_str(), std::ios_base::in);
			if (file_in) {
				int iTmp = glGenLists(1);
				S_ModelIntern modTmp;     //tmp structure for the map
				modTmp.iDispList = iTmp;
				modTmp.vFarthest =  Vector3(0.0f,0.0f,0.001f);
				modTmp.vBoxMin   = -Vector3(0.001f,0.001f,0.001f);
				modTmp.vBoxMax   =  Vector3(0.001f,0.001f,0.001f);
				
				char ch[400];
				
				glNewList(iTmp, GL_COMPILE);
					file_in.getline (ch,398);
					file_in.getline (ch,398,' ');
					const int ciNumVertices = strtoul(ch,NULL,0);
					file_in.getline (ch,398,' ');
					const int ciNumSurfaces = strtoul(ch,NULL,0);
					file_in.getline (ch,398);
					
					Vector3 *va = new Vector3[ciNumVertices];
					for (int i = 0; i < ciNumVertices; i++) {
						file_in.getline (ch,398,' ');
						va[i].x = strtod(ch,NULL);
						file_in.getline (ch,398,' ');
						va[i].y = strtod(ch,NULL);
						file_in.getline (ch,398);
						va[i].z = strtod(ch,NULL);
						
						//apply matrix
						va[i] = Matrix::transformCoords(va[i], mTransform);
						
						//map update
						if(va[i].length() >= modTmp.vFarthest.length()) { //serch longest vector
							modTmp.vFarthest = va[i];
						}
						modTmp.vBoxMin = Vector3::floor(modTmp.vBoxMin, va[i]);
						modTmp.vBoxMax = Vector3::ceil (modTmp.vBoxMax, va[i]);
					}
					
					
					
					if (glIsEnabled(GL_LIGHTING)) {
						S_ModelInternOneSurface *aSurfaces = new S_ModelInternOneSurface[ciNumSurfaces];
						
						//loading surfaces
						for (int i = 0; i < ciNumSurfaces; i++) {
							file_in.getline (ch,398,' ');
							aSurfaces[i].iNumVertices = strtoul(ch,NULL,0);
								
							if (aSurfaces[i].iNumVertices >= 3) { //lighting and surface, so calc normals
								//loading vertices
								for (int j = 0; j < aSurfaces[i].iNumVertices - 1; j++) {
									file_in.getline (ch,398,' ');
									aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,0) ]);
								}
								file_in.getline (ch,398);
								aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,0) ]);
								
								//if CW, invert vector:
								if (cwccw == CW_CW) {
									for (int j = 0; j < aSurfaces[i].iNumVertices / 2; j++) {
										Vector3 swapTmp = aSurfaces[i].vVertices[j];
										aSurfaces[i].vVertices[j] = aSurfaces[i].vVertices[aSurfaces[i].iNumVertices-1-j];
										aSurfaces[i].vVertices[aSurfaces[i].iNumVertices-1-j] = swapTmp;
									}
								}
								
								//calculate normals
								aSurfaces[i].vNormal = Vector3::normalize(Vector3::crossProduct((aSurfaces[i].vVertices[1] - aSurfaces[i].vVertices[0]),
								                                                                (aSurfaces[i].vVertices[2] - aSurfaces[i].vVertices[0]) ) );
								
							}
							else if (aSurfaces[i].iNumVertices >= 1) {
								for (int j = 0; j < aSurfaces[i].iNumVertices - 1; j++) {
									file_in.getline (ch,398,' ');
									aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,0) ]);
								}
								file_in.getline (ch,398);
								aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,0) ]);
								aSurfaces[i].vNormal = Vector3(0.0f,1.0f,0.0f); //simple top down normal for lines
							}
							else {throw Exception("model 0 vertices: " + sPath, "loadOFF", "RoeModel.cpp", ROE_LINE);}
						}
						
						//to OpenGL:
						Vector3 vNormal;
						std::vector<Vector3> vOtherNormals;
						std::vector<Vector3> vFittingOtherNormals;
						int iNumSurfacesWithThatVertex = 0;
						for (int i = 0; i < ciNumSurfaces; i++) {
							if      (aSurfaces[i].iNumVertices == 1)  glBegin(GL_POINTS);
							else if (aSurfaces[i].iNumVertices == 2)  glBegin(GL_LINES);
							else if (aSurfaces[i].iNumVertices == 3)  glBegin(GL_TRIANGLES);
							else if (aSurfaces[i].iNumVertices == 4)  glBegin(GL_QUADS);
							else if (aSurfaces[i].iNumVertices >= 5)  glBegin(GL_POLYGON);
							else  {throw Exception("error in model-datei: " + sPath, "loadOFF", "RoeModel.cpp", ROE_LINE);}
								
								if (aSurfaces[i].iNumVertices >= 3) { //polygons
									vNormal = aSurfaces[i].vNormal;
									
									for (int j = 0; j < aSurfaces[i].iNumVertices; j++) {
										//Smoothing for normals
										if (pfSmoothingAngle > 0.0f) {
											iNumSurfacesWithThatVertex = 0;
											for (int k = 0; k < ciNumSurfaces; k++) {
												for (int l = 0; l < aSurfaces[k].iNumVertices; l++) {
													if (aSurfaces[k].vVertices[l] == aSurfaces[i].vVertices[j]) {
														vOtherNormals.push_back(aSurfaces[k].vNormal);
														iNumSurfacesWithThatVertex++;
														break;
													}
												}
											}
											for (int k = 0; k < iNumSurfacesWithThatVertex; k++) {
												if (ROE_RAD_TO_DEG(Vector3::angleBetween(vNormal, vOtherNormals[k])) < pfSmoothingAngle) {
													vFittingOtherNormals.push_back(vOtherNormals[k]);
												}
											}
											for (int k = 0; k < static_cast<int>(vFittingOtherNormals.size()); k++) {
												vNormal += vFittingOtherNormals[k];
											}
											vNormal = Vector3::normalize(vNormal);
										}
										//end of smoothing
										glNormal3fv( vNormal.rg);
										//glNormal3fv( Vector3(0,1,0).rg);
										glVertex3fv(aSurfaces[i].vVertices[j].rg);
										
										vOtherNormals.clear();
										vFittingOtherNormals.clear();
									}
								}
								else { //lines/points -> no normals needed
									for (int j = 0; j < aSurfaces[i].iNumVertices; j++) {
										glVertex3fv(aSurfaces[i].vVertices[j].rg);
									}
								}
							
							glEnd();
						}
						
						
						for (int i = 0; i < ciNumSurfaces; i++) {
							aSurfaces[i].vVertices.clear();
						}
						delete[] aSurfaces;
					}
					else { //no lighting, so just vertices
						int iTmp2 = 0;
						for (int i = 0; i < ciNumSurfaces; i++) {
							file_in.getline (ch,398,' ');
							iTmp2 = strtoul(ch,NULL,0);
							
							if      (iTmp2 == 1) glBegin(GL_POINTS);
							else if (iTmp2 == 2) glBegin(GL_LINES);
							else if (iTmp2 == 3) glBegin(GL_TRIANGLES);
							else if (iTmp2 == 4) glBegin(GL_QUADS);
							else if (iTmp2 >= 5) glBegin(GL_POLYGON);
							else     throw Exception("error in model file: " + sPath, "loadOFF", "RoeModel.cpp", ROE_LINE);
							
								for (int j = 0; j < iTmp2 - 1; j++) {
									file_in.getline (ch,398,' ');
									glVertex3fv( va[ strtoul(ch,NULL,0) ].rg );
								}
								file_in.getline (ch,398);
								glVertex3fv( va[ strtoul(ch,NULL,0) ].rg );
							
							glEnd();
						}
					}
					delete[] va;
				glEndList();
				file_in.close();
				
				//add to map
				s_pair = make_pair(sPath, modTmp);
				s_mpModel.insert(s_pair);
			} else {
				throw Exception("requested model does not exist: " + sPath, "loadOFF", "RoeModel.cpp", ROE_LINE);
			}
			
		} else {
			throw Exception("tried to load a model again: " + sPath, "loadOFF", "RoeModel.cpp", ROE_LINE);
		}
		
	}
	
	Model Model::createTerrain(std::string imagePath, Vector3 vScale) { //creates an terrain
		std::string name = "res/tmp/" + Util::removeFileExtensionOfString(imagePath) + ".off";
		Texture::createHeightmapOFF(imagePath, name);
		Model::loadOFF(name, false, CW_CCW, 0, Matrix::scaling(vScale));
		//Util::removeFile(name);
		Model mod(name, false);
		return mod;
	}
	
} // namespace Roe


