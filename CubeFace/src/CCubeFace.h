#ifndef CCUBEFACE_H_
#define CCUBEFACE_H_

#include "../GLC/GLC.h"
#include "CIntro.h"
#include "CMainMenu.h"
#include "CGame.h"
#include "CGS.h"

#define g_pCubeFace CCubeFace::Get()


class CCubeFace : public TSingleton<CCubeFace>
{
public:
	CCubeFace();
	~CCubeFace();
	
	void Run();
	
private:
	void Update();
	void Render();
	
	void SwitchGameState();
	void Quit();
	
	EGameState	m_LastGameState;
	bool 		m_bQuit;
	
	CIntro		*m_pIntro;
	CMainMenu	*m_pMainMenu;
	CGame		*m_pGame;
};

#endif /*CCUBEFACE_H_*/
