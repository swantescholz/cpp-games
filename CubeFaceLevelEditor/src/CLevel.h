#ifndef CLEVEL_H_
#define CLEVEL_H_

#include <typeinfo>

#include "GLC.h"
#include "CGS.h"
#include "CBlock.h"

#define g_pLevel CLevel::Get()

class CLevel : public TSingleton<CLevel>
{
public:
	CLevel();
	~CLevel();
	
	void Update();
	void Render();
	
	void LoadLevel(string sPath);	//level laden
	void DeleteBlock();				//einen block loeschen
	void SaveLevel();				//level abspeichern
	void LeftClick();				//ein links click
	
private:
	bool MouseFaceCollision (tbVector2 *pv);		//mouseklick collision mit face
	void DrawRaster (GLenum mode);
	tbVector2 ProcessHits (GLint hits, GLuint buffer[]);
	
	list<CBlock> 			m_lBlock;
	list<CBlock>::iterator	m_itl;
	
	const int m_iTextSize;
	const tbColor m_colText;
	
	string m_sTitle;
	glcFont ttfSans;
	glcTexture m_TexRaster;
};

#endif /*CLEVEL_H_*/
