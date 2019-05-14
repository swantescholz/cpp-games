#ifndef GLCTIMER_H_
#define GLCTIMER_H_

#include "SDL.h"
#include "Singleton.h"
#include <iostream>
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
	
	void Update(unsigned int i = -1);
	float Random(float fMin, float fMax)	{return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);}
	int Random(int min, int max)			{return (rand() % ((max - min) + 1)) + min;}
	float GetElapsed() {return fElapsed;}
	float GetCurTime() {return fCurTime;}
	
private:
	float fElapsed;
	float fCurTime;
	float fLastTime;
};

#endif /*GLCTIMER_H_*/
