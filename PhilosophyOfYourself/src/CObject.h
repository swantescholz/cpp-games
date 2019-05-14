#ifndef COBJECT_H_
#define COBJECT_H_

#include "../GLC/GLC.h"
#include "CGameState.h"

enum EObjectType {
	OT_PLAYER = 0,			//0  the player
	OT_AXIOM,				//1  solid ground
	OT_SOLID_ARGUMENT,		//2  movable block
	OT_RASH_ARGUMENT,		//3  slippery movable block
	OT_FALSE_BELIEF,		//4  solid ground just for blocks, not for player
	OT_IGNORANCE,			//5  destructive lava
	OT_LIE,					//6  kinda bomb
	OT_ADVICE,				//7	 hint box, internet, newspaper or book
	OT_PROOF,				//8  checkpoint
	OT_UNKNOWN,				//9  top/left/right boundary
	OT_ANSWER,				//10 goal of level
	OT_THOUGHT,				//11 backgroung thought-text
	OT_LAST,				//12 number of different object types
	OT_NONE,				//13 shows that object is eliminated/ or destroying, ->particle effect, sounds
	//OT_CONVENTION,		//TODO maybe
	//OT_PREJUDICE,			//TODO maybe
	//OT_DOGMA,				//TODO maybe
};


class CObject;
struct S_LevelIntern { //used for level and must be known here
	CObject *pObj;
};

class CObject
{
public:
	CObject();
	virtual ~CObject(); //that virtual was VERY IMPORTAND once here!
	
	virtual void Update(list<S_LevelIntern> *palObjects) = 0;	//standart updating
	virtual void Render() = 0;									//standart rendering
	virtual bool PushX(list<S_LevelIntern> *palObjects, CObject *pObj) = 0; //returns weather it could be pushed and changes pObj in x-direction
	virtual bool PushY(list<S_LevelIntern> *palObjects, CObject *pObj) = 0; //returns weather it could be pushed and changes pObj in y-direction
	virtual void AddThought(list<S_LevelIntern> *palObjects) = 0;			//adds a thought object to the thought list
	virtual tbVector2 GetCenter() = 0;										//returns the center of an object
	virtual bool Collision(tbVector2 pvCenter, tbVector2 pvWH) = 0;											 //a standart collision test
	virtual bool CanBePassedThrough(tbVector2 pvCenter, tbVector2 pvWH) {return !Collision(pvCenter,pvWH);}  //normally just opposite like collision
	virtual void Delete() {m_bDeleted = true;}		//an elimination, maybe overwritten
	
	//get and set methods
	virtual void SetPos(tbVector2 pv) 	{}
	virtual void SetVel(tbVector2 pv) 	{}
	virtual tbVector2 GetPos() 			{return tbVector2(0.0f,0.0f);}
	virtual tbVector2 GetLastPos() 		{return tbVector2(0.0f,0.0f);}
	virtual tbVector2 GetVel() 			{return tbVector2(0.0f,0.0f);}
	virtual tbVector2 GetLastVel() 		{return tbVector2(0.0f,0.0f);}
	virtual tbVector2 GetWH() 			{return tbVector2(0.0f,0.0f);}
	
	bool 		IsDeleted() {return m_bDeleted;} 	//am I deleted?
	EObjectType GetOT()   	{return m_OT;}			//returns the object type value
	
protected:
	EObjectType m_OT;								//the object type
	bool 		m_bDeleted;							//am I still alive?
	string		m_sThought;							//the thought text which all objects have
};



#endif /*COBJECT_H_*/
