#ifndef GLCTEXTURE_H_
#define GLCTEXTURE_H_

#include "glcSDL_GL.h"

#define FLIP_VERTICAL	nflag::a8[0]
#define FLIP_HORIZONTAL	nflag::a8[1]

#define g_pTexture glcTexture::Get()

struct S_TextureIntern {
	GLuint iTexture;		//textur-index
	//allgemeine textur informationen
	tbMatrix mStdMatrix;	//standart-transformations-matrix
};

class glcTexture : public TSingleton<glcTexture>
{
public:
	glcTexture(const tbMatrix &m = tbMatrixIdentity());
	glcTexture(const string &sPath, const tbMatrix &m = tbMatrixIdentity());
	~glcTexture();
	
	void Load(string sPath);
	
	void SetMatrix(const tbMatrix &m) {m_mMatrix = m;}
	tbMatrix GetMatrix () {return m_mMatrix;}
	
	//nur nach load
	//wechselt in textur-matrix mode und veraendert diese (falls bool true)
	void Bind(tbColor col = tbColor(1,1,1,1), bool bWithTransformation = true);
	
	//laden und loeschen
	static void 	LoadTexture			(string sPath, const tbMatrix &mStdMatrix = tbMatrixIdentity());
	static GLuint	LoadTextureSimple	(const char* file);		//originalversion
	static void		DeleteTexture		(string sPath, bool bCompleteThePath = false);
	
	// fuer die statischen variablen
	static void Init(string sStart, string sEnd);
	static void SetPathStart(string s) 				{c_sPathStart = s;}
	static void SetPathEnd  (string s) 				{c_sPathEnd   = s;}
	static void Quit();
	
	//zusaetzliche funktionen
	static Uint32 GetPixel32( SDL_Surface *surface, int x, int y );
	static void PutPixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
	static SDL_Surface* FlipSurface( SDL_Surface *surface, int flags );
	
private:
	
	static map<string, S_TextureIntern > 				c_mpTexture;
	static map<string, S_TextureIntern >::iterator 		c_mpi;
	static pair<string,S_TextureIntern > 				c_pair;		//paar-objekt fuer die map
	static string c_sPathStart, c_sPathEnd;							//standart path
	
	string 		m_sPath;
	tbMatrix 	m_mMatrix;
};

#endif /*GLCTEXTURE_H_*/
