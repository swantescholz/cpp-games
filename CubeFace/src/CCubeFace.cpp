#include "CCubeFace.h"

CCubeFace::CCubeFace()
{
	//options einlesen
	char cText[500];
	ifstream dat_in("save/options.ini", ios_base::in);
	
	if (!dat_in)
		throw glcException("save/options.ini fehlt");
	
	dat_in.getline (cText,498);
	int iW = strtoul(cText,NULL,10);
	dat_in.getline (cText,498);
	int iH = strtoul(cText,NULL,10);
	dat_in.getline (cText,498);
	int iFull = strtoul(cText,NULL,10);
	dat_in.getline (cText,498);
	double dMusicFactor = strtod(cText, NULL);
	dat_in.getline (cText,498);
	double dSoundFactor = strtod(cText, NULL);
	dat_in.close();
	
	if (iFull == 0)		glcInit(iW, iH, GLC_FOG | GLC_TEXTURE2D | GLC_BLEND, SDL_OPENGL, "CubeFace | 2009 by S. Scholz", "CubeFace");
	else				glcInit(iW, iH, GLC_FOG | GLC_TEXTURE2D | GLC_BLEND, SDL_OPENGL | SDL_FULLSCREEN, "CubeFace | 2009 by S. Scholz", "CubeFace");
	glFogf (GL_FOG_DENSITY, 0.001);
	
	
	m_LastGameState = GS_INTRO;
	m_bQuit 		= false;
	
	//load res
	
	//fonts
	glcFont::Init("res/fonts/", ".ttf");
	glcFont::LoadTTF("FreeSans", 8, 80);
	
	//music
	glcMusic::Init("res/music/", ".ogg");
	glcMusic::LoadMusic("intro"		, dMusicFactor);
	glcMusic::LoadMusic("main_menu"	, dMusicFactor);
	glcMusic::LoadMusic("game"		, dMusicFactor);
	
	//sounds
	glcSound::Init(20, "res/sounds/", ".ogg");
	glcSound::LoadSound("menu"		, dSoundFactor);
	glcSound::LoadSound("kill"		, dSoundFactor);
	glcSound::LoadSound("jump"		, dSoundFactor);
	glcSound::LoadSound("finish"	, dSoundFactor);
	glcSound::LoadSound("reset"		, dSoundFactor);
	
	//textures
	glcTexture::Init("res/textures/",".png");
	glcTexture::LoadTexture("wood");
	glcTexture::LoadTexture("player");
	//blocks
	glcTexture::LoadTexture("solid");
	glcTexture::LoadTexture("kill");
	glcTexture::LoadTexture("goal");
	
	//pointers
	m_pIntro 		= new CIntro;
	m_pMainMenu 	= NULL;
	m_pGame 		= NULL;
}

CCubeFace::~CCubeFace()
{
	GLC_SAFE_DELETE(m_pIntro);
	GLC_SAFE_DELETE(m_pMainMenu);
	GLC_SAFE_DELETE(m_pGame);
	CGS::Del();
	glcQuit();
}

void CCubeFace::Quit() {
	m_bQuit = true;
}

void CCubeFace::Run() {
	while (!m_bQuit) {
		Update();
		Render();
	}
}

void CCubeFace::Update() {
	glcUpdate();
	
	switch (g_pGS->m_GameState) {
		case GS_INTRO:		//titelbildschirm
			m_pIntro->Update(); 
			break;
		case GS_MAIN_MENU:	//hauptmenue
			m_pMainMenu->Update();
			break;
		case GS_GAME:		//spiel
			m_pGame->Update();
			break;
		case GS_QUIT:
			Quit();
			break;
		default: break;
	}
	
	
	if (g_pGS->m_GameState != m_LastGameState) {
		SwitchGameState();
		m_LastGameState = g_pGS->m_GameState;
	}
	
	if ( (glcKeyIsDown(SDLK_F4) && glcKeyIsDown(SDLK_LALT)) || g_pGL->MouseQuit() )	//1. F4 dann LALT fuer abbruch
		Quit();
}

void CCubeFace::Render() {
	switch (g_pGS->m_GameState) {
		case GS_INTRO:		//titelbildschirm
			m_pIntro->Render();
			break;
		case GS_MAIN_MENU:	//hauptmenue
			m_pMainMenu->Render();
			break;
		case GS_GAME:		//spiel
			m_pGame->Render();
			break;
		default: break;
	}
}

void CCubeFace::SwitchGameState() {
	glcMusic::Halt();
	glcSound::HaltAll();
	switch (m_LastGameState) {
		case GS_INTRO:		//titelbildschirm
			GLC_SAFE_DELETE(m_pIntro); 
			break;
		case GS_MAIN_MENU:	//hauptmenue
			GLC_SAFE_DELETE(m_pMainMenu);
			break;
		case GS_GAME:		//spiel
			GLC_SAFE_DELETE(m_pGame);
			break;
		default: break;
	}
	switch (g_pGS->m_GameState) {
		case GS_INTRO:		//titelbildschirm
			m_pIntro = new CIntro;
			break;
		case GS_MAIN_MENU:	//hauptmenue
			m_pMainMenu = new CMainMenu;
			break;
		case GS_GAME:		//spiel
			m_pGame = new CGame;
			break;
		default: break;
	}
}

