#include "glcSDL_GL.h"

glcSDL_GL::glcSDL_GL() :
	vCam1(0.0f,0.0f,20.0f), vCam2(0.0f,0.0f,0.0f), vCam3(0.0f,1.0f,0.0f)
{
	//window
	m_iAllW = m_iAllH = -1;
	m_pScreen = NULL;
	
	
	//tasten auf false setzen
	for (int i = 0; i < SDLK_LAST; i++)
		m_aKeyIsDown[i] = m_aKeyWasDown[i] = m_aKeyWasUp[i] = false;
	for (int i = 0; i < 10; i++)
		m_aMouseIsDown[i] = m_aMouseWasDown[i] = m_aMouseWasUp[i] = false;
	m_bMouseQuit = false;
	m_bMouseDoubleClick = false;
}

glcSDL_GL::~glcSDL_GL()
{
}

void glcSDL_GL::Init(int width, int height, flag8 GLflags, int SDLflags, string sWindow, string sTab) {
	if (m_iAllW != -1) return; // schon initialisiert
	
	
/** SDL **/
	/* Information about the current video settings. */
    const SDL_VideoInfo* info = NULL;
    /* Color depth in bits of our window. */
    int bpp = 0;

    /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        Quit();
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );

    if( !info ) {
        /* This should probably never happen. */
       fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
       Quit();
    }
     
    bpp = info->vfmt->BitsPerPixel;

	/* SDL_GL (init) */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    
    
	if (width < 400) width = 400; else if (width > 1600) width = 1600;
	if (height < 300) height = 300; else if (height > 1024) height = 1024; 
	if (SDLflags & SDL_OPENGL) {} else SDLflags |= SDL_OPENGL;
	
	//windowdaten speichern
	m_iAllW = width;
	m_iAllH = height;
	
	//window oeffnen
	m_pScreen = SDL_SetVideoMode( width, height, bpp, SDLflags );
    if( m_pScreen == NULL ) {
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
        Quit();
    }
	SDL_WM_SetCaption(sWindow.data(), sTab.data());//titel
	
	//timer
	m_MouseClickTimer.Update();
/**SDL_ttf**/

	TTF_Init();

/**SDL_image**/

/**SDL_mixer**/
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512); //nur bei music: 4096; ansonsten 1024 oder kleiner	(std: 512)

/** OpenGL **/
	float ratio = (float) width / (float) height;
	
	//Our shading model--Gouraud (smooth).
	glShadeModel( GL_SMOOTH );	//SMOOTH or FLAT
	
	// Culling
	if (GLflags & GLC_CULLING) {
		glCullFace( GL_BACK );		//BACK or FRONT
		glFrontFace( GL_CCW );		//CCW or CW
		glEnable( GL_CULL_FACE);
	}	
	
	//depth-buffer (z-buffer) aktivieren
	glEnable(GL_DEPTH_TEST); 
	
	//lighting
	if (GLflags & GLC_LIGHTING) {
		glEnable(GL_LIGHTING);
	}
	
	//blending
	if (GLflags & GLC_BLEND) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	//fog
	if (GLflags & GLC_FOG) {
		glEnable(GL_FOG);
		glFogi (GL_FOG_MODE, GL_EXP);
		glFogfv (GL_FOG_COLOR, tbColor(0.5f,0.5f,0.5f,1.0f).c);
		glFogf (GL_FOG_DENSITY, 0.0035);
		glHint (GL_FOG_HINT, GL_DONT_CARE);
		glFogf (GL_FOG_START, 1.0);
		glFogf (GL_FOG_END, 5000.0);
	}
	
	//texturing
	if (GLflags & GLC_TEXTURE2D) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //vorher: decal
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
			GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
			GL_NEAREST);
	}
		
	
	
	//vertex-arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// Set the clear color.
	glClearColor( 0, 0, 0, 0 );
	
	// Setup the viewport.
	glViewport( 0, 0, width, height );
	
	/*
	 * Change to the projection matrix and set
	 * our viewing volume.
	 */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	/*
	 * EXERCISE:
	 * Replace this with a call to glFrustum.
	 */
	gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}
void glcSDL_GL::Quit() {
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}



