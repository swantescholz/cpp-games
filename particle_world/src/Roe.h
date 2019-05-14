#ifndef ROE_H_
#define ROE_H_

//Roe (relevant Ogre extensions) or (reusable O[Ogre/Ode/Ois] engine)

/*Roe TODO:
-terrain with alpha textures
-roller_coaster z-rotation
//*/

//Roe works counter clock wise(CCW)!

#include <GL/gl.h>
#include <ode/ode.h>

//everything is in namespace Roe

//general Roe application base class
#include "RoeApplication.h"
//graphics with SDL, SDL_image, SDL_ttf and OpenGL
#include "RoeSDL_GL.h"  //+input with SDL
#include "RoeTexture.h" //SDL_image is used
#include "RoeFont.h"    //SDL_ttf is used
#include "RoeLight.h"
#include "RoeModel.h"
#include "RoeParticle.h"
//physics with ODE (namespace 'Physics')
#include "RoePhysicsEngine.h"
#include "RoePhysicsModel.h"
//audio with SDL_mixer
#include "RoeMusic.h"
#include "RoeSound.h"
#include "RoeAudioManager.h"
//file i/o
#include "RoeTextfile.h"
#include "RoeXML.h"
#include "RoeBinFile.h"
#include "RoeLogfile.h"
//math
#include "RoeKeyframeInterpolator.h"
#include "RoeMatrix.h"
#include "RoeVector2.h"
#include "RoeVector3.h"
#include "RoeColor.h"
#include "RoeMaterial.h"
#include "RoePlane.h"
#include "RoeCamera.h"
#include "RoeForceField.h"
//system specific things (clipboard, directory parsing...)
#include "RoeSystem.h"
//other
#include "RoeTokenizer.h"
#include "RoeTimer.h"
#include "RoeSingleton.h"
#include "RoeException.h"
//common and util
#include "RoeCommon.h"
#include "RoeUtil.h"



#endif /*ROE_H_*/
