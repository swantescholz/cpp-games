#ifndef ROESOUND_H_
#define ROESOUND_H_

#include "RoeCommon.h"

#ifdef ROE_COMPILE_AUDIO

#include <SDL_mixer.h>
#include <string>
#include <map>

namespace Roe {

	struct S_SoundIntern {
		Mix_Chunk *chunk;   //chunk
		float fStdVolume;   //faktor
	};
	
	class Sound
	{
	public:
		Sound(const float fVolume = 1.0f);
		Sound(const std::string &sPath, const float fVolume = 1.0f);
		Sound(const Sound& rhs);
		~Sound();
		
		Sound& operator = (const Sound& rhs);
		
		//initialises path for instance
		void         load    (std::string sPath);
		std::string  getName () const {return m_sName;}
		std::string  getPath () const {return m_sPath;}
		
		//call all functions only after load()
		void play   (const int iLoops = 0);
		void resume ();
		void pause  ();
		void restart(bool bPlay = true);
		void halt   ();
		
		void  setVolume   (const float f) {m_fVolume = f; if (isPlaying()) Mix_Volume(m_iChannel,MIX_MAX_VOLUME*getStdVolume()*m_fVolume);}
		float getVolume   () const        {return m_fVolume;}
		float getStdVolume();
		int   getChannel  () const        {return m_iChannel;}
		bool  isPlaying   () const        {return (m_sPath != "") ? (s_mpSoundPlaying[m_sPath]) : (false);}
		
		//loading and deleting of sounds
		static void loadSound      (std::string sPath, const float fVolume = 1.0f);
		static void deleteSound    (std::string sPath, bool bCompleteThePath = true);
		static bool isChannelPlaying (int channel);
		static int  getNumberOfChannelsPlaying ();
		
		// for the static variables
		static void init(const int iMaxChannels, std::string sStart, std::string sEnd);
		static void setPathStart(std::string s)      {s_sPathStart = s;}
		static void setPathEnd  (std::string s)      {s_sPathEnd   = s;}
		static void quit();
		
		//have effects on all channels
		static void haltAll  ()         {Mix_HaltChannel(-1);}
		static void pauseAll ()         {Mix_Pause(-1);}
		static void resumeAll()         {Mix_Resume(-1);}
	private:
		static void channelFinished (int channel);      // callback function
		static std::map <std::string, S_SoundIntern >               s_mpSound;  //map of sound-objects
		static std::map <std::string, S_SoundIntern >::iterator     s_mpi;      //map-iterator
		static std::pair<std::string, S_SoundIntern>                s_pair;     //paar-object for teh map
		static std::map <std::string, int >                         s_mpSoundChannel;  //map of sound-objects channels
		static std::map <std::string, bool>                         s_mpSoundPlaying;  //map of sound-objects playing
		static std::string s_sPathStart, s_sPathEnd;                //standart path
		static int s_iMaxChannels;                                  //maximum number of channels
		static bool *s_abChannelPlaying;  // dynamic array of all channels playing or not
		
		std::string m_sPath;        //path of the instance to file
		std::string m_sName;        //name (path without start and end)
		float m_fVolume;            //factor: 1->std, 0->mute, 2->double than std
		int m_iChannel;             //the channel of this sound
	};

} // namespace Roe

#endif //ROE_COMPILE_AUDIO
#endif /*ROESOUND_H_*/
