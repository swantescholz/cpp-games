#ifndef CPLAYER_H_
#define CPLAYER_H_

#include "../GLC/GLC.h"
#include "CGS.h"
#include "CLevel.h"

class CPlayer
{
public:
	CPlayer();
	~CPlayer();
	
	void Update();
	void Render();
	
	float GetCamZoom() {return m_fCamZoom;}
private:
	enum ECam2Style {C2_ZERO = 0, C2_PLAYER};
	
	void SwitchFace();	//test
	void SetCamera();	//setzt die camera 1-3
	void Reset();						//player has pressed r
	void DoKilled();					//player ist gestorben
	void DoFinished();					//player ist beim goal
	
	glcTexture m_Tex;		//player-textur
	
	//positionen (von oben links)
	tbVector2 m_vStartPos;	//startposition
	tbVector2 m_vPos;		//2d-pos (oben links) auf face bezogen (von (gravitationstechnisch) unten links) (untere linke ecke der rechtecks der grafik)
	tbVector2 m_vWH;		//width und heigth
	tbVector3 m_vPos3;		//3d-position (mitte)
	ECubeFace m_Face;		//seite des cubes
	ECubeFace m_StartFace;	//anfangsseite
	
	int 		m_iMaxJumps;				//maximale spruenge (std: 2)
	int			m_iNumJumps;				//anzahl aktueller jumps
	float		m_fXSpeed;					//akt. x - speed
	float		m_fXSpeedAcc;				//x - speed beschleunigung
	float		m_fXSpeedDec;				//x-speed abschaechung
	float		m_fXMaxSpeed;				//maximale gehfgeschwindigkeit
	float 		m_fYSpeed;					//akt. y - speed
	float		m_fYStartSpeed;				//anfangs y speed bei jump
	float		m_fYSpeedDecreasePerSec;	//verringerung von y speed pro s
	float 		m_fYMaxSpeed;				//maximal fallgeschwindigkeit
	
	bool m_bInAir;							//in der luft?
	float m_fCamZoom;						//naehe zu player (std:3.5);
	ECam2Style m_Cam2Style;					//cam style(0->std, 1->nah)
	
	glcSound m_sndJump;
	glcSound m_sndKill;
	glcSound m_sndFinish;
	glcSound m_sndReset;
};

#endif /*CPLAYER_H_*/
