#ifndef GLCTIMER_H_
#define GLCTIMER_H_

#include <SDL/SDL.h>
#include "Singleton.h"
#include "glcLogfile.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

#define g_pTimer glcTimer::Get()
#define ELAPSED glcTimer::Get()->GetElapsed()
#define RANDOM(X,Y) glcTimer::Get()->Random(X,Y)

class glcTimer : public TSingleton<glcTimer>
{
public:
	glcTimer();
	~glcTimer();
	
	void	Update(float f = -1.0f, bool bWriteToLog = false);	//verursacht bei f > 0.0f ggf. ruckler (TODO)
	float	Random(float fMin, float fMax)	{return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);}
	int		Random(int min, int max)			{return (rand() % ((max - min) + 1)) + min;}
	float	GetElapsed() const {return m_fElapsed;}
	float	GetCurTime() {m_fCurTime = SDL_GetTicks() / 1000.0f; return m_fCurTime;}
	float	GetFPS()	 const {return m_fFPS;}
	
private:
	float m_fElapsed;
	float m_fCurTime;
	float m_fLastTime;
	
	float m_fLastCurTime;   //for fps-rate
	float m_fFPS;			//fps-rate
};

#endif /*GLCTIMER_H_*/
