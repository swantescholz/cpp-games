#ifndef CTHOUGHT_H_
#define CTHOUGHT_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"

//an not solid background text-thought object-type
class CThought : public CObject
{
public:
	CThought(string psText,string psFont,int piSize,glcMaterial pMat,
		  tbVector2 pv1,tbVector2 pv2,tbVector2 pv3,tbVector2 pv4);
	~CThought();
	
	void Update(list<S_LevelIntern> *palObjects);
	void Render();
	bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj);
	bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj);
	void AddThought(list<S_LevelIntern> *palObjects) {return;} //is a thought, so should not add a thought
	tbVector2 GetCenter() {return (m_vPos[0]+m_vPos[2])/2.0f;}
	bool Collision(tbVector2 pvCenter, tbVector2 pvWH);
	bool CanBePassedTrough(tbVector2 pvCenter, tbVector2 pvWH) {return true;}
	
	void Start(tbVector2 pvPos = tbVector2(0.0), tbVector2 pvVel = tbVector2(0.0), tbVector2 pvAcc = tbVector2(0.0), float pfFade = 0.0f);
	
	bool  GetActive () const			{return m_bActive;	}
	void  SetActive (bool b) 			{m_bActive = b;		}
	float GetFade   () const			{return m_fFade;	}
	void  SetFade   (float f)			{m_fFade = f;		}
	float GetLife	() const			{return m_fLife;	}
	void  SetLife	(float f)			{m_fLife = f;		}
	tbVector3 GetVel()	const			{return m_vVel;}
	tbVector3 GetAcc()	const			{return m_vAcc;}
	void SetVel(tbVector2 pv)			{m_vVel = pv;}
	void SetAcc(tbVector2 pv)			{m_vAcc = pv;}
	
private:
	glcFont m_Font;			//the font
	tbVector2 m_vPos[4];	//the four corners
	glcMaterial m_Mat;		//the material property
	
	float m_fLife;
	float m_fFade;
	bool  m_bActive;
	float m_fZValue, m_fZValueVel;
	
	tbVector2 m_vVel;
	tbVector2 m_vAcc;
};

#endif /*CTHOUGHT_H_*/

