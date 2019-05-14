#ifndef CINTRO_H_
#define CINTRO_H_

#include "../GLC/GLC.h"
#include "CGS.h"

class CIntro
{
public:
	CIntro(float fAng = 90.0f);
	~CIntro();
	
	void Update();
	void Render();
	
private:
	glcMusic	m_Music;
	glcFont 	m_ttfSans;
	glcTexture	m_Wood;
	const float m_fRotAngle; //rotation per second
};

#endif /*CINTRO_H_*/
