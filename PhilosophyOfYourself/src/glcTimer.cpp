#include "glcTimer.h"

glcTimer::glcTimer() {
	time_t zeit;//zeit
	time (&zeit);
	srand((unsigned)zeit);//zufallsgenerator initialisieren
	
	
	m_fCurTime = 0.0f;
	m_fLastTime = SDL_GetTicks() / 1000.0f;
	m_fElapsed = 0.0f;
	m_fLastCurTime = 0.0f;
	m_fFPS = 0.0f;
}

glcTimer::~glcTimer(){
}

void glcTimer::Update(float f, bool bWriteToLog) {
	m_fCurTime = SDL_GetTicks() / 1000.0f;
	m_fElapsed = m_fCurTime - m_fLastTime;
	m_fLastTime = m_fCurTime;
	
	if (f > 0.0f) {
		if (m_fCurTime - m_fLastCurTime > f) {
			m_fLastCurTime = m_fCurTime;
			m_fFPS = (1.0f / m_fElapsed);
			if (bWriteToLog == true) {
				char cTmp[40];
				sprintf(cTmp, "FPS: %g", m_fFPS);
				g_pLogfile->Textout(BLACK, cTmp);
			}
		}
	}	
}
