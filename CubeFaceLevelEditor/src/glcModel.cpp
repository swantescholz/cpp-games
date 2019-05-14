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


void glcModel::LoadOFF(string sPath, const tbMatrix &mTransform) {
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
				int iVertex = strtoul(ch,NULL,NULL);
				file_in.getline (ch,398,' ');
				int iSurface= strtoul(ch,NULL,NULL);
				file_in.getline (ch,398);
				
				tbVector3 va[iVertex];
				for (int i = 0; i < iVertex; i++) {
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
				int iTmp2;
				tbVector3 vTmp[3];
				
				if (glIsEnabled(GL_LIGHTING)) {
					for (int i = 0; i < iSurface; i++) {
						file_in.getline (ch,398,' ');
						iTmp2 = strtoul(ch,NULL,NULL);
						if (iTmp2 == 1)			glBegin(GL_POINTS);
						else if (iTmp2 == 2)	glBegin(GL_LINES);
						else if (iTmp2 == 3)	glBegin(GL_TRIANGLES);
						else if (iTmp2 == 4) 	glBegin(GL_QUADS);
						else if (iTmp2 >= 5)	glBegin(GL_POLYGON);
						else					{throw glcException("fehler in model-datei: " + sPath);}
							
							if (iTmp2 >= 3) {	//lighting UND flaeche, also normalenvektoren berechnen
								file_in.getline (ch,398,' ');
								vTmp[0] = va[ strtoul(ch,NULL,NULL) ];
								file_in.getline (ch,398,' ');
								vTmp[1] = va[ strtoul(ch,NULL,NULL) ];
								if (iTmp2 == 3)		file_in.getline (ch,398);
								else 				file_in.getline (ch,398,' ');
								vTmp[2] = va[ strtoul(ch,NULL,NULL) ];
								
								glNormal3fv( tbVector3Normalize( tbVector3Cross( (vTmp[1] - vTmp[0]), (vTmp[2] - vTmp[0]) ) ) ); // vielleich vectoren vertauschen (todo)
								
								for (int j = 0; j < 3; j++) {
									glVertex3fv(vTmp[j].c);
								}
								for (int j = 3; j < iTmp2-1; j++) {
									file_in.getline (ch,398,' ');
									glVertex3fv( va[ strtoul(ch,NULL,NULL) ].c );
								}
								if (iTmp2 >= 4) {		// bei dreicken oder weniger waere es schon ganz gerendert
									file_in.getline (ch,398);
									glVertex3fv( va[ strtoul(ch,NULL,NULL) ].c );
								}
							}
							else {
								for (int j = 0; j < iTmp2-1; j++) {
									file_in.getline (ch,398,' ');
									glVertex3fv( va[ strtoul(ch,NULL,NULL) ].c );
								}
								file_in.getline (ch,398);
								glVertex3fv( va[ strtoul(ch,NULL,NULL) ].c );
							}
						
						glEnd();
					}
				}
				else {
					for (int i = 0; i < iSurface; i++) {
						file_in.getline (ch,398,' ');
						iTmp2 = strtoul(ch,NULL,NULL);
						
						if (iTmp2 == 1)			glBegin(GL_POINTS);
						else if (iTmp2 == 2)	glBegin(GL_LINES);
						else if (iTmp2 == 3)	glBegin(GL_TRIANGLES);
						else if (iTmp2 == 4) 	glBegin(GL_QUADS);
						else if (iTmp2 >= 5)	glBegin(GL_POLYGON);
						else					throw glcException("fehler in model-datei: " + sPath);
						
							for (int j = 0; j < iTmp2-1; j++) {
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