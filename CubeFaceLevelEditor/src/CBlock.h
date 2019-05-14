#ifndef CBLOCK_H_
#define CBLOCK_H_

#include "GLC.h"
#include "CGS.h"

class CBlock //TODO: mehr bloecke
{
public:
	CBlock();
	CBlock(string sLoad);	//1: face, 2: pos, 3: wh, 4: type
	~CBlock();
	
	void Load(string sLoad);
	
	void Update();
	void Render();
	string GetStr();
	bool Collision(tbVector2 vPos, tbVector2 vWH = tbVector2(0.0f,0.0f) );
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
