#ifndef CGAME_H_
#define CGAME_H_

#include "../GLC/GLC.h"
#include "CGS.h"
#include "CPlayer.h"
#include "CLevel.h"

class CGame
{
public:
	CGame();
	~CGame();
	
	void Update();
	void Render();
	
	
private:

	glcMusic m_Music;
	glcSound m_sndReset;
	CPlayer *m_pPlayer;
};

#endif /*CGAME_H_*/
