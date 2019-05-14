#ifndef CPROOF_H_
#define CPROOF_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CSolidArgument.h"
#include "CLie.h"
#include "CPlayer.h"
#include "CRashArgument.h"

//these classes must be known:
class CPlayer;

//a object type class for checkpoints
//with a proof you can decide whether to use it or to ignore it
//kinda tree
class CProof : public CObject
{
public:
	CProof(tbVector2 pvStartPos, string psThought);
	~CProof();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects);
	tbVector2 GetCenter() {return m_vPosGround;} 					//just from the solid part
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	bool CanBePassedThrough(tbVector2 pvCenter, tbVector2 pvWH);
	
	bool IsSaved() {return m_bSavedHere;}
private:
	void Save(list<S_LevelIntern> *palObjects, CPlayer *pPlayer);
	void SetSaved(bool pb, list<S_LevelIntern> *palObjects); //sets m_bSavedHere to pb and changes other proofs
	
	glcTexture m_TexGround; //solid block
	glcTexture m_TexTop;    //unsolid grafic
	tbVector2 m_vPosGround;	//position of ground
	tbVector2 m_vPosTop;	//position of top
	tbVector2 m_vWHGround;  //solid
	tbVector2 m_vWHTop;     //unsolid
	
	bool m_bSavedHere;		//was here saved?
	
	const string m_sSndProven; //string for sound when saved
};

#endif /*CPROOF_H_*/
