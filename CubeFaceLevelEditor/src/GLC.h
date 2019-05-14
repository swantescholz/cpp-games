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
#include "glcModel.h"
#include "glcLight.h"
#include "glcSphere.h"
#include "glcFont.h"
#include "glcTexture.h"
#include "glcSound.h"
#include "glcMusic.h"

using namespace std;



void glcInit(int width = 800, int hight = 600, flag8 GLflags = GLC_LIGHTING | GLC_FOG | GLC_CULLING, int SDLflags = SDL_OPENGL,
			 string sWindow = "No_Name | by Swante Scholz 2009", string sTab = "No_Name", string sLogfile = "Logfile.html");
void glcQuit();
void glcUpdate(int iTimer = 0);
void glcFlip();
void glcDelay(float fSec);
void glcDelay(int iMil);

bool glcKeyIsDown	(int iID);
bool glcKeyWasDown	(int iID);
bool glcKeyWasUp	(int iID);


#endif /*GLC_H_*/
