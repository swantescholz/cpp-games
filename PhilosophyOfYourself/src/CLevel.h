#ifndef CLEVEL_H_
#define CLEVEL_H_

#include "../GLC/GLC.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CThought.h"
#include "CAxiom.h"
#include "CSolidArgument.h"
#include "CUnknown.h"
#include "CProof.h"
#include "CIgnorance.h"
#include "CRashArgument.h"
#include "CAdvice.h"

//three load types in addition to the object types
enum ELoadType {
	LT_MUSIC 		= -4,
	LT_BACKGROUND	= -3,
	LT_LIGHT		= -2,
	LT_CAMERA		= -1
};

//a class that controls the whole level
class CLevel
{
public:
	CLevel(CLevel &rhs); //copy constructor
	CLevel(string pPath);
	~CLevel();
	
	void operator = (CLevel &rhs);
	
	void Update();
	void Render();
	
	float GetLightAngle() {return m_fLightAngle;}
	
private:
	CLevel(); //should not be used

	void LoadLevel(string pPath);	//loads a complete level textfile
	void LoadObject(string ps);		//read one line of level textfile
	
	CObject* DoubleObjPtr(CObject *pObj);									//it doubles one object pointer on the heap(used for saving the level)
	CObject* GetPlayer() {return m_alObjects[OT_PLAYER].begin()->pObj;}		//it returns the first player
	
	list<S_LevelIntern> m_alObjects[OT_LAST];	//list of all objects
	list<S_LevelIntern>::iterator m_lit;		//iterator
	
	
	glcTexture m_TexBg;		//background texture
	tbVector2 m_vBgWH;		//width/heigth of background
	int m_iBGNumPasses;		//number of subdivisions
	glcLight *m_pLight1;	//a pointer to the light
	glcMusic m_Music;		//music
	
	float m_fLightZ, m_fLightInterpolationFactor; //light z plane, speed to follow player
	float m_fCam1Z,  m_fCam1InterpolationFactor, m_fCam1Scope[2];   // cam1 z plane, speed to follow player, radius not to follow(min,max)
	
	float m_fLightAngle;
	
	static int c_iNumInstances; //number of me
};

#endif /*CLEVEL_H_*/

