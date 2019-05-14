#include "CCF_LevelEditor.h"

CCF_LevelEditor::CCF_LevelEditor()
{	
	//options einlesen
	char cText[500];
	string sPath = g_pGS->m_sArg0;
	sPath.erase(sPath.rfind("/") + 1);
	ifstream dat_in( (sPath + "save/options.ini").data() , ios_base::in);
	
	if (!dat_in)
		throw glcException("save/options.ini fehlt");
	
	dat_in.getline (cText,498);
	int iW = strtoul(cText,NULL,10);
	dat_in.getline (cText,498);
	int iH = strtoul(cText,NULL,10);
	dat_in.getline (cText,498);
	int iFull = strtoul(cText,NULL,10);
	dat_in.close();
	
	//glcInit()
	if (iFull == 0)		glcInit(iW, iH, GLC_FOG | GLC_TEXTURE2D | GLC_BLEND, SDL_OPENGL					, g_pGS->m_sArg0 + " | 2009 by S. Scholz", "CF_LevelEditor", sPath+"Logfile.html");
	else				glcInit(iW, iH, GLC_FOG | GLC_TEXTURE2D | GLC_BLEND, SDL_OPENGL | SDL_FULLSCREEN, g_pGS->m_sArg0 + " | 2009 by S. Scholz", "CF_LevelEditor", sPath+"Logfile.html");
	
	m_bQuit 		= false;
	
	//load res
	
	//fonts
	glcFont::Init(sPath + "res/fonts/", ".ttf");
	glcFont::LoadTTF("FreeSans", 8, 80);
	
	//textures
	glcTexture::Init(sPath + "res/textures/",".png");
	glcTexture::LoadTexture("player");
	glcTexture::LoadTexture("raster");
	//blocks
	glcTexture::LoadTexture("solid");
	glcTexture::LoadTexture("kill");
	glcTexture::LoadTexture("goal");
}

CCF_LevelEditor::~CCF_LevelEditor()
{
	glcQuit();
}

void CCF_LevelEditor::Run() {
	while (!m_bQuit) {
		Update();
		Render();
	}
}

void CCF_LevelEditor::Load(string sPath) {
	//level (falls vorhanden) einlesen
	g_pLevel->LoadLevel(sPath);
}

void CCF_LevelEditor::Update() {
	glcUpdate();
	
	if (!glcKeyIsDown(SDLK_LCTRL)) {
		g_pGame->Update();
	}
	
	if ( (glcKeyIsDown(SDLK_F4) && glcKeyIsDown(SDLK_LALT)) || g_pGL->MouseQuit() || glcKeyWasDown(SDLK_ESCAPE) )	//1. F4 dann LALT fuer abbruch
		Quit();
}

void CCF_LevelEditor::Render() {
	if (!glcKeyIsDown(SDLK_LCTRL)) {
		g_pGame->Render();
	}
}

void CCF_LevelEditor::Quit() {
	m_bQuit = true;
}

