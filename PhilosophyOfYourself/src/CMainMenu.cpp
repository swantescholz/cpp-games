#include "CMainMenu.h"

int CMainMenu::c_iNumInstances = 0;

CMainMenu::CMainMenu()
{
	glcTexture::LoadTexture("bg/bg_main_menu");
	m_SndStep.Load		("main_menu_step");
	m_SndStep.SetVolume (0.5f); //not so loud
	m_TexAnswer.Load	("answer");
	m_TexPlayer.Load	("player");
	m_TexArrow.Load		("arrow");
	m_TexLadder.Load	("ladder");
	m_TexWood.Load		("wood");
	m_TexBg.Load		("bg/bg_main_menu");
	glcLight::LightModel(tbColor(0,0,0,1));
	m_pLight1 = NULL; //this was once VERY important here
	 	
	glClearColor(0.2,0.2,0.2,1.0);
	
	for (int i = 0; i < 3; i++) {
		m_colStair[i]		= tbColor(1.0,0.0,0.0,1.0);
		m_colLeaveArr[i]	= tbColor(1.0,0.0,0.0,1.0);
		m_colCreditsArr[i]	= tbColor(1.0,0.0,0.0,1.0);
		m_colCredits[i]		= tbColor(1.0,0.0,0.0,1.0);
	}
	m_colLevelText	= tbColor(0.9,0.7,0.5,1.0);
	m_vStairWH		= tbVector2(2.0,1.5);
	m_vPlayerWH		= tbVector2(1.5f,3.5f);
	m_vGroundWH		= tbVector2(m_vStairWH.x*g_pGS->m_ciNumLevels*2.0f,4.0f);
	m_vGround2WH	= tbVector2(m_vGroundWH.x/2.0f,m_vGroundWH.y);
	m_vLadderWH		= tbVector2(2.0f,18.0f);
	m_vAnswerWH		= tbVector2(m_vStairWH.x, m_vStairWH.x*3.0f);
	m_vArrowWH		= tbVector2(5.0f,1.2f);
	m_vBgWH			= tbVector2(1.5f*m_vGroundWH.x,1.5f*(m_vLadderWH.y+m_vGroundWH.y+m_vGround2WH.y+m_vStairWH.y*g_pGS->m_ciNumLevels));
	m_iPlayerPlace	= g_pGS->m_iLevelToPlay;
	m_fDownSpeed = 1.0f;
	m_bFalling = true;
	m_vPlayerCenter = CalculatePlayerCenter();
	m_vPlayerCenter.y += m_vAnswerWH.y * 3.0f;
	
	
	//loading the level
	char ch[500];
	string sLine;
	ifstream dat_in;
	dat_in.open("res/lvl/main_menu.lvl", ios_base::in);
	if (!dat_in) {throw glcException("res/lvl/main_menu.lvl is missing");}
	while (!dat_in.eof()) {
		dat_in.getline (ch,498);
		sLine = ch;
		if (sLine.find("end")!=string::npos || sLine.find("End")!=string::npos || sLine.find("END")!=string::npos) {  
			break; //the end of the file
		}
		if (sLine.length() > 1) {
			if (sLine[0] != '/') {
				LoadLine(sLine);
			}
		}
	}
	dat_in.close();
	
	
	VCAM2 = m_vPlayerCenter;
	VCAM1 = tbVector3(m_vPlayerCenter.x,m_vPlayerCenter.y,m_fCam1Z);
	m_pLight1->Transform(tbMatrixTranslation(tbVector3(m_vPlayerCenter.x,m_vPlayerCenter.y,m_fLightZ)-m_pLight1->GetCenter()));
	CreateMosaics();
	
	c_iNumInstances++; //counting the instances
}

CMainMenu::~CMainMenu()
{
	glcTexture::DeleteTexture("bg/bg_main_menu");
	GLC_SAFE_DELETE(m_pLight1);
	if (m_Music.IsLoaded() == true && c_iNumInstances == 1) { //deleting music, because last instance
		glcMusic::Halt(); //stop playing first
		glcMusic::DeleteMusic(m_Music.GetName());
	}
	glDeleteLists(m_aiMosaicDisp[0], 1);
	glDeleteLists(m_aiMosaicDisp[1], 1);
	glDeleteLists(m_aiMosaicDisp[2], 1);
	
	c_iNumInstances--; //counting the instances
}



