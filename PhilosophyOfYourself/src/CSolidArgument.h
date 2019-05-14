#ifndef CSOLIDARGUMENT_H_
#define CSOLIDARGUMENT_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CPlayer.h"
#include "CUnknown.h"
#include "CAnswer.h"
#include "CFalseBelief.h"
#include "CProof.h"
#include "CIgnorance.h"
#include "CLie.h"
#include "CRashArgument.h"
#include "CThought.h"

//an object type for a pushable solid_argument crate
class CSolidArgument : public CObject
{
public:
	CSolidArgument(tbVector2 pvStartPos, string psThought);
	~CSolidArgument();
	
	void 		Update(list<S_LevelIntern> *palObjects);
	void 		Render();
	bool 		PushX(list<S_LevelIntern> *palObjects, CObject *pObj);		//pObj is trying to push this object in x-dir
	bool 		PushY(list<S_LevelIntern> *palObjects, CObject *pObj);		//pObj is trying to push this object in y-dir
	void 		AddThought(list<S_LevelIntern> *palObjects);
	tbVector2 	GetCenter() {return m_vPos;}
	bool 		Collision(tbVector2 pvCenter, tbVector2 pvWH);
	
	//the usual get and set methods
	void SetPos(tbVector2 pv) {m_vPos = pv;}
	void SetVel(tbVector2 pv) {m_vVel = pv;}
	tbVector2 GetPos() 		{return m_vPos;}
	tbVector2 GetLastPos() 	{return m_vLastPos;}
	tbVector2 GetVel() 		{return m_vVel;}
	tbVector2 GetLastVel() 	{return m_vLastVel;}
	tbVector2 GetWH() 		{return m_vWH;}
	
	void DoDestroy(list<S_LevelIntern> *palObjects); //simply deleting just yet
private:
	
	void MoveX(list<S_LevelIntern> *palObjects); //moving in x-direction
	void MoveY(list<S_LevelIntern> *palObjects); //movind in y-direction
	
	glcTexture m_TexSolidArgument;
	tbVector2  m_vPos, m_vLastPos;
	tbVector2  m_vWH;
	tbVector2  m_vVel, m_vLastVel;
	int m_iCanShowThought; // 1->can show thought, 2->maybe can show thought because there was already the thought shown, 0->definitly cannot show thought
	
	static const tbVector2 c_vMaxSpeed;
	static const tbVector2 c_vAcceleration;
};

#endif /*CSOLIDARGUMENT_H_*/
