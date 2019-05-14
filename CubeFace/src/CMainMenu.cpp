#include "CMainMenu.h"

CMainMenu::CMainMenu() : m_Music("main_menu", 1.0f), m_Sound("menu", 1.0f)
{
	char cText[400];
	string sText;
	ifstream dat_in("save/help.txt", ios_base::in);
	
	if (!dat_in)
		throw glcException("save/help.txt fehlt");
	
	while (!dat_in.eof()) {
		dat_in.getline (cText,398);
		sText = cText;
		vsHelpText.push_back(sText);
	} vsHelpText.pop_back(); //lesefehler korrigieren
	dat_in.close();
	
	
	m_EMainMenu = MENU_PLAY;
	m_iStage = 0;
	m_iCursor = MENU_PLAY;
	
	VCAM3 = tbVector3(0.0f,1.0f,0.0f);
	VCAM2 = tbVector3(0.0f,0.0f,0.0f);
	VCAM1 = tbVector3(0.0f,0.0f,1.0f);
	
	glClearColor(0,0,0,0);
	
	ttfSans.Load("FreeSans", 24);
	ttfSansLittle.Load("FreeSans", 16);
	
	m_Music.Play(-1);
	m_Sound.Play();
}

CMainMenu::~CMainMenu()
{
}

void CMainMenu::Update() {
	if (glcKeyWasDown(SDLK_ESCAPE))
		m_iStage = 0;
	
	switch (m_iStage) {
		case 0:
			m_iLast = MENU_LAST - 1;
			break;
		case 1:
			switch (m_EMainMenu) {
				case MENU_PLAY:
					m_iLast = g_pGS->m_Progress.iNumBeatenLevels;
					break;
				case MENU_HELP:
					m_iLast = 0;
					break;
				default: break;
			}
			break;
		
		default: break;
	}
	
	if (glcKeyWasDown(SDLK_DOWN))	{m_iCursor++; m_Sound.Play();}
	if (glcKeyWasDown(SDLK_UP))		{m_iCursor--; m_Sound.Play();}
	if (m_iCursor < 0)				m_iCursor = m_iLast;
	if (m_iCursor > m_iLast)		m_iCursor = 0;
	
	if (glcKeyWasDown(SDLK_RETURN) || glcKeyWasDown(SDLK_SPACE)) {
		m_Sound.Play();
		if (m_iStage == 0) {
			m_iStage = 1;
			m_EMainMenu = m_iCursor;
			
			switch (m_EMainMenu) {
				case MENU_QUIT:
					g_pGS->m_GameState = GS_QUIT;
					break;
				default: break;
			}
		} else {
			switch (m_EMainMenu) {
				case MENU_PLAY:
					g_pGS->m_iLevelToPlay = m_iCursor;
					g_pGS->m_GameState = GS_GAME;
					break;
				case MENU_HELP:
					m_iStage = 0;
					break;
				default: break;
			}
		}
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
	
	
	
	tbVector2 vTmp, vPos(10.0f, ALLH - 100.0f);
	int iTmp = 0;
	char cTmp[500];
	
	glcFont::Begin();
	switch (m_iStage) {
		case 0:
			
			vPos = tbVector2(10.0f, ALLH - 100.0f);
			ttfSans.RenderText("Play"    		, (m_iCursor == iTmp++) ? ncol::red : ncol::green, vPos, &vTmp); vPos.y -= vTmp.y;
			ttfSans.RenderText("Help"    		, (m_iCursor == iTmp++) ? ncol::red : ncol::green, vPos, &vTmp); vPos.y -= vTmp.y;
			ttfSans.RenderText("Quit"    		, (m_iCursor == iTmp++) ? ncol::red : ncol::green, vPos, &vTmp); vPos.y -= vTmp.y;
			
			break;
		case 1:
			switch (m_EMainMenu) {
				case MENU_PLAY:
					vPos = tbVector2(100.0f, ALLH - 100.0f);
					for (int i = 0; i < NUM_LEVELS; i++) {
						sprintf(cTmp, "Level %i", i + 1);
						if (i <= g_pGS->m_Progress.iNumBeatenLevels) {
							ttfSansLittle.RenderText(cTmp    , (m_iCursor == iTmp++) ? ncol::red : ncol::green, vPos, &vTmp); vPos.y -= vTmp.y;
						} else {
							ttfSansLittle.RenderText(cTmp    , tbColor(0.2f), vPos, &vTmp); vPos.y -= vTmp.y;
						}
					}
					break;
				case MENU_HELP:
					vPos = tbVector2(100.0f, ALLH - 100.0f);
					ttfSans.RenderText("Help", ncol::red, vPos, &vTmp); vPos.y -= vTmp.y * 2.0f;
					for (vsit = vsHelpText.begin(); vsit != vsHelpText.end(); vsit++) {
						ttfSans.RenderText((*vsit), ncol::yellow, vPos, &vTmp); vPos.y -= vTmp.y;
					}
					vPos = tbVector2(100.0f, 100.0f);
					ttfSansLittle.RenderText("Back", ncol::red, vPos, &vTmp);
					break;
					
				default: break;
			}
			break;
			
		default: break;
	}
	glcFont::End();
	
	glcFlip();
}
