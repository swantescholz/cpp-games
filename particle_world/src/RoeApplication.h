#ifndef ROEAPPLICATION_H_
#define ROEAPPLICATION_H_


#include "RoeAudioManager.h"
#include "RoeRandom.h"
#include "RoeSDL_GL.h"
#include "RoeTexture.h"
#include "RoeLogfile.h"
#include "RoeCommon.h"


namespace Roe {
	
	//class Application, it combines Ogre + Ode + OIS + SDL_Mixer
	class Application
	{
	public:
		Application();
		virtual ~Application();
		
	protected:
		
	private:
		
	};
	
} // namespace Roe

#endif /*ROEAPPLICATION_H_*/
