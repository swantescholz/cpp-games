#ifndef GLCSDL_GL_H_
#define GLCSDL_GL_H_


#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <fstream>
#include "Singleton.h"
#include "glcTimer.h"
#include "glcException.h"
#include "glcLogfile.h"
#include "tbMatrix.h"
#include "tbVector3.h"
#include "tbVector2.h"
#include "tbPlane.h"
#include "tbColor.h"
#include "tbUtils.h"


#define TB_DEG_TO_RAD(X) ((X) * 0.0174532925199)
#define TB_RAD_TO_DEG(X) ((X) * 57.295779513082)

#define g_pSDL_GL glcSDL_GL::Get()
#define g_pGL glcSDL_GL::Get()
#define ALLW glcSDL_GL::Get()->GetAllW()
#define ALLH glcSDL_GL::Get()->GetAllH()
#define VCAM1 glcSDL_GL::Get()->vCam1
#define VCAM2 glcSDL_GL::Get()->vCam2
#define VCAM3 glcSDL_GL::Get()->vCam3

//glc - flags fuer init
#define GLC_LIGHTING 	nflag::a8[0]
#define GLC_BLEND 		nflag::a8[1]
#define GLC_CULLING 	nflag::a8[2]
#define GLC_FOG		 	nflag::a8[3]
#define GLC_NONE		nflag::a8[4]
#define GLC_TEXTURE2D	nflag::a8[5]

//nuetzliche makros fuer sicherheit
#define GLC_SAFE_DELETE(x)		{if((x)) {delete (x); (x) = NULL;}}
#define GLC_SAFE_DELETE_ARRAY(x){if((x)) {delete[] (x); (x) = NULL;}}
#define GLC_SAFE_CLOSE_FONT(x)	{if((x)) {TTF_CloseFont((x)); (x) = NULL;}}
#define GLC_SAFE_FREE_SURFACE(x){if((x)) {SDL_FreeSurface((x)); (x) = NULL;}}
#define GLC_SAFE_FREE_CHUNK(x)	{if((x)) {Mix_FreeChunk((x)); (x) = NULL;}}

//typedefs fuer flags
typedef unsigned char 	flag8;
typedef unsigned short 	flag16;
typedef unsigned int	flag32;

//arrays mit 2-potenzen fuer flags
namespace nflag {
	const flag8 	a8[8] = {0x01, 0x02, 0x04, 0x08,
							 0x10, 0x20, 0x40, 0x80};
	const flag16   a16[16]= {0x0001,0x0002,0x0004,0x0008,
							 0x0010,0x0020,0x0040,0x0080,
							 0x0100,0x0200,0x0400,0x0800,
							 0x1000,0x2000,0x4000,0x8000};
	const flag32   a32[32]= {0x00000001,0x00000002,0x00000004,0x00000008,
							 0x00000010,0x00000020,0x00000040,0x00000080,
							 0x00000100,0x00000200,0x00000400,0x00000800,
							 0x00001000,0x00002000,0x00004000,0x00008000,
							 0x00010000,0x00020000,0x00040000,0x00080000,
							 0x00100000,0x00200000,0x00400000,0x00800000,
							 0x10000000,0x02000000,0x04000000,0x08000000,
							 0x10000000,0x20000000,0x40000000,0x80000000};
};

namespace nmath { // praktische kleine (mathematische) funktionen
	const double dPi = 3.1415926535897;
	const double dEuler = 2.718281828450;
	const tbVector3 vZero(0.0f,0.0f,0.0f); //koordinatenursprung
	const tbVector3 vXAxis(1.0f,0.0f,0.0f);
	const tbVector3 vYAxis(0.0f,1.0f,0.0f);
	const tbVector3 vZAxis(0.0f,0.0f,1.0f);
	const tbVector3 vMinOne(-1.0f,-1.0f,-1.0f);
	const tbVector3 vMaxOne(1.0f,1.0f,1.0f);
	const tbMatrix	mIdentity(1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f);
	template<class T> inline T Betrag(T I) {if (I < 0.0f) I*=(-1); return I;}
	template<class T> inline T Min(T A, T B) {if(A < B) return A; return B;}
	template<class T> inline T Max(T A, T B) {if(A < B) return B; return A;}
	template<class T> inline T Nearest(T A, T B, T C) {if(betrag(C-B) < betrag(C-A)) return B; return A;}
	template<class T> inline int Round(T A) {return (int)(A + 0.5f);}
	template<class T> inline void Switch(T &B){if(B==true){B=false;}else{B=true;}}
	template<class T> inline void Swap (T &Var1, T &Var2) {T Var3 = Var1; Var1 = Var2; Var2 = Var3;}
	template<class T> inline int Sign(T A) {if (A < 0.0f) return -1; return 1;}
	template<class T> inline T DegToRad(T X) {return X * 0.0174532925199;}
	template<class T> inline T RadToDeg(T X) {return X * 57.295779513082;}
}


struct S_VertexCNP {
	tbColor col;	// float color
	tbVector3 vN;	// normalvector
	tbVector3 vP; 	// position
};
struct S_VertexCP {
	tbColor col;	// float color
	tbVector3 vP; 	// position
};
struct S_VertexNP {
	tbVector3 vN;	// normalvector
	tbVector3 vP; 	// position
};
struct S_Material_adse {
	tbColor ambient, diffuse, specular, emission;
};
struct S_Material_ads {
	tbColor ambient, diffuse, specular;
};
struct S_Material_ad_se {
	tbColor amb_diff, specular, emission;
};
struct S_Material_ad_s {
	tbColor amb_diff, specular;
};

