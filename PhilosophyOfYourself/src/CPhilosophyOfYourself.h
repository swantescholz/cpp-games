#ifndef CPHILOSOPHYOFYOURSELF_H_
#define CPHILOSOPHYOFYOURSELF_H_

#include "../GLC/GLC.h"
#include "CGameAbstract.h"
#include "CGame.h"
#include "CIntro.h"
#include "CMainMenu.h"
#include "CGameState.h"

//the class that controlls everything
class CPhilosophyOfYourself : public CGameAbstract
{
public:
	CPhilosophyOfYourself();
	~CPhilosophyOfYourself();
	
	void Run();
	void Update();
	void Render();
	
private:
	void SwitchGS();
	
	CGameAbstract *m_pUse;	//intro, mainmenu, or game
	EGameState m_LastGS;
	bool m_bQuit;
	
};

#endif /*CPHILOSOPHYOFYOURSELF_H_*/

