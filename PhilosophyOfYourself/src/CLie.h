#ifndef CLIE_H_
#define CLIE_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CGameState.h"
#include "CPlayer.h"
#include "CUnknown.h"
#include "CAnswer.h"
#include "CFalseBelief.h"
#include "CProof.h"
#include "CIgnorance.h"
#include "CSolidArgument.h"

//an object-type class for a lie
//its a movable block, which explodes, when something falls on it
class CLie : public CObject
{
public:
	CLie(const CLie &rhs);
	CLie(tbVector2 pvStartPos, string psThought);
	~CLie();
	
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
	
	void DoExplode(list<S_LevelIntern> *palObjects); //do the explosion
	
	static const float c_fMinVelToExplode; //minimum speed ot explode (negative)
private:
	
	void MoveX(list<S_LevelIntern> *palObjects); //moving in x-direction
	void MoveY(list<S_LevelIntern> *palObjects); //movind in y-direction
	
	glcTexture 		m_TexLie;
	glcTexture		m_TexParticle;
	tbVector2  		m_vPos, m_vLastPos;
	tbVector2  		m_vWH;
	tbVector2  		m_vVel, m_vLastVel;
	bool 			m_bExploding;
	float 			m_fExplosionTime;
	float 			m_fExplosionRadius;
	const int		m_ciNumParticles;			//number of particles for effect
	glcParticle 	*m_aParticles;
	int m_iCanShowThought; // 1->can show thought, 2->maybe can show thought because there was already the thought shown, 0->definitly cannot show thought
	
	const string m_sSndExplode; //string for sound when lie explodes
	
	static const tbVector2 	c_vMaxSpeed;			//maximum speed x/y
	static const tbVector2 	c_vAcceleration;		//acceleration  x/y
	static const float 		c_fExplosionMaxTime;	//duration of explosion
	static const float 		c_fExplosionMaxRadius;	//radius of explotion in the end
	static const float		c_fExplosionEndVel;		//speed of particles at the end
};

#endif /*CLIE_H_*/