namespace ncol { //float_GL_Farben
	const tbColor black(0.0f,0.0f,0.0f,1.0f);
	const tbColor brown(0.7812f,0.44f,0.215f,1.0f);
	const tbColor grey(0.5f,0.5f,5.0f,1.0f);
	const tbColor white(1.0f,1.0f,1.0f,1.0f);
	const tbColor red(1.0f,0.0f,0.0f,1.0f);
	const tbColor green(0.0f,1.0f,0.0f,1.0f);
	const tbColor blue(0.0f,0.0f,1.0f,1.0f);
	const tbColor yellow(1.0f,1.0f,0.0f,1.0f);
	const tbColor orange(1.0f,0.6f,0.33f,1.0f);
	const tbColor pink(1.0f,0.0f,1.0f,1.0f);
	const tbColor purple(0.5f,0.0f,0.5f,1.0f);
}


class glcSDL_GL : public TSingleton<glcSDL_GL>
{
public:
	glcSDL_GL();
	~glcSDL_GL();
	
	//methoden
	void Init(int width = 800, int hight = 600, flag8 GLflags = GLC_CULLING | GLC_FOG | GLC_LIGHTING, int SDLflags = SDL_OPENGL, string sWindow = "No_Name | by Swante Scholz 2009", string sTab = "No_Name");
	void Quit();
	
	void Update();							//updates events
	void Flip() {SDL_GL_SwapBuffers();}		//swaps buffers to the screen
	
	void ClearColor(SDL_Color col) 					{glClearColor(col.r/255.0f,col.g/255.0f,col.b/255.0f,0);}
	void ClearColor(tbColor col) 					{glClearColor(col.r,col.g,col.b,col.a);}
	void LoadMatrix(const tbMatrix& m)				{glLoadMatrixf(m.n);}
	void MultMatrix(const tbMatrix& m)				{glMultMatrixf(m.n);}
	void Translate(const tbVector3& v) 				{glTranslatef(v.x,v.y,v.z);}
	void Rotate(float fAngle, const tbVector3 &v) 	{glRotatef(fAngle,v.x,v.y,v.z);}
	void LookAt(const tbVector3 &v1, const tbVector3 &v2 = tbVector3(0.0f,0.0f,0.0f), const tbVector3 &v3 = tbVector3(0.0f,1.0f,0.0f)) 	{gluLookAt(v1.x,v1.y,v1.z,v2.x,v2.y,v2.z,v3.x,v3.y,v3.z);}
	void LookAt() 	{gluLookAt(vCam1.x,vCam1.y,vCam1.z,vCam2.x,vCam2.y,vCam2.z,vCam3.x,vCam3.y,vCam3.z);} 
	
	//eingabe
	bool KeyIsDown (int key_ID)		{return m_aKeyIsDown[key_ID];}
	bool KeyWasDown(int key_ID)		{if(m_aKeyWasDown[key_ID]) {m_aKeyWasDown[key_ID] = false; return true; } return false;}
	bool KeyWasUp  (int key_ID)		{if(m_aKeyWasUp[key_ID]) {m_aKeyWasUp[key_ID] = false; return true; } return false;}
	int  GetMouseXPosi () {return m_iMouseXPos;}
	int  GetMouseYPosi () {return m_iMouseYPos;}
	float GetMouseXPosf () {return m_fMouseXPos;}
	float GetMouseYPosf () {return m_fMouseYPos;}
	bool MouseIsDown(int key_ID)	{return m_aMouseIsDown[key_ID];}
	bool MouseWasDown(int key_ID)	{if(m_aMouseWasDown[key_ID]) {m_aMouseWasDown[key_ID] = false; return true;} return false;}
	bool MouseWasUp(int key_ID)		{if(m_aMouseWasUp[key_ID]) {m_aMouseWasUp[key_ID] = false; return true;} return false;}
	bool MouseQuit()				{if(m_bMouseQuit) {m_bMouseQuit = false; return true;} return false;}
	bool MouseDoubleClick()			{if(m_bMouseDoubleClick) {m_bMouseDoubleClick = false; return true;} return false;}
	
	//window
	int GetAllW() {return m_iAllW;}
	int GetAllH() {return m_iAllH;}
	SDL_Surface* GetScreen() {return m_pScreen;}
	
	//membervariablen
	tbVector3 vCam1, vCam2, vCam3; //die 3 cameravektoren
private:
	glcTimer m_MouseClickTimer;

	int m_iMouseXPos, m_iMouseYPos; 	// in pixeln (0,0 oben links)
	float m_fMouseXPos, m_fMouseYPos; 	// als float im koordinatensystem (0,0 mitte des fensters)
	bool m_bMouseQuit;					// quit-button gedrueckt?
	bool m_bMouseDoubleClick;			// doppelclick?
	bool m_aMouseIsDown[10];
	bool m_aMouseWasDown[10];
	bool m_aMouseWasUp[10];
	
	bool m_aKeyIsDown[SDLK_LAST];
	bool m_aKeyWasDown[SDLK_LAST];
	bool m_aKeyWasUp[SDLK_LAST];
	
	int m_iAllW, m_iAllH;
	SDL_Surface *m_pScreen;
	SDL_Event event;
};

#endif /*GLCSDL_GL_H_*/
