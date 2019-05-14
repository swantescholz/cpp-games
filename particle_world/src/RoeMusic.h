#ifndef ROEMUSIC_H_
#define ROEMUSIC_H_

#include "RoeCommon.h"

#ifdef ROE_COMPILE_AUDIO

#include <SDL_mixer.h>
#include <string>
#include <map>


// class Music (a simple music player using SDL_mixer)

namespace Roe {

	struct S_MusicIntern {
		Mix_Music *music;
		float fStdVolume;
		double dStdFadeIn;   //in sec
		double dStdPos;      //in sec
	};
	
	class Music
	{
	public:
		Music(const float fVolume = 1.0f);
		Music(const std::string &sPath, const float fVolume = 1.0f);
		~Music();
		
		void 	load(std::string sPath);
		
		//call all functions only after load
		void play(const int iLoops = 0, const double dFadeIn = 0.0, const double dPos = 0.0);
		static void resume()        {Mix_ResumeMusic();}
		static void pause()         {Mix_PauseMusic();}
		static void restart()       {Mix_RewindMusic();}
		static void halt()          {Mix_HaltMusic();}
		
		void            setVolume (const float f)   {m_fVolume = f;}
		float           getVolume ()    const       {return m_fVolume;}
		float           getStdVolume();     // gets the value from static map
		double          getStdFadeIn();     // gets the value from static map
		double          getStdPos();        // gets the value from static map
		Mix_Music*      getMusic();         // gets the value from static map
		std::string     getName ()      const       {return m_sName;}
		std::string     getPath ()      const       {return m_sPath;}
		bool            isLoaded()      const       {if (m_sPath.length() > 0) return true; return false;}
		
		//loading and deleting
		static void loadMusic           (std::string sPath, const float fVolume = 1.0f, const double dFadeIn = 0.0, const double dPos = 0.0);
		static void deleteMusic         (std::string sPath, bool bCompleteThePath = true); //dont delete music which is still in use by an instance! (TODO)
		static bool isMusicLoaded       (std::string sPath, bool bCompleteThePath = true);
		
		// for the static variables
		static void init(std::string sStart, std::string sEnd);
		static void setPathStart(std::string s)                 {s_sPathStart = s;}
		static void setPathEnd  (std::string s)                 {s_sPathEnd   = s;}
		static void quit();
		
	private:
	
		static std::map<std::string, S_MusicIntern >                s_mpMusic;
		static std::map<std::string, S_MusicIntern >::iterator      s_mpi;
		static std::pair<std::string, S_MusicIntern>                s_pair;   //pair-object for the map
		static std::string s_sPathStart, s_sPathEnd;                          //std path
		
		
		std::string m_sPath;    //path of the instance to file
		std::string m_sName;    //name (path without start and end)
		float m_fVolume;        //factor: 1->std, 0->mute, 2->double than std
	};
	
} // namespace Roe

#endif //ROE_COMPILE_AUDIO
#endif /*ROEMUSIC_H_*/
