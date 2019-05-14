#ifndef CADVICE_H_
#define CADVICE_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CSolidArgument.h"
#include "CLie.h"
#include "CPlayer.h"
#include "CRashArgument.h"
#include "CThought.h"

enum EAdviceType {
	AT_NEWSPAPER 	= 0,
	AT_INTERNET		= 1,
	AT_BOOK			= 2
};

//these classes must be known:
class CPlayer;

//an advice object-type, it holds advise with items(newspaper, internet, book)
//it eventually increases light radius or points at answer
class CAdvice : public CObject
{
public:
	CAdvice(EAdviceType pAT, tbVector2 pvStartPos, string sThought);
	~CAdvice();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects);
	tbVector2 GetCenter() {return m_vPosGround;} 					//just from the solid part
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	bool CanBePassedThrough(tbVector2 pvCenter, tbVector2 pvWH);
	
	bool IsUsed() {return m_bUsed;}	//advise used?
private:
	void UseAdvice(list<S_LevelIntern> *palObjects, CPlayer *pPlayer);
	void IsUsed(bool pb, list<S_LevelIntern> *palObjects); //sets m_bSavedHere to pb and changes other proofs
	void SetArrowDir(list<S_LevelIntern> *palObjects);
	
	glcTexture m_TexGround; //solid block
	glcTexture m_TexTop;    //unsolid grafic item holder
	glcTexture m_TexItem;   //unsolid grafic item
	tbVector2 m_vPosGround;	//position of ground
	tbVector2 m_vPosTop;	//position of top
	tbVector2 m_vPosItem;	//position of item at top
	tbVector2 m_vWHGround;  //solid
	tbVector2 m_vWHTop;     //unsolid
	tbVector2 m_vWHItem;    //unsolid
	tbVector2 m_vArrowDir;	//arrow direction for newspaper item used
	
	bool 		m_bUsed;	//was advise used here?
	EAdviceType m_AT;		//my advise type
	
	const string m_sSndTaken; //string of sound for taking the advice
	
	static const tbColor c_cThoughtStartCol;				//start color of thought
	static const float c_cfItemInternetLightRadiusFactor;	//when internet item is used, light radius will be multiplied by this
};

#endif /*CADVICE_H_*/
