#ifndef CUNKNOWN_H_
#define CUNKNOWN_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CSolidArgument.h"
#include "CLie.h"
#include "CPlayer.h"
#include "CRashArgument.h"


//an object type, pretty much  like axiom: its solid, unpushable), but it has an other design
//its supposed to be the top/left/right boundary
class CUnknown : public CObject
{
public:
	CUnknown(tbVector2 pvStartPos, string psThought);
	~CUnknown();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects);
	tbVector2 GetCenter() {return m_vPos;}
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	bool CanBePassedThrough(tbVector2 pvCenter, tbVector2 pvWH) {return !Collision(pvCenter,pvWH);}
	
private:
	glcTexture m_TexUnknown;
	tbVector2 m_vPos;
	tbVector2 m_vWH;
	int m_iCanShowThought; // 1->can show thought, 2->maybe can show thought because there was already the thought shown, 0->definitly cannot show thought
};

#endif /*CUNKNOWN_H_*/