void CMainMenu::Update() {
	if (g_pGL->KeyWasDown(SDLK_ESCAPE)) {
		g_pGS->m_GS = GS_QUIT;
	}
	
	//cheats
	if (g_pGS->m_bDeveloper == true) {
		if (g_pGL->KeyWasDown(SDLK_1)) {
			if (g_pGS->m_iAnswerPlace >=2) g_pGS->m_iAnswerPlace--;
		}
		if (g_pGL->KeyWasDown(SDLK_2)) {
			if (g_pGS->m_iAnswerPlace < g_pGS->m_ciNumLevels) g_pGS->m_iAnswerPlace++;
		}
	}
	
	//player movement
	if (m_bFalling) {
		m_vPlayerCenter.y -= m_fDownSpeed * ELAPSED;
		m_fDownSpeed += 20.0f * ELAPSED;
		if (m_vPlayerCenter.y <= CalculatePlayerCenter().y + m_vAnswerWH.y && m_iPlayerPlace == g_pGS->m_iAnswerPlace) {
			g_pGS->m_iAnswerPlace++;
		} else if (m_vPlayerCenter.y <= CalculatePlayerCenter().y) {
			m_bFalling = false;
			g_pGL->ResetAllEvents();
		}
	} else {
		if (g_pGL->KeyWasDown(SDLK_LEFT)) {
			m_SndStep.Play();
			if (m_iPlayerPlace == -3) {				//quitting
				g_pGS->m_GS = GS_QUIT;
			} else if (m_iPlayerPlace == -2) {		//restarting
				//g_pGS->m_iAnswerPlace = 1;
				//g_pGS->m_GS = GS_INTRO;
			} else if (m_iPlayerPlace == -1) {
				m_iPlayerPlace = -3;
			} else if (m_iPlayerPlace >= 0) {
				m_iPlayerPlace--;
			}
		}
		if (g_pGL->KeyWasDown(SDLK_RIGHT)) {
			m_SndStep.Play();
			if (m_iPlayerPlace == -3) {
				m_iPlayerPlace = -1;
			} else if (m_iPlayerPlace < g_pGS->m_ciNumLevels-1 && m_iPlayerPlace != -2 && m_iPlayerPlace +1 < g_pGS->m_iAnswerPlace) {
				m_iPlayerPlace++;
			}
		}
		if (g_pGL->KeyWasDown(SDLK_DOWN)) {
			if (m_iPlayerPlace == -1) {
				m_SndStep.Play();
				m_iPlayerPlace = -2;
			}
		}
		if (g_pGL->KeyWasDown(SDLK_UP)) {
			if (m_iPlayerPlace == -2) {
				m_SndStep.Play();
				m_iPlayerPlace = -1;
			}
		}
		m_vPlayerCenter = CalculatePlayerCenter();
	}
	
	//level choosing
	if (g_pGL->KeyWasDown(SDLK_RETURN) || g_pGL->KeyWasDown(SDLK_SPACE)) {
		if (m_iPlayerPlace >= 0) {
			if (m_iPlayerPlace < g_pGS->m_ciNumLevels) {
				g_pGS->m_GS = GS_GAME;
				g_pGS->m_iLevelToPlay = m_iPlayerPlace;
			} else {
				g_pGS->m_GS = GS_QUIT; //quit
			}
		}
	}
	
	if (m_ciCamMode == 1) {
		tbVector3 vTmp;
		tbMatrix mSca, mRot, mRot2;
		if (g_pGL->KeyIsDown(SDLK_DOWN)) 		mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(SCALINGPERSECCAM*ELAPSED),1+(SCALINGPERSECCAM*ELAPSED),1+(SCALINGPERSECCAM*ELAPSED))));
		else if (g_pGL->KeyIsDown(SDLK_UP)) 	mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(-SCALINGPERSECCAM*ELAPSED),1+(-SCALINGPERSECCAM*ELAPSED),1+(-SCALINGPERSECCAM*ELAPSED))));
		if (g_pGL->KeyIsDown(SDLK_LEFT)) 				mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
		else if (g_pGL->KeyIsDown(SDLK_RIGHT))			mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(-ROTATIONPERSECCAM*ELAPSED)));
		if (g_pGL->KeyIsDown(SDLK_LCTRL))				mRot2 = tbMatrix(tbMatrixRotationAxis(tbVector3Cross(VCAM1, tbVector3(0.0f,1.0f,0.0f)),-TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
		else if (g_pGL->KeyIsDown(SDLK_LALT))			mRot2 = tbMatrix(tbMatrixRotationAxis(tbVector3Cross(VCAM1, tbVector3(0.0f,1.0f,0.0f)), TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
		if (g_pGL->KeyIsDown(SDLK_LEFT) || g_pGL->KeyIsDown(SDLK_RIGHT)) 	{VCAM1 -= VCAM2; VCAM1 = tbVector3TransformCoords(VCAM1,mRot); VCAM1 += VCAM2;} 
		if (g_pGL->KeyIsDown(SDLK_DOWN) || g_pGL->KeyIsDown(SDLK_UP)) 		{VCAM1 -= VCAM2; VCAM1 = tbVector3TransformCoords(VCAM1,mSca); VCAM1 += VCAM2;}
		if (g_pGL->KeyIsDown(SDLK_LCTRL) || g_pGL->KeyIsDown(SDLK_LALT))		{VCAM1 -= VCAM2; vTmp = VCAM1; VCAM1 = tbVector3TransformCoords(VCAM1,mRot2); if (nmath::Sign(VCAM1.x) != nmath::Sign(vTmp.x) || nmath::Sign(VCAM1.z) != nmath::Sign(vTmp.z)) VCAM1 = vTmp; VCAM1 += VCAM2;}
		
		if (glcKeyIsDown(SDLK_d)) m_pLight1->Transform(tbMatrixTranslation(tbVector3( 1,0,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_a)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(-1,0,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_e)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0, 1,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_q)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,-1,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_w)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,0,-1) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_s)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,0, 1) * 5.0 * g_pTimer->GetElapsed()));
	} else { // (m_ciCamMode == 0)
		VCAM2 = m_vPlayerCenter;
		VCAM1 = tbVector3InterpolateCoords(tbVector3(m_vPlayerCenter.x,m_vPlayerCenter.y,m_fCam1Z),VCAM1,pow(m_fCam1InterpolationFactor,ELAPSED));
		VCAM1.z = m_fCam1Z;
		tbVector3 vNewLight = tbVector3InterpolateCoords(m_vPlayerCenter,m_pLight1->GetCenter(),pow(m_fLightInterpolationFactor,ELAPSED));
		vNewLight.z = m_fLightZ;
		m_pLight1->Transform(tbMatrixTranslation(vNewLight-m_pLight1->GetCenter()));
	}
}

