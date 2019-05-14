#ifndef CFALSEBELIEF_H_
#define CFALSEBELIEF_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CSolidArgument.h"
#include "CLie.h"
#include "CPlayer.h"
#include "CRashArgument.h"

//a class for false beliefs(espacially religion)
//its an object type
//its solid for blocks, unsolid for the player
class CFalseBelief : public CObject
{
public:
	CFalseBelief(tbVector2 pvStartPos, string psThought);
	~CFalseBelief();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects);
	tbVector2 GetCenter() {return m_vPos;}
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	
private:
	glcTexture m_TexFalseBelief;
	tbVector2 m_vPos;
	tbVector2 m_vWH;
	int m_iCanShowThought; // 1->can show thought, 2->maybe can show thought because there was already the thought shown, 0->definitly cannot show thought
};

#endif /*CFALSEBELIEF_H_*/
