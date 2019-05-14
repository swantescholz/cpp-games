#include "GLC.h"

void glcInit(int width, int hight, flag8 GLflags, int SDLflags, string sWindow, string sTab, string sLogfile) {
	g_pLogfile->CreateLogfile(sLogfile.data());
	g_pGL->Init(width, hight, GLflags, SDLflags, sWindow, sTab);
	glcSphere::Init();
	
	g_pLogfile->Textout(GREEN, "glcInit");
}

void glcQuit() {
	glcMusic::Quit();
	glcTexture::Quit();
	glcSound::Quit();
	glcModel::Quit();
	glcFont::Quit();
	g_pGL->Quit();
	g_pGL->Del();
	
	g_pLogfile->Textout(GREEN, "glcQuit");
	g_pLogfile->Del();
}


void glcUpdate(int iTimer) {
	g_pTimer->Update(iTimer);
	g_pGL->Update();
}

void glcFlip() {g_pGL->Flip();}
void glcDelay(float fSec) {SDL_Delay(static_cast<int> (fSec*1000));}
void glcDelay(int iMil)	  {SDL_Delay(iMil);}


bool glcKeyIsDown	(int iID) {return g_pGL->KeyIsDown	(iID);}
bool glcKeyWasDown	(int iID) {return g_pGL->KeyWasDown	(iID);}
bool glcKeyWasUp	(int iID) {return g_pGL->KeyWasUp	(iID);}





