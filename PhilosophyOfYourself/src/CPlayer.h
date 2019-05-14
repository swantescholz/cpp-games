#ifndef CPLAYER_H_
#define CPLAYER_H_

#include "../GLC/GLC.h"
#include "CGameState.h"
#include "CObject.h"
#include "CAxiom.h"
#include "CSolidArgument.h"
#include "CUnknown.h"
#include "CAnswer.h"
#include "CProof.h"
#include "CIgnorance.h"
#include "CLie.h"
#include "CRashArgument.h"

//a class for the moving player(he is also an object)
class CPlayer : public CObject
{
public:
	CPlayer(tbVector2 pvStartPos, int piNumMaxJumps, float pfJumpSpeed, tbVector2 pvAcc, tbVector2 pvMaxSpeed);
	~CPlayer();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	tbVector2 GetCenter() {return m_vPos;}
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects) {return;}  //player itsself has no thought 
	
	//get and set methods
	void SetPos(tbVector2 pv) {m_vPos = pv;}
	void SetVel(tbVector2 pv) {m_vVel = pv;}
	tbVector2 GetPos() 		{return m_vPos;}
	tbVector2 GetLastPos() 	{return m_vLastPos;}
	tbVector2 GetVel() 		{return m_vVel;}
	tbVector2 GetLastVel() 	{return m_vLastVel;}
	tbVector2 GetWH() 		{return m_vWH;}
	
	void DoKill(list<S_LevelIntern> *palObjects);	//the player gets killed
private:
	void MoveX(list<S_LevelIntern> *palObjects);
	void MoveY(list<S_LevelIntern> *palObjects);
	bool CanUnshrink(list<S_LevelIntern> *palObjects);		//can the player get back big
	
	glcTexture 	 m_TexPlayer;		//texture of player
	const string m_sSndJump;		//string of sound for jumping
	const string m_sSndDie;			//string of sound for dying
	tbVector2 m_vPos, m_vLastPos;	//position
	tbVector2 m_vWH;				//width, heigth
	tbVector2 m_vVel, m_vLastVel;	//velocity
	tbVector2 m_vAcc;				//acceleration
	float 			m_fJumpSpeed;		//the velocity increased when jumping
	int 			m_iNumJumps;		//number of jumps in current flight
	const int 		m_ciNumMaxJumps;	//max. number of jump in one flight
	const tbVector2 m_cvMaxSpeed;		//max. speeds in x/y
	//shrinking
	bool m_bIsShrunken;
	bool m_bShouldUnshrink;
	float m_fShrinkFactor;
	
};

#endif /*CPLAYER_H_*/

