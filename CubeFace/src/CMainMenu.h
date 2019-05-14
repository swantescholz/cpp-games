#ifndef CMAINMENU_H_
#define CMAINMENU_H_

#include "../GLC/GLC.h"
#include "CGS.h"

class CMainMenu
{
public:
	CMainMenu();
	~CMainMenu();
	
	void Update();
	void Render();
	
	
private:
	glcMusic m_Music;
	glcSound m_Sound;
	
	enum {MENU_PLAY, MENU_HELP, MENU_QUIT, MENU_LAST};
	int m_EMainMenu;
	int m_iStage;
	int m_iCursor;
	int m_iLast;
	
	glcFont ttfSans;
	glcFont ttfSansLittle;
	
	vector<string> vsHelpText;	//zeile fuer zeile
	vector<string>::iterator vsit;
};

#endif /*CMAINMENU_H_*/
