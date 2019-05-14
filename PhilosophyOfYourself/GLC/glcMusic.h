#ifndef GLCMUSIC_H_
#define GLCMUSIC_H_

#include "glcSDL_GL.h"

struct S_MusicIntern {
	Mix_Music *music;
	float fStdVolume;
	double dStdFadeIn;		//in sec
	double dStdPos;		//in sec
};

class glcMusic
{
public:
	glcMusic(const float fVolume = 1.0f);
	glcMusic(const string &sPath, const float fVolume = 1.0f);
	~glcMusic();
	
	void 	Load(string sPath);
	
	//alle funktionen erst nach load aufrufen (oder 2. konstruktor)
	void Play(const int iLoops = 0, const double dFadeIn = 0.0, const double dPos = 0.0);
	static void Resume()		{Mix_ResumeMusic();}
	static void Pause()			{Mix_PauseMusic();}
	static void Restart()		{Mix_RewindMusic();}
	static void Halt()			{Mix_HaltMusic();}
	
	void  	SetVolume (const float f)	{m_fVolume = f;}
	float 	GetVolume ()   const		{return m_fVolume;}
	float 	GetStdVolume();
	string  GetName	() const {return m_sName;}
	string  GetPath	() const {return m_sPath;}
	bool	IsLoaded() const {if (m_sPath.length() > 0) return true; return false;}
	
	//zum laden und loeschen
	static void LoadMusic			(string sPath, const float fVolume = 1.0f, const double dFadeIn = 0.0, const double dPos = 0.0);
	static void DeleteMusic			(string sPath, bool bCompleteThePath = true); //dont delete music which is still in use by an instance! (TODO)
	static bool	IsMusicLoaded		(string sPath, bool bCompleteThePath = true);
	
	// fuer die statischen variablen
	static void Init(string sStart, string sEnd);
	static void SetPathStart(string s) 				{c_sPathStart = s;}
	static void SetPathEnd  (string s) 				{c_sPathEnd   = s;}
	static void Quit();
	
private:

	static map<string, S_MusicIntern > 				c_mpMusic;
	static map<string, S_MusicIntern >::iterator 	c_mpi;
	static pair<string, S_MusicIntern> 				c_pair;		//paar-objekt fuer die map
	static string c_sPathStart, c_sPathEnd;						//standart path
	
	
	string m_sPath;		//pfad der instanz zum file
	string m_sName;
	float m_fVolume; 	//faktor: 1->standart, 0->still, 2->doppelte standart-lautstaerke
};

#endif /*GLCMUSIC_H_*/
