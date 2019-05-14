#include "CLevel.h"

CLevel::CLevel() : m_iTextSize(20), m_colText(1.0f, 0.0f, 0.0f, 1.0f)
{
	ttfSans.Load("FreeSans", m_iTextSize);
	m_TexRaster.Load("raster");
}

CLevel::~CLevel()
{
	m_lBlock.clear();
}


void CLevel::Update() {
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {
		m_itl->Update();
	}
}


void CLevel::Render() {
	
	//blocks (DIE HINTEREN ZUERST!)
	switch (g_pGS->m_UserFace) {
		case CF_FRONT:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BACK  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT) m_itl->Render();}
			break;
		case CF_BACK:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BACK) m_itl->Render();}
			break;
		case CF_LEFT:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_RIGHT  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_LEFT) m_itl->Render();}
			break;
		case CF_RIGHT:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_LEFT  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP || m_itl->GetFace() == CF_BOTTOM || m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			break;
		case CF_TOP:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BOTTOM  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP) m_itl->Render();}
			break;
		case CF_BOTTOM:
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_TOP  ) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_FRONT || m_itl->GetFace() == CF_BACK || m_itl->GetFace() == CF_LEFT || m_itl->GetFace() == CF_RIGHT) m_itl->Render();}
			for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {if (m_itl->GetFace() == CF_BOTTOM) m_itl->Render();}
			break;
	}
	
	//raster, or grid
	DrawRaster(GL_RENDER);
	
	//texte
	tbVector2 vPos(0,ALLH - m_iTextSize), vTmp;
	glcFont::Begin();
		ttfSans.RenderText(m_sTitle, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
		
		char cTmp[400];
		string sTmp;
		
		sprintf(cTmp,"GRID : %.3f", g_pGS->m_fRasterSize);
		sTmp = cTmp;
		ttfSans.RenderText(sTmp, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
		
		sprintf(cTmp,"BT%i  : ", static_cast<int>(g_pGS->m_UserBT));
		sTmp = cTmp;
		switch (g_pGS->m_UserBT) {
			case BT_SOLID:	sTmp += "solid";	break;
			case BT_GOAL:	sTmp += "goal";	break;
			case BT_KILL:	sTmp += "kill";	break;
			default: break;
		}
		ttfSans.RenderText(sTmp, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
		
		sprintf(cTmp,"CF%i  : ", static_cast<int>(g_pGS->m_UserFace));
		sTmp = cTmp;
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:	sTmp += "front";	break;
			case CF_RIGHT:	sTmp += "right";	break;
			case CF_LEFT:	sTmp += "left";		break;
			case CF_BACK:	sTmp += "back";		break;
			case CF_TOP:	sTmp += "top";		break;
			case CF_BOTTOM:	sTmp += "bottom";	break;
			default: break;
		}
		ttfSans.RenderText(sTmp, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
		
		sprintf(cTmp,"DD%i  : ", static_cast<int>(g_pGS->m_DownDir));
		sTmp = cTmp;
		switch (g_pGS->m_DownDir) {
			case DD_DOWN:	sTmp += "down";		break;
			case DD_UP:		sTmp += "up";		break;
			case DD_LEFT:	sTmp += "left";		break;
			case DD_RIGHT:	sTmp += "right";	break;
			default: break;
		}
		ttfSans.RenderText(sTmp, m_colText, vPos, &vTmp); vPos.y -= vTmp.y;
	glcFont::End();
}


void CLevel::LoadLevel(string sPath) {
	g_Logfile << "Level wird geladen: " + sPath;
	
	//level einlesen
	char cText[400];
	ifstream dat_in(sPath.data(), ios_base::in);
	
	if (!dat_in)
		throw glcException(sPath + " fehlt");
	
	//titel
	if (!dat_in.eof()) {
		dat_in.getline (cText,398);
		m_sTitle = cText;
	}
	
	//player
	if (!dat_in.eof()) {
		dat_in.getline (cText,398);
		g_pGS->m_sPlayerInit = cText;
	}
	
	//bloecke
	string sTmp;
	CBlock tmpBlock;
	while (! dat_in.eof() ) {
		dat_in.getline (cText,398);
		sTmp = cText;
		if (static_cast<int> (sTmp.length() ) > 5) {
			tmpBlock.Load(cText);
			m_lBlock.push_back(tmpBlock);
		}
	}
	
}

void CLevel::SaveLevel() {
	g_Logfile << "Level wird gespeichert: " + g_pGS->m_sArg1;
	
	ofstream dat_out (g_pGS->m_sArg1.data(), ios_base::out | ios_base::trunc);
	
	if (static_cast<int> (m_sTitle.length()) != 0) {
		dat_out << m_sTitle.data() << endl;
	} else {
		dat_out << "///ENTER TITLE OF LEVEL HERE///" << endl;
	}
	
	if (static_cast<int> (g_pGS->m_sPlayerInit.length()) != 0) {
		dat_out << g_pGS->m_sPlayerInit.data();
	} else {
		dat_out << "///ENTER PLAYER INFORMATION HERE///";
	}
	
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {
		dat_out << endl;
		dat_out << m_itl->GetStr();
	}
	
	dat_out.close();
}

void CLevel::DeleteBlock() {
	tbVector2 vTmp;
	if (!MouseFaceCollision(&vTmp))
		return;
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {
		if (m_itl->Collision(vTmp, tbVector2(g_pGS->m_fRasterSize,g_pGS->m_fRasterSize) )) {
			m_lBlock.erase(m_itl);
			m_itl = m_lBlock.begin();
		}
	}
}

void CLevel::LeftClick() {
	tbVector2 vTmp;
	if (!MouseFaceCollision(&vTmp))
		return;
	
	char cTmp[400];
	sprintf(cTmp, "%i  %f  %f  %f  %f  %i", g_pGS->m_UserFace, vTmp.x, vTmp.y, g_pGS->m_fRasterSize, g_pGS->m_fRasterSize, g_pGS->m_UserBT);
	CBlock block(cTmp);
	bool bOK = true;
	for (m_itl = m_lBlock.begin(); m_itl != m_lBlock.end(); m_itl++) {
		if (m_itl->Collision(vTmp, tbVector2(g_pGS->m_fRasterSize,g_pGS->m_fRasterSize))) {
			bOK = false;
			break;
		}
	}
	if (bOK) {
		m_lBlock.push_back(block);
	} else {
	}
}

bool CLevel::MouseFaceCollision (tbVector2 *pv) {
	static const int iBufsize = 2600;
	if (pv) {
		// We don't want to modify the projection matrix.
		glMatrixMode( GL_MODELVIEW );
    	glLoadIdentity( );
    	
		//cameramatrix festsetzen (zuallererst!)
		g_pGL->LookAt();
		
		
		GLuint selectBuf[iBufsize];
		GLint hits;
		GLint viewport[4];
		
		glGetIntegerv (GL_VIEWPORT, viewport);
		
		glSelectBuffer (iBufsize, selectBuf);
		glRenderMode (GL_SELECT);
	
		glInitNames();
		glPushName(0);
	
		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();
		gluPickMatrix ((GLdouble) g_pGL->GetMouseXPosi(), (GLdouble) (viewport[3] - g_pGL->GetMouseYPosi()), 10.0, 10.0, viewport);
		
		gluPerspective( 60.0, static_cast<float>(ALLW) / ALLH, 1.0, 1024.0 );
		
		DrawRaster(GL_SELECT);
		
		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();
		
		hits = glRenderMode (GL_RENDER);
		
		if (hits == 0)
			return false;
		
		*pv = ProcessHits(hits, selectBuf);
		
		return true;
	}
	return false;
	
}

void CLevel::DrawRaster(GLenum mode) {
	const int iNumRasters = static_cast<int> (CUBE_SIZE / g_pGS->m_fRasterSize);
	const float fHalfCubeSize = CUBE_SIZE / 2.0f - 0.001f; //offset testwert
	tbVector3 vTmp[4];
	for (int i = 0; i < iNumRasters; i++) {
		for (int j = 0; j < iNumRasters; j++) {
			//todo selection
			if (mode == GL_SELECT) {
				glLoadName(i * iNumRasters + j);
			}
			
			m_TexRaster.Bind();
			switch (g_pGS->m_UserFace) {
				case CF_FRONT:
					vTmp[0] = tbVector3(j * g_pGS->m_fRasterSize -fHalfCubeSize, i * g_pGS->m_fRasterSize -fHalfCubeSize, fHalfCubeSize);
					vTmp[1] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y, vTmp[0].z);
					vTmp[2] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z);
					vTmp[3] = tbVector3(vTmp[0].x, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z);
					break;
				case CF_BACK:
					vTmp[0] = tbVector3(-1.0f * ((j+1) * g_pGS->m_fRasterSize -fHalfCubeSize), i * g_pGS->m_fRasterSize -fHalfCubeSize,-fHalfCubeSize);
					vTmp[1] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y, vTmp[0].z);
					vTmp[2] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z);
					vTmp[3] = tbVector3(vTmp[0].x, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z);
					break;
				case CF_RIGHT:
					vTmp[0] = tbVector3( fHalfCubeSize, i * g_pGS->m_fRasterSize -fHalfCubeSize, -1.0f * ((j+1) * g_pGS->m_fRasterSize -fHalfCubeSize) );
					vTmp[1] = tbVector3(vTmp[0].x, vTmp[0].y, vTmp[0].z + g_pGS->m_fRasterSize);
					vTmp[2] = tbVector3(vTmp[0].x, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z + g_pGS->m_fRasterSize);
					vTmp[3] = tbVector3(vTmp[0].x, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z);
					break;
				case CF_LEFT:
					vTmp[0] = tbVector3(-fHalfCubeSize, i * g_pGS->m_fRasterSize -fHalfCubeSize, j * g_pGS->m_fRasterSize -fHalfCubeSize);
					vTmp[1] = tbVector3(vTmp[0].x, vTmp[0].y, vTmp[0].z + g_pGS->m_fRasterSize);
					vTmp[2] = tbVector3(vTmp[0].x, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z + g_pGS->m_fRasterSize);
					vTmp[3] = tbVector3(vTmp[0].x, vTmp[0].y + g_pGS->m_fRasterSize, vTmp[0].z);
					break;
				case CF_TOP:
					vTmp[0] = tbVector3(j * g_pGS->m_fRasterSize -fHalfCubeSize, fHalfCubeSize, -1.0f * ((i+1) * g_pGS->m_fRasterSize -fHalfCubeSize) );
					vTmp[1] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y, vTmp[0].z);
					vTmp[2] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y, vTmp[0].z + g_pGS->m_fRasterSize);
					vTmp[3] = tbVector3(vTmp[0].x, vTmp[0].y, vTmp[0].z + g_pGS->m_fRasterSize);
					break;
				case CF_BOTTOM:
					vTmp[0] = tbVector3(j * g_pGS->m_fRasterSize -fHalfCubeSize,-fHalfCubeSize, i * g_pGS->m_fRasterSize -fHalfCubeSize);
					vTmp[1] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y, vTmp[0].z);
					vTmp[2] = tbVector3(vTmp[0].x + g_pGS->m_fRasterSize, vTmp[0].y, vTmp[0].z + g_pGS->m_fRasterSize);
					vTmp[3] = tbVector3(vTmp[0].x, vTmp[0].y, vTmp[0].z + g_pGS->m_fRasterSize);
					break;
			}
			glBegin (GL_QUADS);
				glTexCoord2i(0,0); glVertex3fv(vTmp[0].c);
				glTexCoord2i(1,0); glVertex3fv(vTmp[1].c);
				glTexCoord2i(1,1); glVertex3fv(vTmp[2].c);
				glTexCoord2i(0,1); glVertex3fv(vTmp[3].c);
			glEnd();
		}
	}		
}

