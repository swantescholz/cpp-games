#include "GLC.h"

void glcInit(int width, int hight, flag8 GLflags, int SDLflags, string sWindow, string sTab) {
	g_pLogfile->CreateLogfile("Logfile.html");
	g_pGL->Init(width, hight, GLflags, SDLflags, sWindow, sTab);
	glcSphere::Init();
	
	g_pLogfile->Textout(GREEN, "glcInit succeded");
}

void glcQuit() {
	glcMusic::Quit();
	glcTexture::Quit();
	glcSound::Quit();
	glcModel::Quit();
	glcFont::Quit();
	g_pGL->Quit();
	g_pGL->Del();
	
	g_pLogfile->Textout(GREEN, "glcQuit succeded");
	g_pLogfile->Del();
}


void glcUpdate(float fTimer) {
	g_pTimer->Update(fTimer);
	g_pGL->Update();
}

void glcFlip() {g_pGL->Flip();}
void glcDelay(float fSec) {SDL_Delay(static_cast<int> (fSec*1000));}
void glcDelay(int iMil)	  {SDL_Delay(iMil);}


bool glcKeyIsDown	(int iID) {return g_pGL->KeyIsDown	(iID);}
bool glcKeyWasDown	(int iID) {return g_pGL->KeyWasDown	(iID);}
bool glcKeyWasUp	(int iID) {return g_pGL->KeyWasUp	(iID);}





