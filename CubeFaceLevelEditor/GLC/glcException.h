#ifndef GLCEXCEPTION_H_
#define GLCEXCEPTION_H_

#include "SDL.h"
#include <iostream>
#include <string>
using namespace std;

class glcException
{
public:
	glcException();
	glcException(string s = "-", int i = 3000) {m_sErrStr = s; m_iDelay = i;}
	~glcException();
	
	void SetErrStr(string s) 	{m_sErrStr = s;}
	void SetDelay(int i)		{m_iDelay = i;}
	string GetErrStr()			const	{return m_sErrStr;}
	int GetDelay()				const	{return m_iDelay;}
	void Out() const {
		cout << "Exception aufgetreten" << endl;
		cout << "Fehlerstring:\t" << m_sErrStr << endl;
		cout << "Delaytion:\t" << m_iDelay << "ms" << endl;
		SDL_Delay(m_iDelay);
	}
	
private:
	string m_sErrStr;
	int m_iDelay; 
};

#endif /*GLCEXCEPTION_H_*/