void CMainMenu::Render() {
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// We don't want to modify the projection matrix.
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
	
	//cameramatrix festsetzen (zuallererst!)
	g_pGL->LookAt();
	
	//light
	m_pLight1->Render();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tbColor(1,1,1,1).c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tbColor(1,1,1,1).c);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
	
	
	
	
	//vectors
	float fZValue = 0.000f;
	float fTextH = 1.0f;
	
	//bg
	fZValue = -0.001f;
	m_TexBg.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1.0f);
		poyDrawTexRect(m_ciBGNumPasses,	tbVector2(0,0),tbVector3(-m_vBgWH.x/2.0f,-m_vBgWH.y/2.0f,fZValue),
						 				tbVector2(1,0),tbVector3( m_vBgWH.x/2.0f,-m_vBgWH.y/2.0f,fZValue),
						 				tbVector2(1,1),tbVector3( m_vBgWH.x/2.0f, m_vBgWH.y/2.0f,fZValue),
						 				tbVector2(0,1),tbVector3(-m_vBgWH.x/2.0f, m_vBgWH.y/2.0f,fZValue));
	glEnd();
	
	//staircase
	glDisable(GL_TEXTURE_2D);
	glNormal3f(0,0,1.0f);
	glCallList(m_aiMosaicDisp[0]);
	glEnable(GL_TEXTURE_2D);
	
	//ground
	fZValue = 0.000f;
	glDisable(GL_TEXTURE_2D);
	glNormal3f(0,0,1.0f);
	glCallList(m_aiMosaicDisp[1]);
	glNormal3f(0,0,1.0f);
	glCallList(m_aiMosaicDisp[2]);
	glEnable(GL_TEXTURE_2D);
	
	//stair numbers
	fZValue = 0.001f;
	m_Font.Load("FreeSans",24);
	for (int i = 0; i < g_pGS->m_ciNumLevels; i++) {
		m_Font.RenderText(string("")+i,m_colStair,
			tbVector3((i+0)*m_vStairWH.x,(i+0)*m_vStairWH.y-m_vStairWH.y,fZValue),
			tbVector3((i+1)*m_vStairWH.x,(i+0)*m_vStairWH.y-m_vStairWH.y,fZValue),
			tbVector3((i+1)*m_vStairWH.x,(i+1)*m_vStairWH.y-m_vStairWH.y,fZValue),
			tbVector3((i+0)*m_vStairWH.x,(i+1)*m_vStairWH.y-m_vStairWH.y,fZValue));
	}
	
	//ladder
	fZValue = 0.002f;
	m_TexLadder.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		poyDrawTexRect(4,tbVector2(0,0),tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f),-m_vLadderWH.y,fZValue),
						 tbVector2(1,0),tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x,-m_vLadderWH.y,fZValue),
						 tbVector2(1,1),tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x,0,fZValue),
						 tbVector2(0,1),tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f),0,fZValue));
	glEnd();
	
	//answer
	fZValue = 0.003f;
	m_TexAnswer.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		poyDrawTexRect(4,
			tbVector2(0,0),tbVector3(m_vStairWH.x*g_pGS->m_iAnswerPlace,m_vStairWH.y*g_pGS->m_iAnswerPlace,fZValue),
			tbVector2(1,0),tbVector3(m_vStairWH.x*g_pGS->m_iAnswerPlace+m_vAnswerWH.x,m_vStairWH.y*g_pGS->m_iAnswerPlace,fZValue),
			tbVector2(1,1),tbVector3(m_vStairWH.x*g_pGS->m_iAnswerPlace+m_vAnswerWH.x,m_vStairWH.y*g_pGS->m_iAnswerPlace+m_vAnswerWH.y,fZValue),
			tbVector2(0,1),tbVector3(m_vStairWH.x*g_pGS->m_iAnswerPlace,m_vStairWH.y*g_pGS->m_iAnswerPlace+m_vAnswerWH.y,fZValue));
	glEnd();
	
	//arrows
	fZValue = 0.004f;
	m_TexArrow.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)-m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)+m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y,fZValue).c);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)+m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y+m_vArrowWH.y,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)-m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y+m_vArrowWH.y,fZValue).c);
		
		glTexCoord2f(1,0); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x,-m_vArrowWH.x,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+m_vArrowWH.y,-m_vArrowWH.x,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+m_vArrowWH.y,0,fZValue).c);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x,0,fZValue).c);
	glEnd();
	m_Font.Load("FreeSans",32);
	m_Font.RenderText("Leave the Philosophy of Yourself",m_colLeaveArr,
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)-m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y+m_vArrowWH.y,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)+m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y+m_vArrowWH.y,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)+m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y+m_vArrowWH.y+fTextH,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f)-m_vArrowWH.x/2.0f,1.2*m_vPlayerWH.y+m_vArrowWH.y+fTextH,fZValue));
	fTextH = 0.8f;
	m_Font.RenderText("Credits",m_colCreditsArr,
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+m_vArrowWH.y,0,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+m_vArrowWH.y,-m_vArrowWH.x,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+m_vArrowWH.y+fTextH,-m_vArrowWH.x,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+m_vArrowWH.y+fTextH,0,fZValue));
	
	//credits
	fZValue = 0.005f;
	m_Font.RenderText("Game by",m_colCredits,
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+4*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+4*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+4*fTextH+fTextH	,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+4*fTextH+fTextH	,fZValue));
	m_Font.RenderText("Swante Scholz",m_colCredits,
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+3*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+3*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+3*fTextH+fTextH	,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+3*fTextH+fTextH	,fZValue));
	/*m_Font.RenderText("Music by",m_colCredits,
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+1*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+1*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+1*fTextH+fTextH	,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+1*fTextH+fTextH	,fZValue));
	m_Font.RenderText("??????",m_colCredits,
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+0*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+0*fTextH		,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x+8.0f	,-m_vLadderWH.y+0*fTextH+fTextH	,fZValue),
		tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vLadderWH.x		,-m_vLadderWH.y+0*fTextH+fTextH	,fZValue));
	*/
		
	//player
	fZValue = 0.006f;
	m_TexPlayer.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPlayerCenter.x-m_vPlayerWH.x/2.0f,m_vPlayerCenter.y-m_vPlayerWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPlayerCenter.x+m_vPlayerWH.x/2.0f,m_vPlayerCenter.y-m_vPlayerWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPlayerCenter.x+m_vPlayerWH.x/2.0f,m_vPlayerCenter.y+m_vPlayerWH.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPlayerCenter.x-m_vPlayerWH.x/2.0f,m_vPlayerCenter.y+m_vPlayerWH.y/2.0f,fZValue).c);
	glEnd();
	
	//ortho text
	if (m_iPlayerPlace >= 0 && m_bFalling == false) {
		if (m_iPlayerPlace < g_pGS->m_ciNumLevels) {
			fTextH = 24.0f;
			glcFont::Begin();
				m_Font.Load("FreeSans",static_cast<int>(fTextH));
				m_Font.RenderText(g_pGS->m_asLevelNames[m_iPlayerPlace],m_colLevelText,tbVector2(0,g_pGL->GetAllH()-fTextH),NULL);
			glcFont::End();
		} else { //ending
			fTextH = 54.0f;
			glcFont::Begin();
				m_Font.Load("FreeSans",static_cast<int>(fTextH));
				m_Font.RenderText("THE END" ,tbColor(0.7, 0.6, 0.1, 1.0),tbVector2(0,g_pGL->GetAllH()-fTextH),NULL);
			glcFont::End();
		}
	}
	if (m_ciShowFPS == 1) {
		glcFont::Begin();
			m_Font.Load("FreeSans",16);
			m_Font.RenderText(string("FPS: ")+g_pTimer->GetFPS(),tbColor(1,0,0,1),tbVector2(0,0),NULL);
		glcFont::End();	
	}
	
	glcFlip();
}

