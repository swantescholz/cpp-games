#ifndef GLC_H_
#define GLC_H_

//GLC (OpenGL complex class - collection completition)

//include
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <fstream>
#include "Singleton.h"

#include "tbMatrix.h"
#include "tbVector3.h"
#include "tbVector2.h"
#include "tbPlane.h"
#include "tbColor.h"
#include "tbUtils.h"

#include "glcTimer.h"
#include "glcException.h"
#include "glcLogfile.h"

#include "glcSDL_GL.h"
#include "glcMaterial.h"
#include "glcModel.h"
#include "glcLight.h"
#include "glcSphere.h"
#include "glcFont.h"
#include "glcTexture.h"
#include "glcSound.h"
#include "glcMusic.h"
#include "glcKeyFrameInterpolator.h"
#include "glcParticle.h"

using namespace std;

/*render() should begin like this in your program:
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// We don't want to modify the projection matrix.
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	
	//cameramatrix festsetzen (zuallererst!)
	g_pGL->LookAt();
*/

void glcInit(int width = 800, int hight = 600, flag8 GLflags = GLC_LIGHTING | GLC_FOG | GLC_CULLING, int SDLflags = SDL_OPENGL, string sWindow = "No_Name | by Swante Scholz 2009", string sTab = "No_Name");
void glcQuit();
void glcUpdate(float fTimer = -1.0f);	//call it every frame
void glcFlip();							//call it at the end of rendering
void glcDelay(float fSec);
void glcDelay(int iMil);

bool glcKeyIsDown	(int iID);
bool glcKeyWasDown	(int iID);
bool glcKeyWasUp	(int iID);


#endif /*GLC_H_*/
