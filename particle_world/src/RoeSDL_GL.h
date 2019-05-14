#ifndef ROESDL_GL_H_
#define ROESDL_GL_H_


#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>

#include <iostream>
#include <string>
#include <fstream>
#include "RoeSingleton.h"
#include "RoeCommon.h"
#include "RoeTimer.h"
#include "RoeVector2.h"
#include "RoeVector3.h"
#include "RoeColor.h"
#include "RoeMaterial.h"
#include "RoeCamera.h"

#define roe_sdlgl Roe::SDL_GL::getSingleton()

namespace Roe {
	
	//controls SDL, SDL_ttf, SDL_image and OpenGL initialisation and quitting, but NOT SDL_mixer
	class SDL_GL : public Roe::Singleton<SDL_GL>
	{
	public:
		enum ESDL_GL_Flag {
			F_FULLSCREEN     = ROE_POW_2_0, //fullscreen
			F_NO_FRAME       = ROE_POW_2_1, //no frame
			F_RESIZABLE      = ROE_POW_2_2, //window is resizable (DONT USE THIS)
			F_CULL_SHOW_CW   = ROE_POW_2_3, //cw sides visible, ccw sides culled
			F_CULL_SHOW_CCW  = ROE_POW_2_4, //ccw sides visible, cw sides culled
			F_LIGHTING       = ROE_POW_2_5, //lighting on
			F_ALPHA_BLENDING = ROE_POW_2_6, //enable aplha transparency?
			F_TEXTURE2D      = ROE_POW_2_7, //with texture mapping?
			F_FOG            = ROE_POW_2_8  //fog enabled?
		};

		SDL_GL();
		~SDL_GL();
		
		//methods
		void init(int width, int hight, std::string sWindow = "No_Name | by swash93 2010",
		          std::string sTab = "No_Name", std::string sIcon = "", flag32 flags = F_CULL_SHOW_CCW | F_TEXTURE2D);
		void quit();
		
		void   flip           () {SDL_GL_SwapBuffers();}            //swaps buffers to the screen
		double getCurrentTime () {return SDL_GetTicks() / 1000.0;}  //returns the current time
		
		// *gl*
		void clearBuffers ()        {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );} //at the beginning of each frame
		void setClearColor(Color c) {glClearColor(c.r,c.g,c.b,c.a);}
		void resetModelViewMatrix() {glMatrixMode(GL_MODELVIEW); glLoadIdentity();} //call that just after clearBuffers!
		void applyCameraToScene(const Camera& c); //cameras operations to scene (BEFORE the scene is rendered)
		void renderSkyBox(const Vector3& size, const Vector3& center, const std::string& name, //call BEFORE other renderings
		                  const std::string& up    = "_up", const std::string& down  = "_dn",  //the six surfaces are oriented from middle position
		                  const std::string& left  = "_lf", const std::string& right = "_rt",
		                  const std::string& front = "_fr", const std::string& back  = "_bk", const Color& color = Color(1,1,1,1));
		
		//input
		void updateEvents               (); //updates events
		void resetKeyEvents    (bool isDown = true, bool wasDown = true, bool wasUp = true);
		void resetMouseEvents  (bool isDown = true, bool wasDown = true, bool wasUp = true);
		bool keyIsDown (int key_ID)     {return m_aKeyIsDown[key_ID];}
		bool keyWasDown(int key_ID, bool setFalse = true) {if(m_aKeyWasDown[key_ID]) {m_aKeyWasDown[key_ID] = !setFalse; return true; } return false;}
		bool keyWasUp  (int key_ID, bool setFalse = true) {if(m_aKeyWasUp  [key_ID]) {m_aKeyWasUp  [key_ID] = !setFalse; return true; } return false;}
		int     getMouseXPosi () {return m_iMouseXPos;} //starts from upper-left corner
		int     getMouseYPosi () {return m_iMouseYPos;} //begins with 0 ends with w-1 or h-1
		Vector2 getMousePosf  () {return Vector2(m_fMouseXPos, m_fMouseYPos);} //float mouse position from center
		void    setMouseXPosi (int x);
		void    setMouseYPosi (int y);
		void    setMousePosi  (int x, int y);
		void    setMousePosf  (Vector2 xy);
		bool mouseIsDown (int key_ID)  {return m_aMouseIsDown[key_ID];}
		bool mouseWasDown(int key_ID, bool setFalse = true) {if(m_aMouseWasDown[key_ID]) {m_aMouseWasDown[key_ID] = !setFalse; return true;} return false;}
		bool mouseWasUp  (int key_ID, bool setFalse = true) {if(m_aMouseWasUp  [key_ID]) {m_aMouseWasUp  [key_ID] = !setFalse; return true;} return false;}
		bool mouseQuit()               {if(m_bMouseQuit)        {m_bMouseQuit        = false; return true;} return false;}
		bool mouseDoubleClick()        {if(m_bMouseDoubleClick) {m_bMouseDoubleClick = false; return true;} return false;}
		
		//window
		void         applyBlackWhiteFilter();              //makes the screen black/white
		void         setIcon          (std::string sIcon); //sets the icon image
		void         saveScreenshot   (std::string sPath); //saves a screenshot as bmp or tga
		void         grabInput     (bool enable = true);   //the window gets nearly all input rights
		void         toggleFullscreen();                   //toggles between fullscreen/windowed mode
		bool         isFullscreen  () {return m_pScreen->flags & SDL_FULLSCREEN;}
		void         minimizeWindow() {SDL_WM_IconifyWindow();} //minimizes window
		void         resizeWindow(int w, int h); //resizes window
		int          getWindowW    () {return m_iWidth;}
		int          getWindowH    () {return m_iHeight;}
		Vector2      getWindowSize () {return Vector2(m_iWidth, m_iHeight);}
		SDL_Surface* getScreen     () {return m_pScreen;}
		
		static const int s_ciMinWidth, s_ciMaxWidth, s_ciMinHeight, s_ciMaxHeight; // = 400,1600,300,1200
	private:
		void setGLViewport(int w, int h); // Setup the viewport and perspective
		
		//membervariables
		Timer m_MouseClickTimer;
		int m_iMouseXPos, m_iMouseYPos;     // in pixels (0,0 top left)
		float m_fMouseXPos, m_fMouseYPos;   // as floats (0,0 is center)
		bool m_bMouseQuit;                  // quit-button pressed=
		bool m_bMouseDoubleClick;           // double clicked?
		bool m_aMouseIsDown [10];
		bool m_aMouseWasDown[10];
		bool m_aMouseWasUp  [10];
		
		bool m_aKeyIsDown [SDLK_LAST];
		bool m_aKeyWasDown[SDLK_LAST];
		bool m_aKeyWasUp  [SDLK_LAST];
		
		byte *m_pPixelData;
		int m_iWidth, m_iHeight;
		SDL_Surface *m_pScreen;
		SDL_Event m_Event;
		unsigned long m_SDL_Flags;
	};
	
} // namespace Roe

#endif /*ROESDL_GL_H_*/
