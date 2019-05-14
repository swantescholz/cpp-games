#ifndef CGAMESTATE_H_
#define CGAMESTATE_H_

#include "../GLC/GLC.h"

#define g_pGS CGameState::Get()

#define ROTATIONPERSECCAM 90.0f
#define TRANSLATIONPERSECCAM 2.0f
#define SCALINGPERSECCAM 1.0f

enum EGameState {
	GS_INTRO = 0,
	GS_MAIN_MENU,
	GS_GAME,
	GS_QUIT,
	GS_NONE
};

void poyMosaic(int piNumVertex, tbVector3 *pav, tbColor *pac, int piNumPasses, float pfProb1, float pfProb2,
				float pfDist1 = 0.25f, float pfDist2 = 0.75f, float pfMinAverageLength = 2.0f, float pfMinLength = 1.0f,
				int piMaxVertex = 8);
void poyDrawTexRect(int piNumPasses, tbVector2 vt1, tbVector3 v1, tbVector2 vt2, tbVector3 v2,
									 tbVector2 vt3, tbVector3 v3, tbVector2 vt4, tbVector3 v4);
string poyGetFirstNumberOfStringAndErase(string &ps); //first XX.XX or XX
string poyGetFirstStringOfStringAndErase(string &ps); //first "XX"
tbVector2 poyAdjustPushingRectX(tbVector2 pvPosPusher, tbVector2 pvWHPusher, tbVector2 pvPosSolid, tbVector2 pvWHSolid); //returns x-adjusted pushing rectangle
tbVector2 poyAdjustPushingRectY(tbVector2 pvPosPusher, tbVector2 pvWHPusher, tbVector2 pvPosSolid, tbVector2 pvWHSolid); //returns y-adjusted pushing rectangle
bool poyCollision2(tbVector2 pvCen1, tbVector2 pvWH1, tbVector2 pvCen2, tbVector2 pvWH2);
int  CompareFunction(const void * a, const void * b); //compares 2 particles for qsort

struct S_GameStateIntern {
	int m_iAnswerPlace;
};
//this class has global access, it saves also the players progress
class CGameState : public TSingleton<CGameState>
{
public:
	CGameState();
	~CGameState();
	
	EGameState m_GS;		//current game state
	int m_ciNumLevels;		//number of all levels
	int m_iLevelToPlay;		//level which should be played
	int m_iAnswerPlace; 	//place with answer block(done levels +1)
	string *m_asLevelNames;	//all level names
	bool m_bRestart;		//restart whole level(includes reading level file!)
	bool m_bRespawn;		//respawn from last checkpoint - or maybe restart the level if no proof was reached (but without reading level file) 
	bool m_bSavingLevel; 	//level should be saved
	const float m_cfOffset; //general offset constant
	const float m_cfLayerGapZ;		//distance for the different layers
	const float m_cfLetterWidth; 	//estimated everyge width of a character, if the height is 1
	const float m_cfStdThoughtFade;	//standard fade speed for thoughts
	float m_fStdThoughtH;			//standard height for thoughts
	bool m_bDeveloper;				//start as developer (cheats)
	int m_iBeginState; 				//only developer (-2:intro,-1:main_menu,0+:level
	int m_ciGameShowFPS;			//in the levels: 1->show FPS, 0->not show FPS
	float m_cfStdMusicVolume;
	float m_cfStdSoundVolume;
	
	//for advise newspaper and CGame only!
	float m_fLightAngle;	//angle-cutoff for the spotlight
};

#endif /*CGAMESTATE_H_*/