void glcSDL_GL::Update() {
	
	/**events**/
	
	m_iMouseXPos = event.motion.x;//XMausposition updaten
	m_iMouseYPos = event.motion.y;//YMausposition updaten
	m_fMouseXPos = (m_iMouseXPos - m_iAllW / 2.0f) / (m_iAllH / 2.0f);			//TODO: letztes m_iAllH vielleicht m_iAllW
	m_fMouseYPos = (m_iMouseYPos - m_iAllH / 2.0f) * -1.0f / (m_iAllH / 2.0f);
	
	while( SDL_PollEvent( &event ) ) { //tastaturbelegung: standart SDL
		switch( event.type ) {
			/*keyboard*/
			case SDL_KEYDOWN: {
				switch( event.key.keysym.sym ) { 
					case SDLK_ESCAPE: 		m_aKeyIsDown[SDLK_ESCAPE] = true; m_aKeyWasDown[SDLK_ESCAPE] = true; break;
					case SDLK_BACKSPACE: 	m_aKeyIsDown[SDLK_BACKSPACE] = true; m_aKeyWasDown[SDLK_BACKSPACE] = true; break;
					case SDLK_TAB: 			m_aKeyIsDown[SDLK_TAB] = true; m_aKeyWasDown[SDLK_TAB] = true; break;
					case SDLK_RETURN: 		m_aKeyIsDown[SDLK_RETURN] = true; m_aKeyWasDown[SDLK_RETURN] = true; break;
					case SDLK_SPACE: 		m_aKeyIsDown[SDLK_SPACE] = true; m_aKeyWasDown[SDLK_SPACE] = true; break;
					case SDLK_0: 	m_aKeyIsDown[SDLK_0] = true; m_aKeyWasDown[SDLK_0] = true; break;
					case SDLK_1: 	m_aKeyIsDown[SDLK_1] = true; m_aKeyWasDown[SDLK_1] = true; break;
					case SDLK_2: 	m_aKeyIsDown[SDLK_2] = true; m_aKeyWasDown[SDLK_2] = true; break;
					case SDLK_3: 	m_aKeyIsDown[SDLK_3] = true; m_aKeyWasDown[SDLK_3] = true; break;
					case SDLK_4: 	m_aKeyIsDown[SDLK_4] = true; m_aKeyWasDown[SDLK_4] = true; break;
					case SDLK_5: 	m_aKeyIsDown[SDLK_5] = true; m_aKeyWasDown[SDLK_5] = true; break;
					case SDLK_6: 	m_aKeyIsDown[SDLK_6] = true; m_aKeyWasDown[SDLK_6] = true; break;
					case SDLK_7: 	m_aKeyIsDown[SDLK_7] = true; m_aKeyWasDown[SDLK_7] = true; break;
					case SDLK_8: 	m_aKeyIsDown[SDLK_8] = true; m_aKeyWasDown[SDLK_8] = true; break;
					case SDLK_9: 	m_aKeyIsDown[SDLK_9] = true; m_aKeyWasDown[SDLK_9] = true; break;
					case SDLK_a: 	m_aKeyIsDown[SDLK_a] = true; m_aKeyWasDown[SDLK_a] = true; break;
					case SDLK_b: 	m_aKeyIsDown[SDLK_b] = true; m_aKeyWasDown[SDLK_b] = true; break;
					case SDLK_c: 	m_aKeyIsDown[SDLK_c] = true; m_aKeyWasDown[SDLK_c] = true; break;
					case SDLK_d: 	m_aKeyIsDown[SDLK_d] = true; m_aKeyWasDown[SDLK_d] = true; break;
					case SDLK_e: 	m_aKeyIsDown[SDLK_e] = true; m_aKeyWasDown[SDLK_e] = true; break;
					case SDLK_f: 	m_aKeyIsDown[SDLK_f] = true; m_aKeyWasDown[SDLK_f] = true; break;
					case SDLK_g: 	m_aKeyIsDown[SDLK_g] = true; m_aKeyWasDown[SDLK_g] = true; break;
					case SDLK_h: 	m_aKeyIsDown[SDLK_h] = true; m_aKeyWasDown[SDLK_h] = true; break;
					case SDLK_i: 	m_aKeyIsDown[SDLK_i] = true; m_aKeyWasDown[SDLK_i] = true; break;
					case SDLK_j: 	m_aKeyIsDown[SDLK_j] = true; m_aKeyWasDown[SDLK_j] = true; break;
					case SDLK_k: 	m_aKeyIsDown[SDLK_k] = true; m_aKeyWasDown[SDLK_k] = true; break;
					case SDLK_l: 	m_aKeyIsDown[SDLK_l] = true; m_aKeyWasDown[SDLK_l] = true; break;
					case SDLK_m: 	m_aKeyIsDown[SDLK_m] = true; m_aKeyWasDown[SDLK_m] = true; break;
					case SDLK_n: 	m_aKeyIsDown[SDLK_n] = true; m_aKeyWasDown[SDLK_n] = true; break;
					case SDLK_o: 	m_aKeyIsDown[SDLK_o] = true; m_aKeyWasDown[SDLK_o] = true; break;
					case SDLK_p: 	m_aKeyIsDown[SDLK_p] = true; m_aKeyWasDown[SDLK_p] = true; break;
					case SDLK_q: 	m_aKeyIsDown[SDLK_q] = true; m_aKeyWasDown[SDLK_q] = true; break;
					case SDLK_r: 	m_aKeyIsDown[SDLK_r] = true; m_aKeyWasDown[SDLK_r] = true; break;
					case SDLK_s: 	m_aKeyIsDown[SDLK_s] = true; m_aKeyWasDown[SDLK_s] = true; break;
					case SDLK_t: 	m_aKeyIsDown[SDLK_t] = true; m_aKeyWasDown[SDLK_t] = true; break;
					case SDLK_u: 	m_aKeyIsDown[SDLK_u] = true; m_aKeyWasDown[SDLK_u] = true; break;
					case SDLK_v: 	m_aKeyIsDown[SDLK_v] = true; m_aKeyWasDown[SDLK_v] = true; break;
					case SDLK_w: 	m_aKeyIsDown[SDLK_w] = true; m_aKeyWasDown[SDLK_w] = true; break;
					case SDLK_x: 	m_aKeyIsDown[SDLK_x] = true; m_aKeyWasDown[SDLK_x] = true; break;
					case SDLK_y: 	m_aKeyIsDown[SDLK_y] = true; m_aKeyWasDown[SDLK_y] = true; break;	//linux: y und z vertaucht
					case SDLK_z: 	m_aKeyIsDown[SDLK_z] = true; m_aKeyWasDown[SDLK_z] = true; break;
					case SDLK_DELETE: 	m_aKeyIsDown[SDLK_DELETE] = true; m_aKeyWasDown[SDLK_DELETE] = true; break;
					case SDLK_KP0: 	m_aKeyIsDown[SDLK_KP0] = true; m_aKeyWasDown[SDLK_KP0] = true; break;
					case SDLK_KP1: 	m_aKeyIsDown[SDLK_KP1] = true; m_aKeyWasDown[SDLK_KP1] = true; break;
					case SDLK_KP2: 	m_aKeyIsDown[SDLK_KP2] = true; m_aKeyWasDown[SDLK_KP2] = true; break;
					case SDLK_KP3: 	m_aKeyIsDown[SDLK_KP3] = true; m_aKeyWasDown[SDLK_KP3] = true; break;
					case SDLK_KP4: 	m_aKeyIsDown[SDLK_KP4] = true; m_aKeyWasDown[SDLK_KP4] = true; break;
					case SDLK_KP5: 	m_aKeyIsDown[SDLK_KP5] = true; m_aKeyWasDown[SDLK_KP5] = true; break;
					case SDLK_KP6: 	m_aKeyIsDown[SDLK_KP6] = true; m_aKeyWasDown[SDLK_KP6] = true; break;
					case SDLK_KP7: 	m_aKeyIsDown[SDLK_KP7] = true; m_aKeyWasDown[SDLK_KP7] = true; break;
					case SDLK_KP8: 	m_aKeyIsDown[SDLK_KP8] = true; m_aKeyWasDown[SDLK_KP8] = true; break;
					case SDLK_KP9: 	m_aKeyIsDown[SDLK_KP9] = true; m_aKeyWasDown[SDLK_KP9] = true; break;
					case SDLK_KP_PERIOD: 	m_aKeyIsDown[SDLK_KP_PERIOD] = true; m_aKeyWasDown[SDLK_KP_PERIOD] = true; break;
					case SDLK_KP_DIVIDE: 	m_aKeyIsDown[SDLK_KP_DIVIDE] = true; m_aKeyWasDown[SDLK_KP_DIVIDE] = true; break;
					case SDLK_KP_MULTIPLY: 	m_aKeyIsDown[SDLK_KP_MULTIPLY] = true; m_aKeyWasDown[SDLK_KP_MULTIPLY] = true; break;
					case SDLK_KP_MINUS: 	m_aKeyIsDown[SDLK_KP_MINUS] = true; m_aKeyWasDown[SDLK_KP_MINUS] = true; break;
					case SDLK_KP_PLUS: 		m_aKeyIsDown[SDLK_KP_PLUS] = true; m_aKeyWasDown[SDLK_KP_PLUS] = true; break;
					case SDLK_KP_ENTER: 	m_aKeyIsDown[SDLK_KP_ENTER] = true; m_aKeyWasDown[SDLK_KP_ENTER] = true; break;
					case SDLK_UP: 		m_aKeyIsDown[SDLK_UP] = true; m_aKeyWasDown[SDLK_UP] = true; break;
					case SDLK_DOWN: 	m_aKeyIsDown[SDLK_DOWN] = true; m_aKeyWasDown[SDLK_DOWN] = true; break;
					case SDLK_RIGHT: 	m_aKeyIsDown[SDLK_RIGHT] = true; m_aKeyWasDown[SDLK_RIGHT] = true; break;
					case SDLK_LEFT: 	m_aKeyIsDown[SDLK_LEFT] = true; m_aKeyWasDown[SDLK_LEFT] = true; break;
					case SDLK_INSERT: 	m_aKeyIsDown[SDLK_INSERT] = true; m_aKeyWasDown[SDLK_INSERT] = true; break;
					case SDLK_HOME: 	m_aKeyIsDown[SDLK_HOME] = true; m_aKeyWasDown[SDLK_HOME] = true; break;
					case SDLK_END: 		m_aKeyIsDown[SDLK_END] = true; m_aKeyWasDown[SDLK_END] = true; break;
					case SDLK_PAGEUP: 	m_aKeyIsDown[SDLK_PAGEUP] = true; m_aKeyWasDown[SDLK_PAGEUP] = true; break;
					case SDLK_PAGEDOWN: m_aKeyIsDown[SDLK_PAGEDOWN] = true; m_aKeyWasDown[SDLK_PAGEDOWN] = true; break;
					case SDLK_F1: 	m_aKeyIsDown[SDLK_F1] = true; m_aKeyWasDown[SDLK_F1] = true; break;
					case SDLK_F2: 	m_aKeyIsDown[SDLK_F2] = true; m_aKeyWasDown[SDLK_F2] = true; break;
					case SDLK_F3: 	m_aKeyIsDown[SDLK_F3] = true; m_aKeyWasDown[SDLK_F3] = true; break;
					case SDLK_F4: 	m_aKeyIsDown[SDLK_F4] = true; m_aKeyWasDown[SDLK_F4] = true; break;
					case SDLK_F5: 	m_aKeyIsDown[SDLK_F5] = true; m_aKeyWasDown[SDLK_F5] = true; break;
					case SDLK_F6: 	m_aKeyIsDown[SDLK_F6] = true; m_aKeyWasDown[SDLK_F6] = true; break;
					case SDLK_F7: 	m_aKeyIsDown[SDLK_F7] = true; m_aKeyWasDown[SDLK_F7] = true; break;
					case SDLK_F8: 	m_aKeyIsDown[SDLK_F8] = true; m_aKeyWasDown[SDLK_F8] = true; break;
					case SDLK_F9: 	m_aKeyIsDown[SDLK_F9] = true; m_aKeyWasDown[SDLK_F9] = true; break;
					case SDLK_F10: 	m_aKeyIsDown[SDLK_F10] = true; m_aKeyWasDown[SDLK_F10] = true; break;
					case SDLK_F11: 	m_aKeyIsDown[SDLK_F11] = true; m_aKeyWasDown[SDLK_F11] = true; break;
					case SDLK_F12: 	m_aKeyIsDown[SDLK_F12] = true; m_aKeyWasDown[SDLK_F12] = true; break;
					case SDLK_F13: 	m_aKeyIsDown[SDLK_F13] = true; m_aKeyWasDown[SDLK_F13] = true; break;
					case SDLK_F14: 	m_aKeyIsDown[SDLK_F14] = true; m_aKeyWasDown[SDLK_F14] = true; break;
					case SDLK_F15: 	m_aKeyIsDown[SDLK_F15] = true; m_aKeyWasDown[SDLK_F15] = true; break;
					case SDLK_CAPSLOCK: m_aKeyIsDown[SDLK_CAPSLOCK] = true; m_aKeyWasDown[SDLK_CAPSLOCK] = true; break;
					case SDLK_RSHIFT: 	m_aKeyIsDown[SDLK_RSHIFT] = true; m_aKeyWasDown[SDLK_RSHIFT] = true; break;
					case SDLK_LSHIFT: 	m_aKeyIsDown[SDLK_LSHIFT] = true; m_aKeyWasDown[SDLK_LSHIFT] = true; break;
					case SDLK_RCTRL: 	m_aKeyIsDown[SDLK_RCTRL] = true; m_aKeyWasDown[SDLK_RCTRL] = true; break;
					case SDLK_LCTRL: 	m_aKeyIsDown[SDLK_LCTRL] = true; m_aKeyWasDown[SDLK_LCTRL] = true; break;
					case SDLK_RALT: 	m_aKeyIsDown[SDLK_RALT] = true; m_aKeyWasDown[SDLK_RALT] = true; break;
					case SDLK_LALT: 	m_aKeyIsDown[SDLK_LALT] = true; m_aKeyWasDown[SDLK_LALT] = true; break;
					case SDLK_LSUPER: 	m_aKeyIsDown[SDLK_LSUPER] = true; m_aKeyWasDown[SDLK_LSUPER] = true; break;		/* Left "Windows" key */
					case SDLK_RSUPER: 	m_aKeyIsDown[SDLK_RSUPER] = true; m_aKeyWasDown[SDLK_RSUPER] = true; break;		/* Right "Windows" key */
					case SDLK_MODE: 	m_aKeyIsDown[SDLK_MODE] = true; m_aKeyWasDown[SDLK_MODE] = true; break;		/* "Alt Gr" key */
					
					default: break;
				}
			} break;
			case SDL_KEYUP: {
				switch( event.key.keysym.sym ) { 
					case SDLK_ESCAPE: 		m_aKeyIsDown[SDLK_ESCAPE] = false; m_aKeyWasUp[SDLK_ESCAPE] = true; break;
					case SDLK_BACKSPACE: 	m_aKeyIsDown[SDLK_BACKSPACE] = false; m_aKeyWasUp[SDLK_BACKSPACE] = true; break;
					case SDLK_TAB: 			m_aKeyIsDown[SDLK_TAB] = false; m_aKeyWasUp[SDLK_TAB] = true; break;
					case SDLK_RETURN: 		m_aKeyIsDown[SDLK_RETURN] = false; m_aKeyWasUp[SDLK_RETURN] = true; break;
					case SDLK_SPACE: 		m_aKeyIsDown[SDLK_SPACE] = false; m_aKeyWasUp[SDLK_SPACE] = true; break;
					case SDLK_0: 	m_aKeyIsDown[SDLK_0] = false; m_aKeyWasUp[SDLK_0] = true; break;
					case SDLK_1: 	m_aKeyIsDown[SDLK_1] = false; m_aKeyWasUp[SDLK_1] = true; break;
					case SDLK_2: 	m_aKeyIsDown[SDLK_2] = false; m_aKeyWasUp[SDLK_2] = true; break;
					case SDLK_3: 	m_aKeyIsDown[SDLK_3] = false; m_aKeyWasUp[SDLK_3] = true; break;
					case SDLK_4: 	m_aKeyIsDown[SDLK_4] = false; m_aKeyWasUp[SDLK_4] = true; break;
					case SDLK_5: 	m_aKeyIsDown[SDLK_5] = false; m_aKeyWasUp[SDLK_5] = true; break;
					case SDLK_6: 	m_aKeyIsDown[SDLK_6] = false; m_aKeyWasUp[SDLK_6] = true; break;
					case SDLK_7: 	m_aKeyIsDown[SDLK_7] = false; m_aKeyWasUp[SDLK_7] = true; break;
					case SDLK_8: 	m_aKeyIsDown[SDLK_8] = false; m_aKeyWasUp[SDLK_8] = true; break;
					case SDLK_9: 	m_aKeyIsDown[SDLK_9] = false; m_aKeyWasUp[SDLK_9] = true; break;
					case SDLK_a: 	m_aKeyIsDown[SDLK_a] = false; m_aKeyWasUp[SDLK_a] = true; break;
					case SDLK_b: 	m_aKeyIsDown[SDLK_b] = false; m_aKeyWasUp[SDLK_b] = true; break;
					case SDLK_c: 	m_aKeyIsDown[SDLK_c] = false; m_aKeyWasUp[SDLK_c] = true; break;
					case SDLK_d: 	m_aKeyIsDown[SDLK_d] = false; m_aKeyWasUp[SDLK_d] = true; break;
					case SDLK_e: 	m_aKeyIsDown[SDLK_e] = false; m_aKeyWasUp[SDLK_e] = true; break;
					case SDLK_f: 	m_aKeyIsDown[SDLK_f] = false; m_aKeyWasUp[SDLK_f] = true; break;
					case SDLK_g: 	m_aKeyIsDown[SDLK_g] = false; m_aKeyWasUp[SDLK_g] = true; break;
					case SDLK_h: 	m_aKeyIsDown[SDLK_h] = false; m_aKeyWasUp[SDLK_h] = true; break;
					case SDLK_i: 	m_aKeyIsDown[SDLK_i] = false; m_aKeyWasUp[SDLK_i] = true; break;
					case SDLK_j: 	m_aKeyIsDown[SDLK_j] = false; m_aKeyWasUp[SDLK_j] = true; break;
					case SDLK_k: 	m_aKeyIsDown[SDLK_k] = false; m_aKeyWasUp[SDLK_k] = true; break;
					case SDLK_l: 	m_aKeyIsDown[SDLK_l] = false; m_aKeyWasUp[SDLK_l] = true; break;
					case SDLK_m: 	m_aKeyIsDown[SDLK_m] = false; m_aKeyWasUp[SDLK_m] = true; break;
					case SDLK_n: 	m_aKeyIsDown[SDLK_n] = false; m_aKeyWasUp[SDLK_n] = true; break;
					case SDLK_o: 	m_aKeyIsDown[SDLK_o] = false; m_aKeyWasUp[SDLK_o] = true; break;
					case SDLK_p: 	m_aKeyIsDown[SDLK_p] = false; m_aKeyWasUp[SDLK_p] = true; break;
					case SDLK_q: 	m_aKeyIsDown[SDLK_q] = false; m_aKeyWasUp[SDLK_q] = true; break;
					case SDLK_r: 	m_aKeyIsDown[SDLK_r] = false; m_aKeyWasUp[SDLK_r] = true; break;
					case SDLK_s: 	m_aKeyIsDown[SDLK_s] = false; m_aKeyWasUp[SDLK_s] = true; break;
					case SDLK_t: 	m_aKeyIsDown[SDLK_t] = false; m_aKeyWasUp[SDLK_t] = true; break;
					case SDLK_u: 	m_aKeyIsDown[SDLK_u] = false; m_aKeyWasUp[SDLK_u] = true; break;
					case SDLK_v: 	m_aKeyIsDown[SDLK_v] = false; m_aKeyWasUp[SDLK_v] = true; break;
					case SDLK_w: 	m_aKeyIsDown[SDLK_w] = false; m_aKeyWasUp[SDLK_w] = true; break;
					case SDLK_x: 	m_aKeyIsDown[SDLK_x] = false; m_aKeyWasUp[SDLK_x] = true; break;
					case SDLK_y: 	m_aKeyIsDown[SDLK_y] = false; m_aKeyWasUp[SDLK_y] = true; break;	//linux: y und z vertaucht
					case SDLK_z: 	m_aKeyIsDown[SDLK_z] = false; m_aKeyWasUp[SDLK_z] = true; break;
					case SDLK_DELETE: 	m_aKeyIsDown[SDLK_DELETE] = false; m_aKeyWasUp[SDLK_DELETE] = true; break;
					case SDLK_KP0: 	m_aKeyIsDown[SDLK_KP0] = false; m_aKeyWasUp[SDLK_KP0] = true; break;
					case SDLK_KP1: 	m_aKeyIsDown[SDLK_KP1] = false; m_aKeyWasUp[SDLK_KP1] = true; break;
					case SDLK_KP2: 	m_aKeyIsDown[SDLK_KP2] = false; m_aKeyWasUp[SDLK_KP2] = true; break;
					case SDLK_KP3: 	m_aKeyIsDown[SDLK_KP3] = false; m_aKeyWasUp[SDLK_KP3] = true; break;
					case SDLK_KP4: 	m_aKeyIsDown[SDLK_KP4] = false; m_aKeyWasUp[SDLK_KP4] = true; break;
					case SDLK_KP5: 	m_aKeyIsDown[SDLK_KP5] = false; m_aKeyWasUp[SDLK_KP5] = true; break;
					case SDLK_KP6: 	m_aKeyIsDown[SDLK_KP6] = false; m_aKeyWasUp[SDLK_KP6] = true; break;
					case SDLK_KP7: 	m_aKeyIsDown[SDLK_KP7] = false; m_aKeyWasUp[SDLK_KP7] = true; break;
					case SDLK_KP8: 	m_aKeyIsDown[SDLK_KP8] = false; m_aKeyWasUp[SDLK_KP8] = true; break;
					case SDLK_KP9: 	m_aKeyIsDown[SDLK_KP9] = false; m_aKeyWasUp[SDLK_KP9] = true; break;
					case SDLK_KP_PERIOD: 	m_aKeyIsDown[SDLK_KP_PERIOD] = false; m_aKeyWasUp[SDLK_KP_PERIOD] = true; break;
					case SDLK_KP_DIVIDE: 	m_aKeyIsDown[SDLK_KP_DIVIDE] = false; m_aKeyWasUp[SDLK_KP_DIVIDE] = true; break;
					case SDLK_KP_MULTIPLY: 	m_aKeyIsDown[SDLK_KP_MULTIPLY] = false; m_aKeyWasUp[SDLK_KP_MULTIPLY] = true; break;
					case SDLK_KP_MINUS: 	m_aKeyIsDown[SDLK_KP_MINUS] = false; m_aKeyWasUp[SDLK_KP_MINUS] = true; break;
					case SDLK_KP_PLUS: 		m_aKeyIsDown[SDLK_KP_PLUS] = false; m_aKeyWasUp[SDLK_KP_PLUS] = true; break;
					case SDLK_KP_ENTER: 	m_aKeyIsDown[SDLK_KP_ENTER] = false; m_aKeyWasUp[SDLK_KP_ENTER] = true; break;
					case SDLK_UP: 		m_aKeyIsDown[SDLK_UP] = false; m_aKeyWasUp[SDLK_UP] = true; break;
					case SDLK_DOWN: 	m_aKeyIsDown[SDLK_DOWN] = false; m_aKeyWasUp[SDLK_DOWN] = true; break;
					case SDLK_RIGHT: 	m_aKeyIsDown[SDLK_RIGHT] = false; m_aKeyWasUp[SDLK_RIGHT] = true; break;
					case SDLK_LEFT: 	m_aKeyIsDown[SDLK_LEFT] = false; m_aKeyWasUp[SDLK_LEFT] = true; break;
					case SDLK_INSERT: 	m_aKeyIsDown[SDLK_INSERT] = false; m_aKeyWasUp[SDLK_INSERT] = true; break;
					case SDLK_HOME: 	m_aKeyIsDown[SDLK_HOME] = false; m_aKeyWasUp[SDLK_HOME] = true; break;
					case SDLK_END: 		m_aKeyIsDown[SDLK_END] = false; m_aKeyWasUp[SDLK_END] = true; break;
					case SDLK_PAGEUP: 	m_aKeyIsDown[SDLK_PAGEUP] = false; m_aKeyWasUp[SDLK_PAGEUP] = true; break;
					case SDLK_PAGEDOWN: m_aKeyIsDown[SDLK_PAGEDOWN] = false; m_aKeyWasUp[SDLK_PAGEDOWN] = true; break;
					case SDLK_F1: 	m_aKeyIsDown[SDLK_F1] = false; m_aKeyWasUp[SDLK_F1] = true; break;
					case SDLK_F2: 	m_aKeyIsDown[SDLK_F2] = false; m_aKeyWasUp[SDLK_F2] = true; break;
					case SDLK_F3: 	m_aKeyIsDown[SDLK_F3] = false; m_aKeyWasUp[SDLK_F3] = true; break;
					case SDLK_F4: 	m_aKeyIsDown[SDLK_F4] = false; m_aKeyWasUp[SDLK_F4] = true; break;
					case SDLK_F5: 	m_aKeyIsDown[SDLK_F5] = false; m_aKeyWasUp[SDLK_F5] = true; break;
					case SDLK_F6: 	m_aKeyIsDown[SDLK_F6] = false; m_aKeyWasUp[SDLK_F6] = true; break;
					case SDLK_F7: 	m_aKeyIsDown[SDLK_F7] = false; m_aKeyWasUp[SDLK_F7] = true; break;
					case SDLK_F8: 	m_aKeyIsDown[SDLK_F8] = false; m_aKeyWasUp[SDLK_F8] = true; break;
					case SDLK_F9: 	m_aKeyIsDown[SDLK_F9] = false; m_aKeyWasUp[SDLK_F9] = true; break;
					case SDLK_F10: 	m_aKeyIsDown[SDLK_F10] = false; m_aKeyWasUp[SDLK_F10] = true; break;
					case SDLK_F11: 	m_aKeyIsDown[SDLK_F11] = false; m_aKeyWasUp[SDLK_F11] = true; break;
					case SDLK_F12: 	m_aKeyIsDown[SDLK_F12] = false; m_aKeyWasUp[SDLK_F12] = true; break;
					case SDLK_F13: 	m_aKeyIsDown[SDLK_F13] = false; m_aKeyWasUp[SDLK_F13] = true; break;
					case SDLK_F14: 	m_aKeyIsDown[SDLK_F14] = false; m_aKeyWasUp[SDLK_F14] = true; break;
					case SDLK_F15: 	m_aKeyIsDown[SDLK_F15] = false; m_aKeyWasUp[SDLK_F15] = true; break;
					case SDLK_CAPSLOCK: m_aKeyIsDown[SDLK_CAPSLOCK] = false; m_aKeyWasUp[SDLK_CAPSLOCK] = true; break;
					case SDLK_RSHIFT: 	m_aKeyIsDown[SDLK_RSHIFT] = false; m_aKeyWasUp[SDLK_RSHIFT] = true; break;
					case SDLK_LSHIFT: 	m_aKeyIsDown[SDLK_LSHIFT] = false; m_aKeyWasUp[SDLK_LSHIFT] = true; break;
					case SDLK_RCTRL: 	m_aKeyIsDown[SDLK_RCTRL] = false; m_aKeyWasUp[SDLK_RCTRL] = true; break;
					case SDLK_LCTRL: 	m_aKeyIsDown[SDLK_LCTRL] = false; m_aKeyWasUp[SDLK_LCTRL] = true; break;
					case SDLK_RALT: 	m_aKeyIsDown[SDLK_RALT] = false; m_aKeyWasUp[SDLK_RALT] = true; break;
					case SDLK_LALT: 	m_aKeyIsDown[SDLK_LALT] = false; m_aKeyWasUp[SDLK_LALT] = true; break;
					case SDLK_LSUPER: 	m_aKeyIsDown[SDLK_LSUPER] = false; m_aKeyWasUp[SDLK_LSUPER] = true; break;		/* Left "Windows" key */
					case SDLK_RSUPER: 	m_aKeyIsDown[SDLK_RSUPER] = false; m_aKeyWasUp[SDLK_RSUPER] = true; break;		/* Right "Windows" key */
					case SDLK_MODE: 	m_aKeyIsDown[SDLK_MODE] = false; m_aKeyWasUp[SDLK_MODE] = true; break;		/* "Alt Gr" key */
					
					default: break;
				}
			} break;
			
			
			/*mouse*/
			case SDL_QUIT: { //quit-button
				m_bMouseQuit = true;
			} break;
			case SDL_MOUSEBUTTONDOWN: {
				//doppelklick abfragen
				m_MouseClickTimer.Update();
				if(m_MouseClickTimer.GetElapsed() < 0.2f) { //value: 0.15 - 0.25; std: 0.2
					m_bMouseDoubleClick = true;
				}
				
				switch(event.button.button) {
					case SDL_BUTTON_LEFT: 		m_aMouseIsDown[SDL_BUTTON_LEFT] = true; m_aMouseWasDown[SDL_BUTTON_LEFT] = true; break;
					case SDL_BUTTON_MIDDLE:		m_aMouseIsDown[SDL_BUTTON_MIDDLE] = true; m_aMouseWasDown[SDL_BUTTON_MIDDLE] = true; break;
					case SDL_BUTTON_RIGHT:		m_aMouseIsDown[SDL_BUTTON_RIGHT] = true; m_aMouseWasDown[SDL_BUTTON_RIGHT] = true; break;
					case SDL_BUTTON_WHEELUP:	m_aMouseIsDown[SDL_BUTTON_WHEELUP] = true; m_aMouseWasDown[SDL_BUTTON_WHEELUP] = true; break;
					case SDL_BUTTON_WHEELDOWN:	m_aMouseIsDown[SDL_BUTTON_WHEELDOWN] = true; m_aMouseWasDown[SDL_BUTTON_WHEELDOWN] = true; break;
					
					default: break;
				}
			} break;
			case SDL_MOUSEBUTTONUP: {
				switch(event.button.button) {
					case SDL_BUTTON_LEFT: 		m_aMouseIsDown[SDL_BUTTON_LEFT] = false; m_aMouseWasUp[SDL_BUTTON_LEFT] = true; break;
					case SDL_BUTTON_MIDDLE:		m_aMouseIsDown[SDL_BUTTON_MIDDLE] = false; m_aMouseWasUp[SDL_BUTTON_MIDDLE] = true; break;
					case SDL_BUTTON_RIGHT:		m_aMouseIsDown[SDL_BUTTON_RIGHT] = false; m_aMouseWasUp[SDL_BUTTON_RIGHT] = true; break;
					case SDL_BUTTON_WHEELUP:	m_aMouseIsDown[SDL_BUTTON_WHEELUP] = false; m_aMouseWasUp[SDL_BUTTON_WHEELUP] = true; break;
					case SDL_BUTTON_WHEELDOWN:	m_aMouseIsDown[SDL_BUTTON_WHEELDOWN] = false; m_aMouseWasUp[SDL_BUTTON_WHEELDOWN] = true; break;
					
					default: break;
				}
			} break;
			
			default: break;
		}
	}
}


