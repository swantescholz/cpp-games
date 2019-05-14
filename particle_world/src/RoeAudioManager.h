#ifndef ROEAUDIOMANAGER_H_
#define ROEAUDIOMANAGER_H_

#include "RoeCommon.h"

#ifdef ROE_COMPILE_AUDIO

#include <SDL_mixer.h>
#include "RoeMusic.h"
#include "RoeSound.h"
#include "RoeSingleton.h"

namespace Roe {
	
	class AudioManager : public Roe::Singleton<AudioManager>
	{
		friend class Roe::Singleton<AudioManager>;
	public:
		~AudioManager();
		
		void initAudio(int frequency = MIX_DEFAULT_FREQUENCY, bool stereo = true, int chunksize = 1024, int numSoundChannels = 20);
		void quitAudio();
		
		//some usefull prescripted sound methods
		void loadEffectNumber     (const std::string &psPath = "");
		void deleteEffectNumber   ();
		void playEffectNumber     (int piNumber);
		void updateAudioEffects   ();
		bool isEffectNumberRunning(); //is a number spoken out in this moment?
		
	private:
		AudioManager();
		
		enum ESoundNumber {
			SND_NUM_0 = 0,
			SND_NUM_1,
			SND_NUM_2,
			SND_NUM_3,
			SND_NUM_4,
			SND_NUM_5,
			SND_NUM_6,
			SND_NUM_7,
			SND_NUM_8,
			SND_NUM_9,
			SND_NUM_10,
			SND_NUM_11,
			SND_NUM_12,
			SND_NUM_13,
			SND_NUM_14,
			SND_NUM_15,
			SND_NUM_16,
			SND_NUM_17,
			SND_NUM_18,
			SND_NUM_19,
			SND_NUM_20,
			SND_NUM_30,
			SND_NUM_40,
			SND_NUM_50,
			SND_NUM_60,
			SND_NUM_70,
			SND_NUM_80,
			SND_NUM_90,
			SND_NUM_HUNDRED,
			SND_NUM_THOUSAND,
			SND_NUM_MILLION,
			SND_NUM_BILLION,
			SDN_NUM_NUMBER,
			SND_NUM_NONE
		};
		
		bool isAnyNumberPlayed();
		
		std::string m_sPathNumber;
		bool m_bIsPlayingNumber;
		int  m_iChannelNumber;
		int  m_iNumberToPlay;
		Roe::Sound *m_aSndNum;
	};
	
} // namespace Roe

#endif //ROE_COMPILE_AUDIO
#endif /*ROEAUDIOMANAGER_H_*/
