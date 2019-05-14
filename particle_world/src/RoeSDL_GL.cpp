#include "RoeSDL_GL.h"
#include "RoeUtil.h"
#include "RoeTexture.h"
#include "RoeFont.h"
#include "RoeTextfile.h"
#include "RoeBinFile.h"
#include "RoeException.h"

namespace Roe {
	
	const int SDL_GL::s_ciMinWidth  = 400;
	const int SDL_GL::s_ciMaxWidth  = 1600;
	const int SDL_GL::s_ciMinHeight = 300;
	const int SDL_GL::s_ciMaxHeight = 1200;
	
	SDL_GL::SDL_GL()
	{
		//window
		m_iWidth  = -1;
		m_iHeight = -1;
		m_pScreen = NULL;
		m_pPixelData = NULL;
		m_SDL_Flags = 0;
		
		m_MouseClickTimer.setLimit(0.2f);
		
		//set all keys to false
		for (int i = 0; i < SDLK_LAST; i++)
			m_aKeyIsDown[i] = m_aKeyWasDown[i] = m_aKeyWasUp[i] = false;
		for (int i = 0; i < 10; i++)
			m_aMouseIsDown[i] = m_aMouseWasDown[i] = m_aMouseWasUp[i] = false;
		m_bMouseQuit = false;
		m_bMouseDoubleClick = false;
	}
	
	SDL_GL::~SDL_GL()
	{
	}
	
	void SDL_GL::init(int width, int height, std::string sWindow, std::string sTab, std::string sIcon, flag32 flags) {
		if (m_iWidth != -1) return; // already initialized
		
		// ** SDL **
		//Information about the current video settings
		const SDL_VideoInfo* info = NULL;
		//Color depth in bits of our window
		int bpp = 0;
		
		// First, initialize SDL's video subsystem
		if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0 ) {
			//Failed, therefore exit
			throw Exception(std::string("Video initialization failed: ") + SDL_GetError(), "init", "RoeSDL_GL.cpp", ROE_LINE);
		}
		
		//Let's get some video information
		info = SDL_GetVideoInfo( );
		
		if( !info ) {
			// This should probably never happen
			throw Exception(std::string("Video query failed: ") + SDL_GetError(), "init", "RoeSDL_GL.cpp", ROE_LINE);
		}
		
		bpp = info->vfmt->BitsPerPixel;
		
