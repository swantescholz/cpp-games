#ifndef CIGNORANCE_H_
#define CIGNORANCE_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CSolidArgument.h"
#include "CLie.h"
#include "CPlayer.h"
#include "CRashArgument.h"

//a destructive object-type
//kinda lava block, it will destroy everything it touches
class CIgnorance : public CObject
{
public:
	CIgnorance(tbVector2 pvStartPos, string psThought);
	~CIgnorance();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects);
	tbVector2 GetCenter() {return m_vPos;}
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	
private:
	void DoDestroying(list<S_LevelIntern> *palObjects, CObject *pObj);
	
	glcTexture m_TexIgnorance;
	tbVector2 m_vPos;
	tbVector2 m_vWH;
	
	const string m_sSndDestroy; // string for sound when it destroy something
};

#endif /*CIGNORANCE_H_*/
