#ifndef CGS_H_
#define CGS_H_

#include "../GLC/GLC.h"

#define g_pGS CGS::Get()
#define NUM_LEVELS 20
#define CUBE_SIZE 100.0f	//von -50,-50,-50 bis 50,50,50 NOTE: do not change

enum ECollisionState {
	CS_NO = 0,
	CS_SOLID,
	CS_GOAL,
	CS_KILL,
	
	CS_LAST
};

enum EGameState {
	GS_INTRO = 0,
	GS_MAIN_MENU,
	GS_GAME,
	GS_QUIT
};

enum ECubeFace {
	CF_FRONT = 0,
	CF_RIGHT,
	CF_BACK,
	CF_LEFT,
	CF_TOP,
	CF_BOTTOM
};

//fuer rand normal (up: y+, down: y-, ...), fuer top (up: z-, down z+, left: x-, right: x+), fuer bottom(up: z+, down: z+, left: x-, right: x+)
enum EGravitationDirection {
	GD_LEFT = 0,
	GD_RIGHT,
	GD_UP,
	GD_DOWN
};

struct S_Progress {
	int iNumBeatenLevels;
};


class CGS : public TSingleton<CGS>
{
public:
	CGS();
	~CGS();
	
	EGameState m_GameState;
	S_Progress m_Progress;
	int m_iLevelToPlay;
	
	EGravitationDirection m_GravitationDir;
	ECubeFace m_PlayerFace;
};

#endif /*CGS_H_*/
