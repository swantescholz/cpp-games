#include "RoeApplication.h"

namespace Roe {
	
	Application::Application()
	{
		Timer::init(); //take at first the clock time
		Logfile::createSingleton();
		Random::createSingleton();
#ifdef ROE_COMPILE_AUDIO
		AudioManager::createSingleton();
#endif
		SDL_GL::createSingleton();
	}
	
	Application::~Application()
	{
		Random::destroySingleton();
#ifdef ROE_COMPILE_AUDIO
		AudioManager::destroySingleton();
#endif
		SDL_GL::destroySingleton();
		Logfile::destroySingleton();
	}
	
} // namespace Roe
