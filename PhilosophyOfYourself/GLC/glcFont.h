#ifndef GLCFONT_H_
#define GLCFONT_H_


#include "glcSDL_GL.h"




struct S_FontIntern {
	vector<TTF_Font*> vFont;
	int iSizeMin, iSizeMax;
};

//zum render erst Begin() und an ende End() aufrufen (fuer 2d)
class glcFont
{
public:
	glcFont();
	glcFont(const string &s, int iSize);			//ruft nur load auf
	~glcFont();
	
	void Load(const string &s, int iSize);				//loaded nur fuer eine instanz eine font aus der font-map
	string GetName() const {return m_sName;}
	void SetSize(int iSize);
	int  GetSize() const   {return m_iSize;}
	
	//mit 2d (zwischen Begin() und End()
	//ohne extra color
	void RenderText(const std::string& text, SDL_Rect *rct) {
		tbVector2 vTmp;
		RenderText(GetFont (), 255, 255, 255, rct->x, rct->y, 0, text, &vTmp);
		rct->w = vTmp.x;
		rct->h = vTmp.y;
	}
	void RenderText(const std::string& text, const tbVector2 &v, tbVector2 *pvOut = NULL) {
		RenderText(GetFont (), 255, 255, 255, v.x, v.y, 0, text, pvOut);
	}
	void RenderText(const std::string& text, const tbVector3 &v, tbVector2 *pvOut = NULL) {
		RenderText(GetFont (), 255, 255, 255, v.x, v.y, v.z, text, pvOut);
	}
	
	//mit extra-color
	void RenderText(const std::string& text, const tbColor& col, SDL_Rect *rct) {
		glColor4fv(col.c);
		tbVector2 vTmp;
		RenderText(GetFont (), 255, 255, 255, rct->x, rct->y, 0, text, &vTmp);
		rct->w = vTmp.x;
		rct->h = vTmp.y;
	}
	void RenderText(const std::string& text, const tbColor& col, const tbVector2 &v, tbVector2 *pvOut = NULL) {
		glColor4fv(col.c);
		RenderText(GetFont (), 255, 255, 255, v.x, v.y, 0, text, pvOut);
	}
	void RenderText(const std::string& text, const tbColor& col, const tbVector3 &v, tbVector2 *pvOut = NULL) {
		glColor4fv(col.c);
		RenderText(GetFont (), 255, 255, 255, v.x, v.y, v.z, text, pvOut);
	}
	
	//mit 3D (nicht zwischen begin und end)
	//v1: unten links, v2: unten rechts, v3: oben rechts, v4: oben links
	void RenderText(const std::string& text, const tbColor& col, const tbVector3 &v1, const tbVector3 &v2,
					const tbVector3 &v3, const tbVector3 &v4, bool bDisableDepthTest = false, float fOffset = 0.0f);
	//with ligth (TODO: subdividing)
	void RenderText(const std::string& text, const tbColor& amb, const tbColor& diff, const tbColor& spec,
					const tbVector3 &v1, const tbVector3 &v2, const tbVector3 &v3, const tbVector3 &v4,
					bool bDisableDepthTest = false, float fOffset = 0.0f);
	void RenderText(const std::string& text, const glcMaterial &pMat,
					const tbVector3 &v1, const tbVector3 &v2, const tbVector3 &v3, const tbVector3 &v4,
					bool bDisableDepthTest = false, float fOffset = 0.0f);
	void RenderText(const std::string& text, tbColor *acol,
					const tbVector3 &v1, const tbVector3 &v2, const tbVector3 &v3, const tbVector3 &v4,
					bool bDisableDepthTest = false, float fOffset = 0.0f) {
						RenderText(text,acol[0],acol[1],acol[2],v1,v2,v3,v4,bDisableDepthTest,fOffset);
					}
	
	//echte basis-funktion
	void RenderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B, const double& X, 
							 const double& Y, const double& Z,  const std::string& Text, tbVector2* pvOut = NULL);
							 
	TTF_Font* GetFont () {
		c_mpi = c_mpFont.find(m_sPath);
		return c_mpi->second.vFont[m_iSize - c_mpi->second.iSizeMin];
	}
	
	
	static void Begin();		// Go in HUD-drawing mode
	static void End();			// Come out of HUD mode
	
	static void LoadTTF (string sPath, int ptsizeMin, int ptsizeMax); //ladet alle fonts zwischen min und max
	static void DeleteTTF (string sPath, bool bCompleteThePath = true); //deletes the complete font
	// fuer die statischen variablen
	static void Init(string sStart, string sEnd)	{c_sPathStart = sStart; c_sPathEnd = sEnd;}
	static void SetPathStart(string s) 				{c_sPathStart = s;}
	static void SetPathEnd  (string s) 				{c_sPathEnd   = s;}
	static void Quit();
	
private:
	int nextpoweroftwo(int x) {
		double logbase2 = log(x) / log(2);
		return nmath::Round(pow(2,ceil(logbase2)));
	}
	
	
	static map<string, S_FontIntern> 				c_mpFont;	//map mit allen display lists
	static map<string, S_FontIntern>::iterator 		c_mpi;		//map iterator
	static pair<string, S_FontIntern> 				c_pair;		//paar-objekt fuer die map
	static string c_sPathStart, c_sPathEnd;						//standart path
	
	string	m_sPath;								//vollstaendiger eigener path
	string  m_sName;
	int 	m_iSize;								//eigene groesse
};

#endif /*GLCFONT_H_*/
