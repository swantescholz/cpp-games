#ifndef CBLOCK_H_
#define CBLOCK_H_

#include "../GLC/GLC.h"
#include "CGS.h"

enum EBlockType {
	BT_SOLID,
	BT_GOAL,
	BT_KILL,
	
	BT_LAST
};

class CBlock //TODO: mehr bloecke
{
public:
	CBlock();
	CBlock(string sLoad);	//1: face, 2: pos, 3: wh, 4: type
	~CBlock();
	
	void Load(string sLoad);
	
	void UpdateGD(EGravitationDirection lastGD);	//updatet nur GD
	void Update();
	void Render();
	
	ECollisionState Collision(ECubeFace face, tbVector2 vPos, tbVector2 vWH);
	ECubeFace GetFace() {return m_Face;}
	
private:
	tbVector2 m_vPos;
	ECubeFace m_Face;
	tbVector2 m_vWH;
	
	tbVector2 m_vWrap; //std wrapping abstand
	
	glcTexture m_Tex;
	EBlockType m_Type;
};

#endif /*CBLOCK_H_*/
