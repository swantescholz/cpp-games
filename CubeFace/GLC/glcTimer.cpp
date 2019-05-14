#include "glcTimer.h"

glcTimer::glcTimer() {
	time_t zeit;//zeit
	time (&zeit);
	srand((unsigned)zeit);//zufallsgenerator initialisieren
	
	
	fCurTime = 0.0f;
	fLastTime = SDL_GetTicks() / 1000.0f;
	fElapsed = 0.0f;
}

glcTimer::~glcTimer(){
}

void glcTimer::Update(unsigned int i) {
	static int si = SDL_GetTicks();
	
	fCurTime = SDL_GetTicks() / 1000.0f;
	fElapsed = fCurTime - fLastTime;
	fLastTime = fCurTime;
	
	if (i != 0) {
		if (SDL_GetTicks() - si > i) {
			si = SDL_GetTicks();
			cout << 1 / fElapsed << endl;
		}
	}	
}