tbVector2 CLevel::ProcessHits (GLint hits, GLuint buffer[])
{
	//tbVector2 vTmp(-CUBE_SIZE/2.0f,-CUBE_SIZE/2.0f);
	tbVector2 vTmp(0.0f,0.0f);
	
	int iTmp = -1;
	
	unsigned int i, j;
	GLuint names, *ptr;
	
	//printf ("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	for (i = 0; static_cast<int>(i) < hits; i++) { /*  for each hit  */
		names = *ptr;
		//printf (" number of names for hit = %d\n", names);
		ptr++;
		//printf("  z1 is %g;", (float) *ptr/0x7fffffff);
		ptr++;
		//printf(" z2 is %g\n", (float) *ptr/0x7fffffff);
		ptr++;
		//printf ("   the name is ");
		for (j = 0; j < names; j++) {     /*  for each name */
			if (iTmp == -1) iTmp = (*ptr);
			//printf ("%d ", *ptr);
			ptr++;
		}
		//printf ("\n");
	}
	
	vTmp += tbVector2( (iTmp % (static_cast<int>(CUBE_SIZE / g_pGS->m_fRasterSize) ) ) * g_pGS->m_fRasterSize,
					  static_cast<int>(iTmp / (static_cast<int>(CUBE_SIZE / g_pGS->m_fRasterSize) ) ) * g_pGS->m_fRasterSize);
	
	//cout << iTmp << endl;
	//cout << vTmp << endl;			  
	return vTmp;
}

