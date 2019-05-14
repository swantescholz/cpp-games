#ifndef CGAME_H_
#define CGAME_H_

#include "../GLC/GLC.h"
#include "CGameAbstract.h"
#include "CGameState.h"
#include "CLevel.h"

//the third game-state, it merely loads the textures, the level-class does the rest
class CGame : public CGameAbstract
{
public:
	CGame();
	~CGame();
	
	void Update();
	void Render();
	
private:
	CLevel *m_pLevel;		//level pointer
	CLevel *m_pSavedLevel;  //saved level for proof-checkpoint
	glcFont m_Font;			//font for level name
	tbColor m_colLevelText; //color for level name
	
	const string m_sSndKeyRespawn; //string for sound when player restarts with key to last proof
	const string m_sSndKeyRestart; //string for sound when player respawns level
};

#endif /*CGAME_H_*/

