#include "glcModel.h"

map<string, S_ModelIntern> glcModel::c_mpModel;
map<string, S_ModelIntern>::iterator glcModel::c_mpi;
pair<string, S_ModelIntern> glcModel::c_pair;
string glcModel::c_sPathStart, glcModel::c_sPathEnd = ".off";


//methoden
glcModel::glcModel()	{
	m_mMatrix	= tbMatrixIdentity();
	m_vScale	= nmath::vMaxOne;
	m_mScale	= tbMatrixIdentity();
}
glcModel::~glcModel()									{}

glcModel::glcModel(const string &s)
{
	m_mMatrix	= tbMatrixIdentity();
	m_vScale	= nmath::vMaxOne;
	m_mScale	= tbMatrixIdentity();
	
	Load(s);
}

void glcModel::Load(const string &s) {
	string sPath = c_sPathStart + s + c_sPathEnd;
	
	c_mpi = c_mpModel.find(sPath);
	if (c_mpi == c_mpModel.end()) {
		throw glcException("angefragtes model noch nicht geladen");
	}
	
	m_sPath = sPath;
	m_sName = s;
	
	Update();
}

void glcModel::Update() {
	m_mScale	= tbMatrixScaling(m_vScale);
	m_vCenter 	= tbVector3TransformCoords(nmath::vZero,  m_mScale * m_mMatrix);
	m_vXAxis	= tbVector3Normalize(tbVector3TransformCoords(nmath::vXAxis, m_mScale * m_mMatrix) - m_vCenter);
	m_vYAxis	= tbVector3Normalize(tbVector3TransformCoords(nmath::vYAxis, m_mScale * m_mMatrix) - m_vCenter);
	m_vZAxis	= tbVector3Normalize(tbVector3TransformCoords(nmath::vZAxis, m_mScale * m_mMatrix) - m_vCenter);
	m_fRadius	= tbVector3Length(tbVector3TransformCoords(c_mpModel[m_sPath].vFarthest, m_mScale * m_mMatrix) - m_vCenter);
}

void glcModel::Transform(const tbMatrix &m) {
	m_mMatrix *= m;
	Update();
}

void glcModel::Render() {
	glPushMatrix();
		g_pGL->MultMatrix(m_mScale * m_mMatrix);
		glCallList(c_mpModel[m_sPath].iDispList);
	glPopMatrix();
}

void glcModel::Quit()	{
	for(c_mpi=c_mpModel.begin(); c_mpi!=c_mpModel.end(); ++c_mpi) {
		glDeleteLists(c_mpi->second.iDispList, 1);
	}
	c_mpModel.clear();
}
void glcModel::DeleteOFF	(string sPath, bool bCompleteThePath)	{
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpModel.find(sPath);
	if (c_mpi == c_mpModel.end()) {
		throw glcException("OFF model to be deleted does not exist");
	} else {
		glDeleteLists(c_mpi->second.iDispList, 1);
		c_mpModel.erase(c_mpi);
	}
}

