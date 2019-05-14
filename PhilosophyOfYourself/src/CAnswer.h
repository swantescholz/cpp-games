#ifndef CANSWER_H_
#define CANSWER_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CPlayer.h"

//a class for the answer goal block
//player wins when he touches it
//unsolid for movable blocks
class CAnswer : public CObject
{
public:
	CAnswer(tbVector2 pvStartPos, string psThought);
	~CAnswer();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	tbVector2 GetCenter() {return m_vPos;}
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects);
	bool CanBePassedTrough(tbVector2 pvCenter, tbVector2 pvWH) {return true;}
	
private:
	void DoReached();	//answer reached
	
	glcTexture m_TexAnswer;
	tbVector2 m_vPos;
	tbVector2 m_vWH;
	
	const string m_sSndReached; //string for sound for finding the answer
};

#endif /*CANSWER_H_*/