		// *SDL_GL (init)*
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE    , 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE  , 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE   , 8 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE  , 16);
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		
		
		//save window size
		m_iWidth  = width;
		m_iHeight = height;
		
		if      (m_iWidth  < s_ciMinWidth)  m_iWidth  = s_ciMinWidth;
		else if (m_iWidth  > s_ciMaxWidth)  m_iWidth  = s_ciMaxWidth;
		if      (m_iHeight < s_ciMinHeight) m_iHeight = s_ciMinHeight;
		else if (m_iHeight > s_ciMaxHeight) m_iHeight = s_ciMaxHeight;
		m_pPixelData = new byte[3*m_iWidth*m_iHeight]; //tmp pixel array for screenshots
		
		m_SDL_Flags |= SDL_OPENGL;
		if (flags & F_FULLSCREEN) m_SDL_Flags |= SDL_FULLSCREEN;
		if (flags & F_NO_FRAME  ) m_SDL_Flags |= SDL_NOFRAME;
		if (flags & F_RESIZABLE ) m_SDL_Flags |= SDL_RESIZABLE;
		
		//setting the window icon
		setIcon(sIcon);
		
		
		//open window
		m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, bpp, m_SDL_Flags );
		if( m_pScreen == NULL ) {
			throw Exception(std::string("Video mode set failed: ") + SDL_GetError(), "init", "RoeSDL_GL.cpp", ROE_LINE);
		}
		SDL_WM_SetCaption(sWindow.c_str(), sTab.c_str());//titel
		
		// **SDL_ttf**
		if (TTF_Init() != 0) {
			throw Exception(std::string("TTF_Init() failed: ") + TTF_GetError(), "init", "RoeSDL_GL.cpp", ROE_LINE);
		}
		
		// **SDL_image**
		//does not need any initialisation
		
		
		// ** OpenGL **
		
		//our shading model--Gouraud (smooth).
		glShadeModel( GL_SMOOTH ); //SMOOTH or FLAT
		
		//culling
		if ((flags & F_CULL_SHOW_CW) || (flags & F_CULL_SHOW_CCW)) {
			glCullFace( GL_BACK ); //BACK or FRONT
			if (flags & F_CULL_SHOW_CW)
				glFrontFace( GL_CW );
			else
				glFrontFace( GL_CCW );
			glEnable( GL_CULL_FACE);
		}

		//activate depth-buffer (z-buffer)
		glEnable(GL_DEPTH_TEST);

		//lighting
		if (flags & F_LIGHTING) {
			glEnable(GL_LIGHTING);
			//glEnable(GL_RESCALE_NORMAL); //for correct normals after (model)scaling
			//wont compile with MINGW!
		}

		//blending
		if (flags & F_ALPHA_BLENDING) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		//fog
		if (flags & F_FOG) {
			glEnable(GL_FOG);
			glFogi  (GL_FOG_MODE, GL_EXP);
			glFogfv (GL_FOG_COLOR, Color(0.5f,0.5f,0.5f,1.0f).rg);
			glFogf  (GL_FOG_DENSITY, 0.0035);
			glHint  (GL_FOG_HINT, GL_DONT_CARE);
			glFogf  (GL_FOG_START, 1.0);
			glFogf  (GL_FOG_END, 5000.0);
		}

		//texturing
		if (flags & F_TEXTURE2D) {
			glEnable (GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //before: decal, std: modulate
			//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true); //mipmaps (dont use it!, its bad!)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR or GL_NEAREST, no mipmap here
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				// GL_NEAREST_MIPMAP_NEAREST or GL_NEAREST_MIPMAP_LINEAR or GL_LINEAR
				//or GL_LINEAR_MIPMAP_NEAREST or GL_LINEAR_MIPMAP_LINEAR or GL_NEAREST 
		}
		
		//vertex-arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		
		// Set the clear color to opaque black
		glClearColor( 0, 0, 0, 1 );
		
		// Setup the viewport and perspective
		setGLViewport(m_iWidth, m_iHeight);
	}
	void SDL_GL::quit() {
		ROE_SAFE_DELETE_ARRAY(m_pPixelData);
		Font::quit();
		Texture::quit();
		TTF_Quit();
		SDL_Quit();
	}
	
	void SDL_GL::saveScreenshot (std::string sPath) {
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, m_iWidth, m_iHeight, GL_RGB, GL_UNSIGNED_BYTE, m_pPixelData);
		
		if (Util::stringEndsWith(sPath,".bmp")) {
			Util::saveImageBMP(sPath, m_pPixelData, (long)m_iWidth, (long)m_iHeight);
		}
		else {
			if (Util::stringEndsWith(sPath,".tga") == false) sPath += ".tga";
			Util::saveImageTGA(sPath, m_pPixelData, (short)m_iWidth, (short)m_iHeight, false);
		}
	}
	
	// *gl*
	void SDL_GL::applyCameraToScene(const Camera& c) {
		//new (without gluLookAt)
		if (c.isDirAbs()) glMultMatrixf(Matrix::camera(c.pos,      c.dir,c.up).n);
		else              glMultMatrixf(Matrix::camera(c.pos,c.pos+c.dir,c.up).n);
	}
	
	void SDL_GL::renderSkyBox(const Vector3& size, const Vector3& center, const std::string& name,
	                          const std::string& up   , const std::string& down ,
	                          const std::string& left , const std::string& right,
	                          const std::string& front, const std::string& back , const Color& color)
	{
		static float extraDistance = 0.0f; //scale factor to prevent cracks between the surfaces
		static Vector3 nonx(1.0,1.0+extraDistance,1.0+extraDistance);
		static Vector3 nony(1.0+extraDistance,1.0,1.0+extraDistance);
		static Vector3 nonz(1.0+extraDistance,1.0+extraDistance,1.0);
		
		Vector3 av[8] = {
			Vector3(-0.5,-0.5, 0.5),
			Vector3( 0.5,-0.5, 0.5),
			Vector3( 0.5, 0.5, 0.5),
			Vector3(-0.5, 0.5, 0.5),
			Vector3(-0.5,-0.5,-0.5),
			Vector3( 0.5,-0.5,-0.5),
			Vector3( 0.5, 0.5,-0.5),
			Vector3(-0.5, 0.5,-0.5)};
		
		for (int i = 0; i < 8; i++) {
			av[i] *= size;
		}
		
		//save attributes and change them
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		
		Texture(name+up).bind(color);
		glBegin(GL_QUADS);
			glNormal3f(0.0f,-1.0f,0.0f);
			glTexCoord2i(0,0); glVertex3fv((av[2]*nony+center).rg);
			glTexCoord2i(1,0); glVertex3fv((av[3]*nony+center).rg);
			glTexCoord2i(1,1); glVertex3fv((av[7]*nony+center).rg);
			glTexCoord2i(0,1); glVertex3fv((av[6]*nony+center).rg);
		glEnd();
		Texture(name+down).bind(color);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f,0.0f);
			glTexCoord2i(0,0); glVertex3fv((av[5]*nony+center).rg);
			glTexCoord2i(1,0); glVertex3fv((av[4]*nony+center).rg);
			glTexCoord2i(1,1); glVertex3fv((av[0]*nony+center).rg);
			glTexCoord2i(0,1); glVertex3fv((av[1]*nony+center).rg);
		glEnd();
		Texture(name+left).bind(color);
		glBegin(GL_QUADS);
			glNormal3f(-1.0f, 0.0f,0.0f);
			glTexCoord2i(0,0); glVertex3fv((av[5]*nonx+center).rg);
			glTexCoord2i(1,0); glVertex3fv((av[1]*nonx+center).rg);
			glTexCoord2i(1,1); glVertex3fv((av[2]*nonx+center).rg);
			glTexCoord2i(0,1); glVertex3fv((av[6]*nonx+center).rg);
		glEnd();
		Texture(name+right).bind(color);
		glBegin(GL_QUADS);
			glNormal3f( 1.0f, 0.0f,0.0f);
			glTexCoord2i(0,0); glVertex3fv((av[0]*nonx+center).rg);
			glTexCoord2i(1,0); glVertex3fv((av[4]*nonx+center).rg);
			glTexCoord2i(1,1); glVertex3fv((av[7]*nonx+center).rg);
			glTexCoord2i(0,1); glVertex3fv((av[3]*nonx+center).rg);
		glEnd();
		Texture(name+front).bind(color);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2i(0,0); glVertex3fv((av[1]*nonz+center).rg);
			glTexCoord2i(1,0); glVertex3fv((av[0]*nonz+center).rg);
			glTexCoord2i(1,1); glVertex3fv((av[3]*nonz+center).rg);
			glTexCoord2i(0,1); glVertex3fv((av[2]*nonz+center).rg);
		glEnd();
		Texture(name+back).bind(color);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f,-1.0f);
			glTexCoord2i(0,0); glVertex3fv((av[4]*nonz+center).rg);
			glTexCoord2i(1,0); glVertex3fv((av[5]*nonz+center).rg);
			glTexCoord2i(1,1); glVertex3fv((av[6]*nonz+center).rg);
			glTexCoord2i(0,1); glVertex3fv((av[7]*nonz+center).rg);
		glEnd();
		
		glPopAttrib(); //reset to old attributes
	}
	
	//INPUT
	void SDL_GL::updateEvents() {
		
		// **events**
		
		while( SDL_PollEvent( &m_Event ) ) { //standart SDL keys
			switch( m_Event.type ) {
				// *keyboard*
				case SDL_KEYDOWN: {
					switch( m_Event.key.keysym.sym ) {
						case SDLK_ESCAPE:     m_aKeyIsDown[SDLK_ESCAPE   ] = true; m_aKeyWasDown[SDLK_ESCAPE   ] = true; break;
						case SDLK_BACKSPACE:  m_aKeyIsDown[SDLK_BACKSPACE] = true; m_aKeyWasDown[SDLK_BACKSPACE] = true; break;
						case SDLK_TAB:        m_aKeyIsDown[SDLK_TAB      ] = true; m_aKeyWasDown[SDLK_TAB      ] = true; break;
						case SDLK_RETURN:     m_aKeyIsDown[SDLK_RETURN   ] = true; m_aKeyWasDown[SDLK_RETURN   ] = true; break;
						case SDLK_SPACE:      m_aKeyIsDown[SDLK_SPACE    ] = true; m_aKeyWasDown[SDLK_SPACE    ] = true; break;
						case SDLK_0:  m_aKeyIsDown[SDLK_0] = true; m_aKeyWasDown[SDLK_0] = true; break;
						case SDLK_1:  m_aKeyIsDown[SDLK_1] = true; m_aKeyWasDown[SDLK_1] = true; break;
						case SDLK_2:  m_aKeyIsDown[SDLK_2] = true; m_aKeyWasDown[SDLK_2] = true; break;
						case SDLK_3:  m_aKeyIsDown[SDLK_3] = true; m_aKeyWasDown[SDLK_3] = true; break;
						case SDLK_4:  m_aKeyIsDown[SDLK_4] = true; m_aKeyWasDown[SDLK_4] = true; break;
						case SDLK_5:  m_aKeyIsDown[SDLK_5] = true; m_aKeyWasDown[SDLK_5] = true; break;
						case SDLK_6:  m_aKeyIsDown[SDLK_6] = true; m_aKeyWasDown[SDLK_6] = true; break;
						case SDLK_7:  m_aKeyIsDown[SDLK_7] = true; m_aKeyWasDown[SDLK_7] = true; break;
						case SDLK_8:  m_aKeyIsDown[SDLK_8] = true; m_aKeyWasDown[SDLK_8] = true; break;
						case SDLK_9:  m_aKeyIsDown[SDLK_9] = true; m_aKeyWasDown[SDLK_9] = true; break;
						case SDLK_a:  m_aKeyIsDown[SDLK_a] = true; m_aKeyWasDown[SDLK_a] = true; break;
						case SDLK_b:  m_aKeyIsDown[SDLK_b] = true; m_aKeyWasDown[SDLK_b] = true; break;
						case SDLK_c:  m_aKeyIsDown[SDLK_c] = true; m_aKeyWasDown[SDLK_c] = true; break;
						case SDLK_d:  m_aKeyIsDown[SDLK_d] = true; m_aKeyWasDown[SDLK_d] = true; break;
						case SDLK_e:  m_aKeyIsDown[SDLK_e] = true; m_aKeyWasDown[SDLK_e] = true; break;
						case SDLK_f:  m_aKeyIsDown[SDLK_f] = true; m_aKeyWasDown[SDLK_f] = true; break;
						case SDLK_g:  m_aKeyIsDown[SDLK_g] = true; m_aKeyWasDown[SDLK_g] = true; break;
						case SDLK_h:  m_aKeyIsDown[SDLK_h] = true; m_aKeyWasDown[SDLK_h] = true; break;
						case SDLK_i:  m_aKeyIsDown[SDLK_i] = true; m_aKeyWasDown[SDLK_i] = true; break;
						case SDLK_j:  m_aKeyIsDown[SDLK_j] = true; m_aKeyWasDown[SDLK_j] = true; break;
						case SDLK_k:  m_aKeyIsDown[SDLK_k] = true; m_aKeyWasDown[SDLK_k] = true; break;
						case SDLK_l:  m_aKeyIsDown[SDLK_l] = true; m_aKeyWasDown[SDLK_l] = true; break;
						case SDLK_m:  m_aKeyIsDown[SDLK_m] = true; m_aKeyWasDown[SDLK_m] = true; break;
						case SDLK_n:  m_aKeyIsDown[SDLK_n] = true; m_aKeyWasDown[SDLK_n] = true; break;
						case SDLK_o:  m_aKeyIsDown[SDLK_o] = true; m_aKeyWasDown[SDLK_o] = true; break;
						case SDLK_p:  m_aKeyIsDown[SDLK_p] = true; m_aKeyWasDown[SDLK_p] = true; break;
						case SDLK_q:  m_aKeyIsDown[SDLK_q] = true; m_aKeyWasDown[SDLK_q] = true; break;
						case SDLK_r:  m_aKeyIsDown[SDLK_r] = true; m_aKeyWasDown[SDLK_r] = true; break;
						case SDLK_s:  m_aKeyIsDown[SDLK_s] = true; m_aKeyWasDown[SDLK_s] = true; break;
						case SDLK_t:  m_aKeyIsDown[SDLK_t] = true; m_aKeyWasDown[SDLK_t] = true; break;
						case SDLK_u:  m_aKeyIsDown[SDLK_u] = true; m_aKeyWasDown[SDLK_u] = true; break;
						case SDLK_v:  m_aKeyIsDown[SDLK_v] = true; m_aKeyWasDown[SDLK_v] = true; break;
						case SDLK_w:  m_aKeyIsDown[SDLK_w] = true; m_aKeyWasDown[SDLK_w] = true; break;
						case SDLK_x:  m_aKeyIsDown[SDLK_x] = true; m_aKeyWasDown[SDLK_x] = true; break;
						case SDLK_y:  m_aKeyIsDown[SDLK_y] = true; m_aKeyWasDown[SDLK_y] = true; break;	//linux: y and swapped
						case SDLK_z:  m_aKeyIsDown[SDLK_z] = true; m_aKeyWasDown[SDLK_z] = true; break;
						case SDLK_DELETE:  m_aKeyIsDown[SDLK_DELETE] = true; m_aKeyWasDown[SDLK_DELETE] = true; break;
						case SDLK_KP0:  m_aKeyIsDown[SDLK_KP0] = true; m_aKeyWasDown[SDLK_KP0] = true; break;
						case SDLK_KP1:  m_aKeyIsDown[SDLK_KP1] = true; m_aKeyWasDown[SDLK_KP1] = true; break;
						case SDLK_KP2:  m_aKeyIsDown[SDLK_KP2] = true; m_aKeyWasDown[SDLK_KP2] = true; break;
						case SDLK_KP3:  m_aKeyIsDown[SDLK_KP3] = true; m_aKeyWasDown[SDLK_KP3] = true; break;
						case SDLK_KP4:  m_aKeyIsDown[SDLK_KP4] = true; m_aKeyWasDown[SDLK_KP4] = true; break;
						case SDLK_KP5:  m_aKeyIsDown[SDLK_KP5] = true; m_aKeyWasDown[SDLK_KP5] = true; break;
						case SDLK_KP6:  m_aKeyIsDown[SDLK_KP6] = true; m_aKeyWasDown[SDLK_KP6] = true; break;
						case SDLK_KP7:  m_aKeyIsDown[SDLK_KP7] = true; m_aKeyWasDown[SDLK_KP7] = true; break;
						case SDLK_KP8:  m_aKeyIsDown[SDLK_KP8] = true; m_aKeyWasDown[SDLK_KP8] = true; break;
						case SDLK_KP9:  m_aKeyIsDown[SDLK_KP9] = true; m_aKeyWasDown[SDLK_KP9] = true; break;
						case SDLK_KP_PERIOD:  m_aKeyIsDown[SDLK_KP_PERIOD  ] = true; m_aKeyWasDown[SDLK_KP_PERIOD  ] = true; break;
						case SDLK_KP_DIVIDE:  m_aKeyIsDown[SDLK_KP_DIVIDE  ] = true; m_aKeyWasDown[SDLK_KP_DIVIDE  ] = true; break;
						case SDLK_KP_MULTIPLY:m_aKeyIsDown[SDLK_KP_MULTIPLY] = true; m_aKeyWasDown[SDLK_KP_MULTIPLY] = true; break;
						case SDLK_KP_MINUS:   m_aKeyIsDown[SDLK_KP_MINUS   ] = true; m_aKeyWasDown[SDLK_KP_MINUS   ] = true; break;
						case SDLK_KP_PLUS:    m_aKeyIsDown[SDLK_KP_PLUS    ] = true; m_aKeyWasDown[SDLK_KP_PLUS    ] = true; break;
						case SDLK_KP_ENTER:   m_aKeyIsDown[SDLK_KP_ENTER   ] = true; m_aKeyWasDown[SDLK_KP_ENTER   ] = true; break;
						case SDLK_UP:      m_aKeyIsDown[SDLK_UP      ] = true; m_aKeyWasDown[SDLK_UP      ] = true; break;
						case SDLK_DOWN:    m_aKeyIsDown[SDLK_DOWN    ] = true; m_aKeyWasDown[SDLK_DOWN    ] = true; break;
						case SDLK_RIGHT:   m_aKeyIsDown[SDLK_RIGHT   ] = true; m_aKeyWasDown[SDLK_RIGHT   ] = true; break;
						case SDLK_LEFT:    m_aKeyIsDown[SDLK_LEFT    ] = true; m_aKeyWasDown[SDLK_LEFT    ] = true; break;
						case SDLK_INSERT:  m_aKeyIsDown[SDLK_INSERT  ] = true; m_aKeyWasDown[SDLK_INSERT  ] = true; break;
						case SDLK_HOME:    m_aKeyIsDown[SDLK_HOME    ] = true; m_aKeyWasDown[SDLK_HOME    ] = true; break;
						case SDLK_END:     m_aKeyIsDown[SDLK_END     ] = true; m_aKeyWasDown[SDLK_END     ] = true; break;
						case SDLK_PAGEUP:  m_aKeyIsDown[SDLK_PAGEUP  ] = true; m_aKeyWasDown[SDLK_PAGEUP  ] = true; break;
						case SDLK_PAGEDOWN:m_aKeyIsDown[SDLK_PAGEDOWN] = true; m_aKeyWasDown[SDLK_PAGEDOWN] = true; break;
						case SDLK_F1:   m_aKeyIsDown[SDLK_F1 ] = true; m_aKeyWasDown[SDLK_F1 ] = true; break;
						case SDLK_F2:   m_aKeyIsDown[SDLK_F2 ] = true; m_aKeyWasDown[SDLK_F2 ] = true; break;
						case SDLK_F3:   m_aKeyIsDown[SDLK_F3 ] = true; m_aKeyWasDown[SDLK_F3 ] = true; break;
						case SDLK_F4:   m_aKeyIsDown[SDLK_F4 ] = true; m_aKeyWasDown[SDLK_F4 ] = true; break;
						case SDLK_F5:   m_aKeyIsDown[SDLK_F5 ] = true; m_aKeyWasDown[SDLK_F5 ] = true; break;
						case SDLK_F6:   m_aKeyIsDown[SDLK_F6 ] = true; m_aKeyWasDown[SDLK_F6 ] = true; break;
						case SDLK_F7:   m_aKeyIsDown[SDLK_F7 ] = true; m_aKeyWasDown[SDLK_F7 ] = true; break;
						case SDLK_F8:   m_aKeyIsDown[SDLK_F8 ] = true; m_aKeyWasDown[SDLK_F8 ] = true; break;
						case SDLK_F9:   m_aKeyIsDown[SDLK_F9 ] = true; m_aKeyWasDown[SDLK_F9 ] = true; break;
						case SDLK_F10:  m_aKeyIsDown[SDLK_F10] = true; m_aKeyWasDown[SDLK_F10] = true; break;
						case SDLK_F11:  m_aKeyIsDown[SDLK_F11] = true; m_aKeyWasDown[SDLK_F11] = true; break;
						case SDLK_F12:  m_aKeyIsDown[SDLK_F12] = true; m_aKeyWasDown[SDLK_F12] = true; break;
						case SDLK_F13:  m_aKeyIsDown[SDLK_F13] = true; m_aKeyWasDown[SDLK_F13] = true; break;
						case SDLK_F14:  m_aKeyIsDown[SDLK_F14] = true; m_aKeyWasDown[SDLK_F14] = true; break;
						case SDLK_F15:  m_aKeyIsDown[SDLK_F15] = true; m_aKeyWasDown[SDLK_F15] = true; break;
						case SDLK_CAPSLOCK:m_aKeyIsDown[SDLK_CAPSLOCK] = true; m_aKeyWasDown[SDLK_CAPSLOCK] = true; break;
						case SDLK_RSHIFT:  m_aKeyIsDown[SDLK_RSHIFT  ] = true; m_aKeyWasDown[SDLK_RSHIFT  ] = true; break;
						case SDLK_LSHIFT:  m_aKeyIsDown[SDLK_LSHIFT  ] = true; m_aKeyWasDown[SDLK_LSHIFT  ] = true; break;
						case SDLK_RCTRL:   m_aKeyIsDown[SDLK_RCTRL   ] = true; m_aKeyWasDown[SDLK_RCTRL   ] = true; break;
						case SDLK_LCTRL:   m_aKeyIsDown[SDLK_LCTRL   ] = true; m_aKeyWasDown[SDLK_LCTRL   ] = true; break;
						case SDLK_RALT:    m_aKeyIsDown[SDLK_RALT    ] = true; m_aKeyWasDown[SDLK_RALT    ] = true; break;
						case SDLK_LALT:    m_aKeyIsDown[SDLK_LALT    ] = true; m_aKeyWasDown[SDLK_LALT    ] = true; break;
						case SDLK_LSUPER:  m_aKeyIsDown[SDLK_LSUPER  ] = true; m_aKeyWasDown[SDLK_LSUPER  ] = true; break;  // Left "Windows" key
						case SDLK_RSUPER:  m_aKeyIsDown[SDLK_RSUPER  ] = true; m_aKeyWasDown[SDLK_RSUPER  ] = true; break;  // Right "Windows" key 
						case SDLK_MODE:    m_aKeyIsDown[SDLK_MODE    ] = true; m_aKeyWasDown[SDLK_MODE    ] = true; break;  // "Alt Gr" key
						default: break;
					}
				} break;
				case SDL_KEYUP: {
					switch( m_Event.key.keysym.sym ) {
						case SDLK_ESCAPE:    m_aKeyIsDown[SDLK_ESCAPE   ] = false; m_aKeyWasUp[SDLK_ESCAPE   ] = true; break;
						case SDLK_BACKSPACE: m_aKeyIsDown[SDLK_BACKSPACE] = false; m_aKeyWasUp[SDLK_BACKSPACE] = true; break;
						case SDLK_TAB:       m_aKeyIsDown[SDLK_TAB      ] = false; m_aKeyWasUp[SDLK_TAB      ] = true; break;
						case SDLK_RETURN:    m_aKeyIsDown[SDLK_RETURN   ] = false; m_aKeyWasUp[SDLK_RETURN   ] = true; break;
						case SDLK_SPACE:     m_aKeyIsDown[SDLK_SPACE    ] = false; m_aKeyWasUp[SDLK_SPACE    ] = true; break;
						case SDLK_0:  m_aKeyIsDown[SDLK_0] = false; m_aKeyWasUp[SDLK_0] = true; break;
						case SDLK_1:  m_aKeyIsDown[SDLK_1] = false; m_aKeyWasUp[SDLK_1] = true; break;
						case SDLK_2:  m_aKeyIsDown[SDLK_2] = false; m_aKeyWasUp[SDLK_2] = true; break;
						case SDLK_3:  m_aKeyIsDown[SDLK_3] = false; m_aKeyWasUp[SDLK_3] = true; break;
						case SDLK_4:  m_aKeyIsDown[SDLK_4] = false; m_aKeyWasUp[SDLK_4] = true; break;
						case SDLK_5:  m_aKeyIsDown[SDLK_5] = false; m_aKeyWasUp[SDLK_5] = true; break;
						case SDLK_6:  m_aKeyIsDown[SDLK_6] = false; m_aKeyWasUp[SDLK_6] = true; break;
						case SDLK_7:  m_aKeyIsDown[SDLK_7] = false; m_aKeyWasUp[SDLK_7] = true; break;
						case SDLK_8:  m_aKeyIsDown[SDLK_8] = false; m_aKeyWasUp[SDLK_8] = true; break;
						case SDLK_9:  m_aKeyIsDown[SDLK_9] = false; m_aKeyWasUp[SDLK_9] = true; break;
						case SDLK_a:  m_aKeyIsDown[SDLK_a] = false; m_aKeyWasUp[SDLK_a] = true; break;
						case SDLK_b:  m_aKeyIsDown[SDLK_b] = false; m_aKeyWasUp[SDLK_b] = true; break;
						case SDLK_c:  m_aKeyIsDown[SDLK_c] = false; m_aKeyWasUp[SDLK_c] = true; break;
						case SDLK_d:  m_aKeyIsDown[SDLK_d] = false; m_aKeyWasUp[SDLK_d] = true; break;
						case SDLK_e:  m_aKeyIsDown[SDLK_e] = false; m_aKeyWasUp[SDLK_e] = true; break;
						case SDLK_f:  m_aKeyIsDown[SDLK_f] = false; m_aKeyWasUp[SDLK_f] = true; break;
						case SDLK_g:  m_aKeyIsDown[SDLK_g] = false; m_aKeyWasUp[SDLK_g] = true; break;
						case SDLK_h:  m_aKeyIsDown[SDLK_h] = false; m_aKeyWasUp[SDLK_h] = true; break;
						case SDLK_i:  m_aKeyIsDown[SDLK_i] = false; m_aKeyWasUp[SDLK_i] = true; break;
						case SDLK_j:  m_aKeyIsDown[SDLK_j] = false; m_aKeyWasUp[SDLK_j] = true; break;
						case SDLK_k:  m_aKeyIsDown[SDLK_k] = false; m_aKeyWasUp[SDLK_k] = true; break;
						case SDLK_l:  m_aKeyIsDown[SDLK_l] = false; m_aKeyWasUp[SDLK_l] = true; break;
						case SDLK_m:  m_aKeyIsDown[SDLK_m] = false; m_aKeyWasUp[SDLK_m] = true; break;
						case SDLK_n:  m_aKeyIsDown[SDLK_n] = false; m_aKeyWasUp[SDLK_n] = true; break;
						case SDLK_o:  m_aKeyIsDown[SDLK_o] = false; m_aKeyWasUp[SDLK_o] = true; break;
						case SDLK_p:  m_aKeyIsDown[SDLK_p] = false; m_aKeyWasUp[SDLK_p] = true; break;
						case SDLK_q:  m_aKeyIsDown[SDLK_q] = false; m_aKeyWasUp[SDLK_q] = true; break;
						case SDLK_r:  m_aKeyIsDown[SDLK_r] = false; m_aKeyWasUp[SDLK_r] = true; break;
						case SDLK_s:  m_aKeyIsDown[SDLK_s] = false; m_aKeyWasUp[SDLK_s] = true; break;
						case SDLK_t:  m_aKeyIsDown[SDLK_t] = false; m_aKeyWasUp[SDLK_t] = true; break;
						case SDLK_u:  m_aKeyIsDown[SDLK_u] = false; m_aKeyWasUp[SDLK_u] = true; break;
						case SDLK_v:  m_aKeyIsDown[SDLK_v] = false; m_aKeyWasUp[SDLK_v] = true; break;
						case SDLK_w:  m_aKeyIsDown[SDLK_w] = false; m_aKeyWasUp[SDLK_w] = true; break;
						case SDLK_x:  m_aKeyIsDown[SDLK_x] = false; m_aKeyWasUp[SDLK_x] = true; break;
						case SDLK_y:  m_aKeyIsDown[SDLK_y] = false; m_aKeyWasUp[SDLK_y] = true; break;	//linux: y und z swapped
						case SDLK_z:  m_aKeyIsDown[SDLK_z] = false; m_aKeyWasUp[SDLK_z] = true; break;
						case SDLK_DELETE:  m_aKeyIsDown[SDLK_DELETE] = false; m_aKeyWasUp[SDLK_DELETE] = true; break;
						case SDLK_KP0:  m_aKeyIsDown[SDLK_KP0] = false; m_aKeyWasUp[SDLK_KP0] = true; break;
						case SDLK_KP1:  m_aKeyIsDown[SDLK_KP1] = false; m_aKeyWasUp[SDLK_KP1] = true; break;
						case SDLK_KP2:  m_aKeyIsDown[SDLK_KP2] = false; m_aKeyWasUp[SDLK_KP2] = true; break;
						case SDLK_KP3:  m_aKeyIsDown[SDLK_KP3] = false; m_aKeyWasUp[SDLK_KP3] = true; break;
						case SDLK_KP4:  m_aKeyIsDown[SDLK_KP4] = false; m_aKeyWasUp[SDLK_KP4] = true; break;
						case SDLK_KP5:  m_aKeyIsDown[SDLK_KP5] = false; m_aKeyWasUp[SDLK_KP5] = true; break;
						case SDLK_KP6:  m_aKeyIsDown[SDLK_KP6] = false; m_aKeyWasUp[SDLK_KP6] = true; break;
						case SDLK_KP7:  m_aKeyIsDown[SDLK_KP7] = false; m_aKeyWasUp[SDLK_KP7] = true; break;
						case SDLK_KP8:  m_aKeyIsDown[SDLK_KP8] = false; m_aKeyWasUp[SDLK_KP8] = true; break;
						case SDLK_KP9:  m_aKeyIsDown[SDLK_KP9] = false; m_aKeyWasUp[SDLK_KP9] = true; break;
						case SDLK_KP_PERIOD:  m_aKeyIsDown[SDLK_KP_PERIOD  ] = false; m_aKeyWasUp[SDLK_KP_PERIOD  ] = true; break;
						case SDLK_KP_DIVIDE:  m_aKeyIsDown[SDLK_KP_DIVIDE  ] = false; m_aKeyWasUp[SDLK_KP_DIVIDE  ] = true; break;
						case SDLK_KP_MULTIPLY:m_aKeyIsDown[SDLK_KP_MULTIPLY] = false; m_aKeyWasUp[SDLK_KP_MULTIPLY] = true; break;
						case SDLK_KP_MINUS:   m_aKeyIsDown[SDLK_KP_MINUS   ] = false; m_aKeyWasUp[SDLK_KP_MINUS   ] = true; break;
						case SDLK_KP_PLUS:    m_aKeyIsDown[SDLK_KP_PLUS    ] = false; m_aKeyWasUp[SDLK_KP_PLUS    ] = true; break;
						case SDLK_KP_ENTER:   m_aKeyIsDown[SDLK_KP_ENTER   ] = false; m_aKeyWasUp[SDLK_KP_ENTER   ] = true; break;
						case SDLK_UP:      m_aKeyIsDown[SDLK_UP      ] = false; m_aKeyWasUp[SDLK_UP      ] = true; break;
						case SDLK_DOWN:    m_aKeyIsDown[SDLK_DOWN    ] = false; m_aKeyWasUp[SDLK_DOWN    ] = true; break;
						case SDLK_RIGHT:   m_aKeyIsDown[SDLK_RIGHT   ] = false; m_aKeyWasUp[SDLK_RIGHT   ] = true; break;
						case SDLK_LEFT:    m_aKeyIsDown[SDLK_LEFT    ] = false; m_aKeyWasUp[SDLK_LEFT    ] = true; break;
						case SDLK_INSERT:  m_aKeyIsDown[SDLK_INSERT  ] = false; m_aKeyWasUp[SDLK_INSERT  ] = true; break;
						case SDLK_HOME:    m_aKeyIsDown[SDLK_HOME    ] = false; m_aKeyWasUp[SDLK_HOME    ] = true; break;
						case SDLK_END:     m_aKeyIsDown[SDLK_END     ] = false; m_aKeyWasUp[SDLK_END     ] = true; break;
						case SDLK_PAGEUP:  m_aKeyIsDown[SDLK_PAGEUP  ] = false; m_aKeyWasUp[SDLK_PAGEUP  ] = true; break;
						case SDLK_PAGEDOWN:m_aKeyIsDown[SDLK_PAGEDOWN] = false; m_aKeyWasUp[SDLK_PAGEDOWN] = true; break;
						case SDLK_F1:   m_aKeyIsDown[SDLK_F1 ] = false; m_aKeyWasUp[SDLK_F1 ] = true; break;
						case SDLK_F2:   m_aKeyIsDown[SDLK_F2 ] = false; m_aKeyWasUp[SDLK_F2 ] = true; break;
						case SDLK_F3:   m_aKeyIsDown[SDLK_F3 ] = false; m_aKeyWasUp[SDLK_F3 ] = true; break;
						case SDLK_F4:   m_aKeyIsDown[SDLK_F4 ] = false; m_aKeyWasUp[SDLK_F4 ] = true; break;
						case SDLK_F5:   m_aKeyIsDown[SDLK_F5 ] = false; m_aKeyWasUp[SDLK_F5 ] = true; break;
						case SDLK_F6:   m_aKeyIsDown[SDLK_F6 ] = false; m_aKeyWasUp[SDLK_F6 ] = true; break;
						case SDLK_F7:   m_aKeyIsDown[SDLK_F7 ] = false; m_aKeyWasUp[SDLK_F7 ] = true; break;
						case SDLK_F8:   m_aKeyIsDown[SDLK_F8 ] = false; m_aKeyWasUp[SDLK_F8 ] = true; break;
						case SDLK_F9:   m_aKeyIsDown[SDLK_F9 ] = false; m_aKeyWasUp[SDLK_F9 ] = true; break;
						case SDLK_F10:  m_aKeyIsDown[SDLK_F10] = false; m_aKeyWasUp[SDLK_F10] = true; break;
						case SDLK_F11:  m_aKeyIsDown[SDLK_F11] = false; m_aKeyWasUp[SDLK_F11] = true; break;
						case SDLK_F12:  m_aKeyIsDown[SDLK_F12] = false; m_aKeyWasUp[SDLK_F12] = true; break;
						case SDLK_F13:  m_aKeyIsDown[SDLK_F13] = false; m_aKeyWasUp[SDLK_F13] = true; break;
						case SDLK_F14:  m_aKeyIsDown[SDLK_F14] = false; m_aKeyWasUp[SDLK_F14] = true; break;
						case SDLK_F15:  m_aKeyIsDown[SDLK_F15] = false; m_aKeyWasUp[SDLK_F15] = true; break;
						case SDLK_CAPSLOCK:m_aKeyIsDown[SDLK_CAPSLOCK] = false; m_aKeyWasUp[SDLK_CAPSLOCK] = true; break;
						case SDLK_RSHIFT:  m_aKeyIsDown[SDLK_RSHIFT  ] = false; m_aKeyWasUp[SDLK_RSHIFT  ] = true; break;
						case SDLK_LSHIFT:  m_aKeyIsDown[SDLK_LSHIFT  ] = false; m_aKeyWasUp[SDLK_LSHIFT  ] = true; break;
						case SDLK_RCTRL:   m_aKeyIsDown[SDLK_RCTRL   ] = false; m_aKeyWasUp[SDLK_RCTRL   ] = true; break;
						case SDLK_LCTRL:   m_aKeyIsDown[SDLK_LCTRL   ] = false; m_aKeyWasUp[SDLK_LCTRL   ] = true; break;
						case SDLK_RALT:    m_aKeyIsDown[SDLK_RALT    ] = false; m_aKeyWasUp[SDLK_RALT    ] = true; break;
						case SDLK_LALT:    m_aKeyIsDown[SDLK_LALT    ] = false; m_aKeyWasUp[SDLK_LALT    ] = true; break;
						case SDLK_LSUPER:  m_aKeyIsDown[SDLK_LSUPER  ] = false; m_aKeyWasUp[SDLK_LSUPER  ] = true; break;    // Left "Windows" key
						case SDLK_RSUPER:  m_aKeyIsDown[SDLK_RSUPER  ] = false; m_aKeyWasUp[SDLK_RSUPER  ] = true; break;    // Right "Windows" key 
						case SDLK_MODE:    m_aKeyIsDown[SDLK_MODE    ] = false; m_aKeyWasUp[SDLK_MODE    ] = true; break;    // "Alt Gr" key
						default: break;
					}
				} break;
				
				
				// *mouse clicks*
				case SDL_QUIT: { //quit-button
					m_bMouseQuit = true;
				} break;
				case SDL_MOUSEBUTTONDOWN: {
					//double click?
					if(m_MouseClickTimer.expired()) { //value: 0.15 - 0.25; std: 0.2
						m_MouseClickTimer.reset();
						m_bMouseDoubleClick = true;
					}
					
					switch(m_Event.button.button) {
						case SDL_BUTTON_LEFT:      m_aMouseIsDown[SDL_BUTTON_LEFT     ] = true; m_aMouseWasDown[SDL_BUTTON_LEFT     ] = true; break;
						case SDL_BUTTON_MIDDLE:    m_aMouseIsDown[SDL_BUTTON_MIDDLE   ] = true; m_aMouseWasDown[SDL_BUTTON_MIDDLE   ] = true; break;
						case SDL_BUTTON_RIGHT:     m_aMouseIsDown[SDL_BUTTON_RIGHT    ] = true; m_aMouseWasDown[SDL_BUTTON_RIGHT    ] = true; break;
						case SDL_BUTTON_WHEELUP:   m_aMouseIsDown[SDL_BUTTON_WHEELUP  ] = true; m_aMouseWasDown[SDL_BUTTON_WHEELUP  ] = true; break;
						case SDL_BUTTON_WHEELDOWN: m_aMouseIsDown[SDL_BUTTON_WHEELDOWN] = true; m_aMouseWasDown[SDL_BUTTON_WHEELDOWN] = true; break;
						default: break;
					}
				} break;
				case SDL_MOUSEBUTTONUP: {
					switch(m_Event.button.button) {
						case SDL_BUTTON_LEFT:      m_aMouseIsDown[SDL_BUTTON_LEFT     ] = false; m_aMouseWasUp[SDL_BUTTON_LEFT     ] = true; break;
						case SDL_BUTTON_MIDDLE:    m_aMouseIsDown[SDL_BUTTON_MIDDLE   ] = false; m_aMouseWasUp[SDL_BUTTON_MIDDLE   ] = true; break;
						case SDL_BUTTON_RIGHT:     m_aMouseIsDown[SDL_BUTTON_RIGHT    ] = false; m_aMouseWasUp[SDL_BUTTON_RIGHT    ] = true; break;
						case SDL_BUTTON_WHEELUP:   m_aMouseIsDown[SDL_BUTTON_WHEELUP  ] = false; m_aMouseWasUp[SDL_BUTTON_WHEELUP  ] = true; break;
						case SDL_BUTTON_WHEELDOWN: m_aMouseIsDown[SDL_BUTTON_WHEELDOWN] = false; m_aMouseWasUp[SDL_BUTTON_WHEELDOWN] = true; break;
						default: break;
					}
				} break;
				
				//resizing itself works automatically
				case SDL_VIDEORESIZE: { //resizing
					resizeWindow(m_Event.resize.w, m_Event.resize.h);
				} break;
				
				default: break;
			}
		}
		
		//adjust impossible event (past events)
		for (int i = 0; i < SDLK_LAST; i++) {
			if (m_aKeyIsDown[i] == true  && m_aKeyWasUp  [i] == true) m_aKeyWasUp  [i] = false;
			if (m_aKeyIsDown[i] == false && m_aKeyWasDown[i] == true) m_aKeyWasDown[i] = false;
		}
		for (int i = 0; i< 10; i++) {
			if (m_aMouseIsDown[i] == true  && m_aMouseWasUp  [i] == true) m_aMouseWasUp  [i] = false;
			if (m_aMouseIsDown[i] == false && m_aMouseWasDown[i] == true) m_aMouseWasDown[i] = false;
		}
		
		//mouse position update
		m_iMouseXPos = m_Event.motion.x;//X mouse update
		m_iMouseYPos = m_Event.motion.y;//Y mouse update
		m_fMouseXPos = (m_iMouseXPos - m_iWidth  / 2.0f) /  (m_iWidth  / 2.0f);
		m_fMouseYPos = (m_iMouseYPos - m_iHeight / 2.0f) / -(m_iHeight / 2.0f);
	}
	
	void SDL_GL::grabInput(bool enable) {
		if (enable) SDL_WM_GrabInput(SDL_GRAB_ON);
		else        SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
	void SDL_GL::resetKeyEvents (bool isDown, bool wasDown, bool wasUp) {
		for (int i = 0; i < SDLK_LAST; i++) {
			if (isDown ) m_aKeyIsDown [i] = false;
			if (wasDown) m_aKeyWasDown[i] = false;
			if (wasUp  ) m_aKeyWasUp  [i] = false;
		}
	}
	void SDL_GL::resetMouseEvents (bool isDown, bool wasDown, bool wasUp) {
		for (int i = 0; i < 10; i++) {
			if (isDown ) m_aMouseIsDown [i] = false;
			if (wasDown) m_aMouseWasDown[i] = false;
			if (wasUp  ) m_aMouseWasUp  [i] = false;
		}
	}
	
	//setting the cursors position
	void SDL_GL::setMouseXPosi (int x) {
		Util::clamp(x, 0, m_iWidth -1);
		SDL_WarpMouse(x, m_iMouseYPos);
		m_fMouseXPos = (x-m_iWidth /2.0f)/  (m_iWidth /2.0f);
	}
	void SDL_GL::setMouseYPosi (int y) {
		Util::clamp(y, 0, m_iHeight-1);
		SDL_WarpMouse(m_iMouseXPos, y);
		m_fMouseYPos = (y-m_iHeight/2.0f)/ -(m_iHeight/2.0f);
	}
	void SDL_GL::setMousePosi  (int x, int y) {
		Util::clamp(x, 0, m_iWidth -1);
		Util::clamp(y, 0, m_iHeight-1);
		SDL_WarpMouse(x, y);
		m_fMouseXPos = (x-m_iWidth /2.0f)/  (m_iWidth /2.0f);
		m_fMouseYPos = (y-m_iHeight/2.0f)/ -(m_iHeight/2.0f);
	}
	void SDL_GL::setMousePosf  (Vector2 xy)   {setMousePosi(xy.x*0.5f*m_iWidth+0.5f*m_iWidth, xy.y*(-0.5f)*m_iHeight+0.5f*m_iHeight);}
	
	//toggle fullscreen/windowed mode
	void SDL_GL::toggleFullscreen() {
		int w = m_iWidth, h = m_iHeight, bitspp = m_pScreen->format->BitsPerPixel;
		unsigned long flags = m_pScreen->flags; //Save the current flags in case toggling fails
		if (m_SDL_Flags & SDL_NOFRAME) {
			m_pScreen = SDL_SetVideoMode(w, h, bitspp, flags ^ SDL_FULLSCREEN); //Toggles FullScreen Mode
		} else { //with frame
			m_pScreen = SDL_SetVideoMode(w, h, bitspp, (flags ^ SDL_FULLSCREEN) & (~SDL_NOFRAME)); //Toggles FullScreen Mode
		}
		if(m_pScreen == NULL) m_pScreen = SDL_SetVideoMode(w, h, bitspp, flags); //If toggle FullScreen failed, then switch back
		if(m_pScreen == NULL) throw Exception("toggle to/from fullscreen failed", "toggleFullscreen", "RoeSDL_GL.cpp", ROE_LINE);
	}
	//resizes window to new size
	void SDL_GL::resizeWindow(int w, int h) {
		m_iWidth  = w;
		m_iHeight = h;
		if      (m_iWidth  < s_ciMinWidth)  m_iWidth  = s_ciMinWidth;
		else if (m_iWidth  > s_ciMaxWidth)  m_iWidth  = s_ciMaxWidth;
		if      (m_iHeight < s_ciMinHeight) m_iHeight = s_ciMinHeight;
		else if (m_iHeight > s_ciMaxHeight) m_iHeight = s_ciMaxHeight;
		ROE_SAFE_DELETE(m_pPixelData);
		m_pPixelData = new byte[3*m_iWidth*m_iHeight]; //tmp pixel array for screenshots
		m_pScreen = SDL_SetVideoMode(m_iWidth, m_iHeight, m_pScreen->format->BitsPerPixel, m_pScreen->flags); //new window
		setGLViewport(m_iWidth, m_iHeight);
	}
	
	// Setup the viewport and perspective
	void SDL_GL::setGLViewport(int w, int h) {
		glViewport( 0, 0, w, h);
		
		double ratio = static_cast<double>(m_iWidth) / static_cast<double>(m_iHeight);
		double top, bottom, left, right;
		double zNear = 0.1, zFar = 16777216.0, fovy = 60.0; //you may change this values (std: 1.0, 65536.0, 60.0)
		
		top = zNear * tan((ROE_PI/180.0)*(fovy/2));
		bottom = -top;
		right = ratio*top;
		left = -right;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(left, right, bottom, top, zNear, zFar); //new (without gluPerspective)
		glMatrixMode(GL_MODELVIEW);
	}
	
	//black/white filter
	void SDL_GL::applyBlackWhiteFilter() {
		const int w = m_iWidth, h = m_iHeight;
		const int bpp = 3;
		Color c;
		float brightness;
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, m_pPixelData);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				c.r = m_pPixelData[i*w*bpp+j*bpp+0]/256.0f;
				c.g = m_pPixelData[i*w*bpp+j*bpp+1]/256.0f;
				c.b = m_pPixelData[i*w*bpp+j*bpp+2]/256.0f;
				brightness = c.brightness();
				c.r = c.g = c.b = brightness;
				m_pPixelData[i*w*bpp+j*bpp+0] = (byte)(c.r * 256.0f);
				m_pPixelData[i*w*bpp+j*bpp+1] = (byte)(c.g * 256.0f);
				m_pPixelData[i*w*bpp+j*bpp+2] = (byte)(c.b * 256.0f);
			}
		}
		glDrawPixels(w,h, GL_RGB, GL_UNSIGNED_BYTE, m_pPixelData);
	}
	
	//setting the icon
	void SDL_GL::setIcon(std::string sIcon) {
		if (sIcon.length() > 3) {
			if (Util::stringEndsWith(sIcon, ".bmp")) {
				SDL_WM_SetIcon(SDL_LoadBMP(sIcon.c_str()), NULL);
			} else if (Util::stringEndsWith(sIcon, ".tga")) {
				BinFile bf;
				bf.openIn(sIcon);
				byte  in1;
				byte2 in2;
				for (int i = 0; i < 12; i++) {bf >> in1;} //move to interesting part
				bf >> in2;
				int w = (int)in2;
				bf >> in2;
				int h = (int)in2;
				bf >> in1;
				if ((int)in1 % 8 != 0 || (int)in1 < 24 || (int)in1 > 32) {
					throw Exception("tga icon '" + sIcon + "' has bad bits per pixel: " + Util::toString((int)in1),
					                "setIcon", "RoeSDL_GL.cpp", ROE_LINE);
				}
				int bpp = (int)in1 / 8; //bytes per pixel
				bf >> in1;
				byte  *data  = new byte[w*h*3];
				byte  *alpha = new byte[w*h];
				for (int i = h-1; i >= 0; i--) { //flipped!
					for (int j = 0; j < w; j++) {
						bf >> in1; data[i*w*3+j*3+2] = in1;
						bf >> in1; data[i*w*3+j*3+1] = in1;
						bf >> in1; data[i*w*3+j*3+0] = in1;
						if (bpp == 4)
							bf >> in1; alpha[i*w+j] = (in1<20)?0:1;
					}
				}
				byte *alpha2 = new byte[w*h/8];
				for (int i = 0; i < w*h/8; i++) {alpha2[i] = 0;}
				for (int i = 0; i < w*h; i++) {
					alpha2[i/8] |= (alpha[i]<<(i%8));
				}
				SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(data, w, h, 24, w*3, 0x0000ff, 0x00ff00, 0xff0000, 0);
				SDL_WM_SetIcon(surface, alpha2);
				SDL_FreeSurface(surface);
				delete[] alpha;
				delete[] alpha2;
				delete[] data;
				bf.closeIn();
			}
		}
	}
	
} // namespace Roe