void CMainMenu::LoadLine(string psLine) {
	float af[40];
	int ai[20];
	string as[20];
	string sTmp;
	switch (strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10)) {
		case MMLL_BG:
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_ciBGNumPasses = ai[0];
			break;
		case MMLL_CAM1:
			for (int i = 0; i < 4; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			m_fCam1Z = af[0];
			m_fCam1InterpolationFactor = af[1];
			break;
		case MMLL_CAM_MODE:
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_ciCamMode = ai[0];
			break;
		case MMLL_LIGHT:
			for (int i = 0; i < 16; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			m_fLightZ = af[0];
			m_fLightInterpolationFactor = af[1];
			GLC_SAFE_DELETE(m_pLight1);
			m_pLight1 = new glcLight(
				glcSphere(tbVector3(0.0,0.0,m_fLightZ), 0.1), true, true,
			 	tbColor(af[2],af[3],af[4],1.0f), tbColor(af[5],af[6],af[7],1.0f), tbColor(af[8],af[9],af[10],1.0f),
			 	af[11],af[12],af[13],tbVector3(0.0,0.0,-1.0),af[14],af[15]);
			break;
		case MMLL_MUSIC:
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(psLine);
			for (int i = 0; i < 1; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			if (m_Music.IsLoaded() == false) {
				if (glcMusic::IsMusicLoaded(as[0]) == false) glcMusic::LoadMusic(as[0],g_pGS->m_cfStdMusicVolume);
				m_Music.Load(as[0]);
				m_Music.SetVolume(af[0]); 
				m_Music.Play(-1);
			}
			break;
		case MMLL_SHOW_FPS:
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_ciShowFPS = ai[0];
			break;
		default: throw glcException("wrong load line: " + psLine); break;
	}
}

void CMainMenu::CreateMosaics() {
	float fZValue = 0.0f;
	
	tbColor ac[4];
	ac[0] = tbColor(0.5,0.7,0.9,1.0); ac[1] = tbColor(0.9,0.7,0.5,1.0);
	ac[2] = tbColor(0.7,0.7,0.0,1.0); ac[3] = tbColor(0.1,0.7,0.8,1.0);
	
	tbVector3 av[2*(g_pGS->m_ciNumLevels-1)+2];
	int iTmp;
	av[0] = tbVector3((g_pGS->m_ciNumLevels-1+1) * m_vStairWH.x,0.0f,fZValue);
	av[1] = tbVector3((g_pGS->m_ciNumLevels-1+1) * m_vStairWH.x,(g_pGS->m_ciNumLevels-1) * m_vStairWH.y,fZValue);
	for (int i = 2; i < 2*(g_pGS->m_ciNumLevels-1)+2; i++) {
		iTmp = 2*(g_pGS->m_ciNumLevels-1)+2 - i - 1;
		av[i] = tbVector3((1+static_cast<int>((iTmp)/2.0f))*m_vStairWH.x,static_cast<int>((iTmp+1)/2.0f)*m_vStairWH.y,fZValue);
	}
	
	//stairs
	fZValue = 0.000f;
	tbVector3 av2[4];
	av2[0] = av[0];
	m_aiMosaicDisp[0] = glGenLists(1);
	glNewList(m_aiMosaicDisp[0], GL_COMPILE);
		for (int i = 1; i < 2*(g_pGS->m_ciNumLevels-1)+2-2; i+=2) {
			for (int j = 0; j < 3; j++) {
				av2[1+j] = av[i+j];
			}
			poyMosaic(4,av2, ac,12, 0.0,0.8, 0.45,0.55, 0.8,0.4, 8);
		}
	glEndList();
	
	//ground1
	fZValue = 0.000f;
	av2[0] = tbVector3(-m_vGroundWH.x/2.0f,-m_vGroundWH.y,fZValue);
	av2[1] = tbVector3( m_vGroundWH.x/2.0f+m_vPlayerWH.x,-m_vGroundWH.y,fZValue);
	av2[2] = tbVector3( m_vGroundWH.x/2.0f+m_vPlayerWH.x, 0,fZValue);
	av2[3] = tbVector3(-m_vGroundWH.x/2.0f, 0,fZValue);
	m_aiMosaicDisp[1] = glGenLists(1);
	glNewList(m_aiMosaicDisp[1], GL_COMPILE);
		poyMosaic(4,av2, ac,12, 0.0,0.8, 0.45,0.55, 0.8,0.4, 8);
	glEndList();
	
	//ground2
	fZValue = 0.000f;
	ac[0] = tbColor(0.5,0.5,0.2,1.0); ac[1] = tbColor(0.2,0.7,0.5,1.0);
	ac[2] = tbColor(0.7,0.7,0.4,1.0); ac[3] = tbColor(0.6,0.3,0.8,1.0);
	av2[0] = tbVector3(-m_vGround2WH.x/2.0f, -m_vLadderWH.y-m_vGround2WH.y,fZValue);
	av2[1] = tbVector3( m_vGround2WH.x/2.0f, -m_vLadderWH.y-m_vGround2WH.y,fZValue);
	av2[2] = tbVector3( m_vGround2WH.x/2.0f, -m_vLadderWH.y,fZValue);
	av2[3] = tbVector3(-m_vGround2WH.x/2.0f, -m_vLadderWH.y,fZValue);
	m_aiMosaicDisp[2] = glGenLists(1);
	glNewList(m_aiMosaicDisp[2], GL_COMPILE);
		poyMosaic(4,av2, ac,12, 0.0,0.8, 0.45,0.55, 0.8,0.4, 8);
	glEndList();
}

tbVector3 CMainMenu::CalculatePlayerCenter() {
	//center_player
	if (m_iPlayerPlace == -3) {
		return tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/2.0f) + m_vPlayerWH.x/2.0f,m_vPlayerWH.y/2.0f,0);
	} else if (m_iPlayerPlace == -2) {
		return tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vPlayerWH.x/2.0f,-m_vLadderWH.y+m_vPlayerWH.y/2.0f,0);
	} else if (m_iPlayerPlace == -1) {
		return tbVector3(-(m_vStairWH.x*g_pGS->m_ciNumLevels/4.0f)+m_vPlayerWH.x/2.0f,m_vPlayerWH.y/2.0f,0);
	} else if (m_iPlayerPlace == g_pGS->m_ciNumLevels) { 
		return tbVector3(m_vStairWH.x*m_iPlayerPlace+m_vPlayerWH.x/2.0f,m_vPlayerWH.y/2.0f,0);
	} else if (m_iPlayerPlace >= 0) {
		return tbVector3(m_vStairWH.x*m_iPlayerPlace+m_vPlayerWH.x/2.0f,m_vStairWH.y*m_iPlayerPlace+m_vPlayerWH.y/2.0f,0);
	} else {
		throw glcException("unacceptible m_iPlayerPlace");
		return tbVector3(0,0,0);
	}
}

