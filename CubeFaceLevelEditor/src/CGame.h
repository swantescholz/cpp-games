#ifndef CGAME_H_
#define CGAME_H_

#include "GLC.h"
#include "CGS.h"
#include "CLevel.h"

#define g_pGame CGame::Get()

class CGame : public TSingleton<CGame>
{
public:
	CGame();
	~CGame();
	
	void Update();
	void Render();
	
	
private:
	
	const float m_fCamMoveSpeed;	//w/h
};

#endif /*CGAME_H_*/
