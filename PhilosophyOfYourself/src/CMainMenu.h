#ifndef CMAINMENU_H_
#define CMAINMENU_H_

#include "../GLC/GLC.h"
#include "CGameAbstract.h"
#include "CGameState.h"

//the load types for the main_menu level file
enum EMainMenuLevelLine {
	MMLL_BG 		= 0,
	MMLL_CAM1		= 1,
	MMLL_CAM_MODE	= 2,
	MMLL_LIGHT		= 3,
	MMLL_MUSIC		= 4,
	MMLL_SHOW_FPS	= 5
};

//the second game-state, a "in-game" menu with level-choosing
class CMainMenu : public CGameAbstract
{
public:
	CMainMenu();
	~CMainMenu();
	
	void Update();
	void Render();
	
private:
	void CreateMosaics();					//it creates all the mosaics
	tbVector3 CalculatePlayerCenter();		//the players center is returned
	void LoadLine (string psLine);			//it loads a single load line
	
	glcFont m_Font;
	glcSound   m_SndStep;
	glcTexture m_TexWood;
	glcTexture m_TexAnswer;
	glcTexture m_TexPlayer;
	glcTexture m_TexArrow;
	glcTexture m_TexLadder;
	glcTexture m_TexBg;
	glcLight *m_pLight1;
	glcMusic m_Music;
	tbColor m_colStair[3];
	tbColor m_colLeaveArr[3];
	tbColor m_colCreditsArr[3];
	tbColor m_colCredits[3];
	tbColor m_colLevelText;
	tbVector2 m_vStairWH;
	tbVector2 m_vPlayerWH;
	tbVector2 m_vGroundWH;
	tbVector2 m_vGround2WH;
	tbVector2 m_vLadderWH;
	tbVector2 m_vAnswerWH;
	tbVector2 m_vArrowWH;
	tbVector2 m_vBgWH;
	tbVector3 m_vPlayerCenter;
	float m_fDownSpeed;
	int m_aiMosaicDisp[3];	//0:stairs,1:ground1,2:ground2
	int m_iPlayerPlace;
	bool m_bFalling;
	
	//from here on manipulated by the text file
	float m_fLightZ, m_fLightInterpolationFactor;
	float m_fCam1Z,  m_fCam1InterpolationFactor;
	int m_ciBGNumPasses;
	int m_ciShowFPS;
	int m_ciCamMode;
	
	static int c_iNumInstances; //number of me
};

#endif /*CMAINMENU_H_*/
