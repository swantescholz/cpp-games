#include "CGame.h"

CGame::CGame() : m_Music("game", 1.0f), m_sndReset("reset", 1.0f)
{
	g_Logfile << "GameState: Game";
	m_Music.Play(-1);
	
	CLevel::Get();
	char cTmp[500];
	sprintf (cTmp, "save/levels/level%i.lvl", g_pGS->m_iLevelToPlay + 1);
	g_pLevel->Load(cTmp);	//level aus datei laden
	m_pPlayer	= new CPlayer;
	
	VCAM3 = tbVector3(0.0f,1.0f,0.0f);
	VCAM2 = tbVector3(0.0f,0.0f,0.0f);
	VCAM1 = tbVector3(0.0f,0.0f,20.0f);
	
	glClearColor(0.5,0.7,0.9,0);
	
	glFogi (GL_FOG_MODE, GL_LINEAR);
	glFogf (GL_FOG_START, ((CUBE_SIZE / 2.0f) * m_pPlayer->GetCamZoom() ) - (CUBE_SIZE / 2.0f) );
	glFogf (GL_FOG_END  , ((CUBE_SIZE / 2.0f) * m_pPlayer->GetCamZoom() ) + ( (CUBE_SIZE / 2.0f) * 1.5f ) ); //wert btw: 1-2.5 (std: 1.5)
}

CGame::~CGame()
{
	GLC_SAFE_DELETE(m_pPlayer);
	CLevel::Del();
	glFogf (GL_FOG_DENSITY, 0.001);
	glFogi (GL_FOG_MODE, GL_EXP);
}

void CGame::Update() {
	if (glcKeyWasDown(SDLK_ESCAPE)) {
		g_pGS->m_GameState = GS_MAIN_MENU;
		g_pGS->m_GravitationDir = GD_DOWN;
		g_pGS->m_iLevelToPlay = 0;
	} else if (glcKeyWasDown(SDLK_BACKSPACE)) {		//vollstaendiger player-neustart
		m_sndReset.Play();
		g_pGS->m_GravitationDir = GD_DOWN;
		GLC_SAFE_DELETE(m_pPlayer);
		CLevel::Del();
		CLevel::Get();
		char cTmp[500];
		sprintf (cTmp, "save/levels/level%i.lvl", g_pGS->m_iLevelToPlay + 1);
		g_pLevel->Load(cTmp);	//level aus datei laden
		m_pPlayer	= new CPlayer;
	} else {
		if (!glcKeyIsDown(SDLK_LCTRL)) {
			m_pPlayer->Update();
			g_pLevel->Update();
		}
	}
}

void CGame::Render() {
	if (!glcKeyIsDown(SDLK_LCTRL)) {
		// Clear the color and depth buffers.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
		// We don't want to modify the projection matrix.
		glMatrixMode( GL_MODELVIEW );
	    glLoadIdentity( );
		
		//cameramatrix festsetzen (zuallererst!)
		g_pGL->LookAt();
		
		//todo
		g_pLevel->Render();
		m_pPlayer->Render();
		
		
		glcFlip();
	}
}