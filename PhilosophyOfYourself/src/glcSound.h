#ifndef GLCSOUND_H_
#define GLCSOUND_H_

#include "glcSDL_GL.h"

struct S_SoundIntern {
	Mix_Chunk *chunk;	//chunk
	float fStdVolume;	//faktor
};

class glcSound
{
public:
	glcSound(const float fVolume = 1.0f);
	glcSound(const string &sPath, const float fVolume = 1.0f);
	glcSound(const glcSound& rhs);
	~glcSound();
	
	glcSound& operator = (const glcSound& rhs);
	
	//initialisiert string fuer instanz
	void Load(string sPath);
	string 	  GetName	() const {return m_sName;}
	
	//alle funktionen erst nach load aufrufen (oder 2. konstruktor)
	void Play(const int iLoops = 0);
	void Resume();
	void Pause();
	void Restart(bool bPlay = true);
	void Halt();
	
	void  SetVolume (const float f)	{m_fVolume = f;}
	float GetVolume ()	 const			{return m_fVolume;}
	float GetStdVolume();
	
	//zum laden und loeschen eines sounds
	static void LoadSound	(string sPath, const float fVolume = 1.0f);
	static void DeleteSound	(string sPath, bool bCompleteThePath = true);
	
	
	// fuer die statischen variablen
	static void Init(const int iMaxChannels, string sStart, string sEnd);
	static void SetPathStart(string s) 				{c_sPathStart = s;}
	static void SetPathEnd  (string s) 				{c_sPathEnd   = s;}
	static void Quit();
	
	//haben auswirkungen auf alle channels
	static void HaltAll()							{Mix_HaltChannel(-1);}
	static void PauseAll()							{Mix_Pause(-1);}
	static void ResumeAll()							{Mix_Resume(-1);}
private:
	static map<string, S_SoundIntern > 				c_mpSound;	//map of sound-objects
	static map<string, S_SoundIntern >::iterator 	c_mpi;		//map-iterator
	static pair<string, S_SoundIntern> 				c_pair;		//paar-objekt fuer die map
	static string c_sPathStart, c_sPathEnd;						//standart path
	static int c_iNumChannels;									//number of channels
	static int c_iMaxChannels;									//maximum number of channels
	
	string m_sPath;		//pfad der instanz zum file
	string m_sName;
	float m_fVolume; 	//faktor: 1->standart, 0->still, 2->doppelte standart-lautstaerke
	int m_iChannel;		//kanal
};

#endif /*GLCSOUND_H_*/