void glcModel::LoadOFF(string sPath, const float pfSmoothingAngle, const tbMatrix &mTransform) { //(TODO) auto smothing lighting!
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpModel.find(sPath);
	if (c_mpi == c_mpModel.end()) {
		
		ifstream file_in(sPath.data(), ios_base::in);
		if (file_in) {
			int iTmp = glGenLists(1);
			S_ModelIntern modTmp;					//struktur fuer die map
			modTmp.iDispList 	= iTmp;
			modTmp.vFarthest 	= tbVector3(0.0f,0.0f,0.001f);
			modTmp.vBoxMin		= -tbVector3(0.001f,0.001f,0.001f);
			modTmp.vBoxMax		=  tbVector3(0.001f,0.001f,0.001f);
			
			char ch[400];
			
			glNewList(iTmp, GL_COMPILE);
				file_in.getline (ch,398);
				file_in.getline (ch,398,' ');
				const int ciNumVertices = strtoul(ch,NULL,NULL);
				file_in.getline (ch,398,' ');
				const int ciNumSurfaces = strtoul(ch,NULL,NULL);
				file_in.getline (ch,398);
				
				tbVector3 va[ciNumVertices];
				for (int i = 0; i < ciNumVertices; i++) {
					file_in.getline (ch,398,' ');
					va[i].x = strtod(ch,NULL);
					file_in.getline (ch,398,' ');
					va[i].z = strtod(ch,NULL);			// y- und z-achse wegen blender vertauscht
					file_in.getline (ch,398);
					va[i].y = strtod(ch,NULL);
					
					//matrix anwenden
					va[i] = tbVector3TransformCoords(va[i], mTransform);
					
					//map-struktur updaten
					if(tbVector3Length(va[i]) >= tbVector3Length(modTmp.vFarthest)) {	// den laengsten vektor suchen
						modTmp.vFarthest = va[i];
					}
					modTmp.vBoxMin = tbVector3Min(modTmp.vBoxMin, va[i]);
					modTmp.vBoxMax = tbVector3Max(modTmp.vBoxMax, va[i]);
				}
				
				
				
				if (glIsEnabled(GL_LIGHTING)) {
					S_ModelInternOneSurface aSurfaces[ciNumSurfaces];
					
					//flaechen laden
					for (int i = 0; i < ciNumSurfaces; i++) {
						file_in.getline (ch,398,' ');
						aSurfaces[i].iNumVertices = strtoul(ch,NULL,NULL);
							
						if (aSurfaces[i].iNumVertices >= 3) {	//lighting UND flaeche, also normalenvektoren berechnen
							//ecken laden
							for (int j = 0; j < aSurfaces[i].iNumVertices - 1; j++) {
								file_in.getline (ch,398,' ');
								aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,NULL) ]);
							}
							file_in.getline (ch,398);
							aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,NULL) ]);
							
							//normale berechnen
							aSurfaces[i].vNormal = tbVector3Normalize( tbVector3Cross(  (aSurfaces[i].vVertices[1] - aSurfaces[i].vVertices[0]),
																						(aSurfaces[i].vVertices[2] - aSurfaces[i].vVertices[0]) ) );
							
						}
						else if (aSurfaces[i].iNumVertices >= 1) {
							for (int j = 0; j < aSurfaces[i].iNumVertices - 1; j++) {
								file_in.getline (ch,398,' ');
								aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,NULL) ]);
							}
							file_in.getline (ch,398);
							aSurfaces[i].vVertices.push_back(va[ strtoul(ch,NULL,NULL) ]);
							aSurfaces[i].vNormal = tbVector3(0.0f,1.0f,0.0f);	//simple top down normal for lines
						}
						else {throw glcException("model 0 vertexe: " + sPath);}
					}
					
					//an OpenGL schicken
					tbVector3 vNormal;
					vector<tbVector3> vOtherNormals;
					vector<tbVector3> vFittingOtherNormals;
					int iNumSurfacesWithThatVertex = 0;
					for (int i = 0; i < ciNumSurfaces; i++) {
						if 		(aSurfaces[i].iNumVertices == 1)	glBegin(GL_POINTS);
						else if (aSurfaces[i].iNumVertices == 2)	glBegin(GL_LINES);
						else if (aSurfaces[i].iNumVertices == 3)	glBegin(GL_TRIANGLES);
						else if (aSurfaces[i].iNumVertices == 4) 	glBegin(GL_QUADS);
						else if (aSurfaces[i].iNumVertices >= 5)	glBegin(GL_POLYGON);
						else					{throw glcException("fehler in model-datei: " + sPath);}
							
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
											if (TB_RAD_TO_DEG(tbVector3Angle(vNormal, vOtherNormals[k])) < pfSmoothingAngle) {
												vFittingOtherNormals.push_back(vOtherNormals[k]);
											}
										}
										for (int k = 0; k < static_cast<int>(vFittingOtherNormals.size()); k++) {
											vNormal += vFittingOtherNormals[k];
										}
										vNormal = tbVector3Normalize(vNormal);
									}
									//end of smoothing
									glNormal3fv( vNormal.c);
									glVertex3fv(aSurfaces[i].vVertices[j].c);
									
									vOtherNormals.clear();
									vFittingOtherNormals.clear();
								}
							}
							else { //lines/points -> no normals needed
								for (int j = 0; j < aSurfaces[i].iNumVertices; j++) {
									glVertex3fv(aSurfaces[i].vVertices[j].c);
								}
							}
						
						glEnd();
					}
					
					
					for (int i = 0; i < ciNumSurfaces; i++) {
						aSurfaces[i].vVertices.clear();
					}
				}
				else {		//no lighting, so just vertices
					int iTmp2 = 0;
					for (int i = 0; i < ciNumSurfaces; i++) {
						file_in.getline (ch,398,' ');
						iTmp2 = strtoul(ch,NULL,NULL);
						
						if 		(iTmp2 == 1)	glBegin(GL_POINTS);
						else if (iTmp2 == 2)	glBegin(GL_LINES);
						else if (iTmp2 == 3)	glBegin(GL_TRIANGLES);
						else if (iTmp2 == 4) 	glBegin(GL_QUADS);
						else if (iTmp2 >= 5)	glBegin(GL_POLYGON);
						else					throw glcException("fehler in model-datei: " + sPath);
						
							for (int j = 0; j < iTmp2 - 1; j++) {
								file_in.getline (ch,398,' ');
								glVertex3fv( va[ strtoul(ch,NULL,NULL) ].c );
							}
							file_in.getline (ch,398);
							glVertex3fv( va[ strtoul(ch,NULL,NULL) ].c );
						
						glEnd();
					}
				}
					
			glEndList();
			file_in.close();
			
			//map erweitern
			c_pair = make_pair (sPath.data(), modTmp);
			c_mpModel.insert(c_pair);
		} else {
			throw glcException("model-datei zum laden existiert nicht: " + sPath);
		}
		
	} else {
		throw glcException("2-mal dasselbe model geladen");
	}
	
}
