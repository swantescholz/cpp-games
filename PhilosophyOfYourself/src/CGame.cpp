#include "CGame.h"

CGame::CGame() : m_sSndKeyRespawn("key_respawn"), m_sSndKeyRestart("key_restart")
{
	m_colLevelText = tbColor(0.9,0.7,0.5,1.0);
	
	m_pLevel 		= new CLevel(string("res/lvl/level") + g_pGS->m_iLevelToPlay + ".lvl");
	m_pSavedLevel	= new CLevel((*m_pLevel));
}

CGame::~CGame()
{
	GLC_SAFE_DELETE(m_pSavedLevel);
	GLC_SAFE_DELETE(m_pLevel);
}


void CGame::Update() {
	if (g_pGL->KeyWasDown(SDLK_ESCAPE)) {
		g_pGS->m_GS = GS_MAIN_MENU;
	}
	if (g_pGL->KeyWasDown(SDLK_BACKSPACE) || g_pGS->m_bRestart == true) {
		if (g_pGS->m_bRestart == false) { // key was down, so play sound
			glcSound(m_sSndKeyRestart).Play();
		}
		g_pGS->m_bRestart = false;
		GLC_SAFE_DELETE(m_pLevel);
		m_pLevel = new CLevel(string("res/lvl/level") + g_pGS->m_iLevelToPlay + ".lvl");
		GLC_SAFE_DELETE(m_pSavedLevel);
		m_pSavedLevel = new CLevel((*m_pLevel)); 
	}
	if (g_pGL->KeyWasDown(SDLK_DELETE)    || g_pGS->m_bRespawn == true) {
		if (g_pGS->m_bRespawn == false) { // key was down, so play sound
			glcSound(m_sSndKeyRespawn).Play();
		}
		g_pGS->m_bRespawn = false;
		GLC_SAFE_DELETE(m_pLevel);
		g_pGS->m_fLightAngle = m_pSavedLevel->GetLightAngle(); //reset to old light angle
		m_pLevel = new CLevel((*m_pSavedLevel));
	}
	if (g_pGL->MouseQuit()) {
		g_pGS->m_GS = GS_QUIT;
	}
	
	//actually updating the level
	m_pLevel->Update();
	
	//saving at checkpoint?
	if (g_pGS->m_bSavingLevel == true) {
		g_pGS->m_bSavingLevel = false;
		GLC_SAFE_DELETE(m_pSavedLevel);
		m_pSavedLevel = new CLevel((*m_pLevel)); 
	}
	
	
	//resetting events
	g_pGL->ResetWasEvents();
}

void CGame::Render() {
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// We don't want to modify the projection matrix.
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
	
	//cameramatrix festsetzen (zuallererst!)
	g_pGL->LookAt();
	
	m_pLevel->Render();
	
	float fTextH = 24.0f;
	glcFont::Begin();
		m_Font.Load("FreeSans",static_cast<int>(fTextH));
		m_Font.RenderText(g_pGS->m_asLevelNames[g_pGS->m_iLevelToPlay],m_colLevelText,tbVector2(0,g_pGL->GetAllH()-fTextH),NULL);
		if (g_pGS->m_ciGameShowFPS == 1) { //show FPS?
			m_Font.SetSize(16);
			m_Font.RenderText(string("FPS: ") + g_pTimer->GetFPS(),tbColor(1,0,0,1),tbVector2(0,0),NULL);
		}
	glcFont::End();
	
	glcFlip();
}

