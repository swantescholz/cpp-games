#ifndef CLEVEL_H_
#define CLEVEL_H_

#include <typeinfo>

#include "../GLC/GLC.h"
#include "CGS.h"
#include "CBlock.h"

#define g_pLevel CLevel::Get()

class CLevel : public TSingleton<CLevel>
{
public:
	CLevel();
	~CLevel();
	
	void Update();
	void UpdateBlockGD();
	void Render();
	
	void Load(string sPath);
	string GetPlayerInitString () {return m_sPlayerInit;}
	
	ECollisionState Collision(ECubeFace face, tbVector2 vPos, tbVector2 vWH);
	
private:
	list<CBlock> 			m_lBlock;
	list<CBlock>::iterator	m_itl;
	
	const int m_iTextSize;
	const tbColor m_colText;
	
	glcFont ttfSans;
	glcTexture m_Wood;
	
	
	
	EGravitationDirection m_LastGD;	//letzte gravitationsrichtung
	
	string m_sTitle;
	string m_sPlayerInit;	//player initialisierungstext
};

#endif /*CLEVEL_H_*/
