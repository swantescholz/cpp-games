#include "RoeAudioManager.h"

#ifdef ROE_COMPILE_AUDIO

#include "RoeException.h"
#include "RoeUtil.h"
#include <queue>

namespace Roe {
	
	AudioManager::AudioManager()
		: m_bIsPlayingNumber (false)
		, m_iChannelNumber   (-1)
	{
	}
	
	AudioManager::~AudioManager()
	{
	}
	
	
	void AudioManager::initAudio(int frequency, bool stereo, int chunksize, int numSoundChannels) {
		//setting up audio with defaults
		//Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3); // not used in my old version (1.2.8)
		if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, stereo ? 2 : 1, numSoundChannels) != 0) {
			Exception("Mix_OpenAudio() failed", "intiAudio", "RoeAudioManager.cpp");
		} 
		Roe::Music::init(                  "res/music/" , ".ogg"); //std. paths, change them if you need to
		Roe::Sound::init(numSoundChannels, "res/sounds/", ".ogg"); //std. paths, change them if you need to, (std:: 20 channels)
		//load music/sounds in your application with: Roe::Music::loadMusic("name"); and Roe::Sound::loadSound("name");
	}
	
	void AudioManager::quitAudio() {
		Roe::Music::quit();
		Roe::Sound::quit();
		Mix_CloseAudio();
		//Mix_Quit(); // not used in my old version
	}
	
	void AudioManager::loadEffectNumber (const std::string &psPath) {
		m_sPathNumber = psPath;
		
		for (int i = 0; i < 20; i++)
			Roe::Sound::loadSound(m_sPathNumber + Util::toString(i));
		for (int i = 20; i <= 90; i += 10)
			Roe::Sound::loadSound(m_sPathNumber + Util::toString(i));
		
		Roe::Sound::loadSound(m_sPathNumber + "hundred");
		Roe::Sound::loadSound(m_sPathNumber + "thousand");
		Roe::Sound::loadSound(m_sPathNumber + "million");
		Roe::Sound::loadSound(m_sPathNumber + "billion");
		
		m_aSndNum = new Sound[SDN_NUM_NUMBER];
		
		for (int i = 0; i < 20; i++)
			m_aSndNum[i].load(m_sPathNumber + Util::toString(i));
		
		m_aSndNum[SND_NUM_20].load(m_sPathNumber + "20");
		m_aSndNum[SND_NUM_30].load(m_sPathNumber + "30");
		m_aSndNum[SND_NUM_40].load(m_sPathNumber + "40");
		m_aSndNum[SND_NUM_50].load(m_sPathNumber + "50");
		m_aSndNum[SND_NUM_60].load(m_sPathNumber + "60");
		m_aSndNum[SND_NUM_70].load(m_sPathNumber + "70");
		m_aSndNum[SND_NUM_80].load(m_sPathNumber + "80");
		m_aSndNum[SND_NUM_90].load(m_sPathNumber + "90");
		m_aSndNum[SND_NUM_HUNDRED ].load(m_sPathNumber + "hundred");
		m_aSndNum[SND_NUM_THOUSAND].load(m_sPathNumber + "thousand");
		m_aSndNum[SND_NUM_MILLION ].load(m_sPathNumber + "million");
		m_aSndNum[SND_NUM_BILLION ].load(m_sPathNumber + "billion");
	}
	
	void AudioManager::deleteEffectNumber() {
		ROE_SAFE_DELETE_ARRAY(m_aSndNum);
		
		for (int i = 0; i < 20; i++)
			Roe::Sound::deleteSound(m_sPathNumber + Util::toString(i));
		for (int i = 30; i <= 90; i += 10)
			Roe::Sound::deleteSound(m_sPathNumber + Util::toString(i));
		
		Roe::Sound::deleteSound(m_sPathNumber + "hundred");
		Roe::Sound::deleteSound(m_sPathNumber + "thousand");
		Roe::Sound::deleteSound(m_sPathNumber + "million");
		Roe::Sound::deleteSound(m_sPathNumber + "billion");
	}
	
	void AudioManager::playEffectNumber (int piNumber) {
		m_bIsPlayingNumber = true;
		m_iChannelNumber   = -1;
		m_iNumberToPlay    = (piNumber < 0) ? (piNumber * -1) : (piNumber);
	}
	
	void AudioManager::updateAudioEffects() {
		static std::queue<int> nextNumbers; // indexes used!
		if (m_bIsPlayingNumber == true) {           // number update
			if (isAnyNumberPlayed() == false) {     // one sound finished
				if (m_iChannelNumber == -1) {
					if (m_iNumberToPlay == 0) {
						nextNumbers.push(0);
						m_iNumberToPlay = -1;
					}
				}
				if (m_iChannelNumber == -1 || Mix_Playing(m_iChannelNumber) != 1) {
					if (m_iNumberToPlay == -1 && nextNumbers.empty() == true) { // number finished
						m_bIsPlayingNumber = false;
						return;
					}
					if (nextNumbers.empty() == false) {
						m_aSndNum[nextNumbers.front()].play();
						m_iChannelNumber = m_aSndNum[nextNumbers.front()].getChannel();
						nextNumbers.pop();
					} else { // calculate numbers to play
						std::string sNum   = Util::toString(m_iNumberToPlay);
						int         length = sNum.length();
						int         subStrLength = ( (length-1) % 3) + 1;
						ESoundNumber sn = SND_NUM_NONE;
						switch ( (length-1) / 3) {
							case 1:  sn = SND_NUM_THOUSAND; break;
							case 2:  sn = SND_NUM_MILLION; break;
							case 3:  sn = SND_NUM_BILLION; break;
							default: sn = SND_NUM_NONE; break;
						}
						
						std::string sNumSub;
						for(int i = 0; i < subStrLength; i++) {
							sNumSub += sNum[i];
						}
						
						if (subStrLength >= 3) {
							nextNumbers.push(Util::parseInt(Util::toString(sNumSub[0])));
							nextNumbers.push(static_cast<int>(SND_NUM_HUNDRED));
						}
						if (subStrLength >= 2) {
							if (sNumSub.length() == 3) {
								sNumSub.erase(0,1);
							}
							if (Util::parseInt(sNumSub) >= 20) { //tys
								switch (Util::parseInt(std::string(1,sNumSub[0]))) {
									case 2: nextNumbers.push(static_cast<int>(SND_NUM_20)); break;
									case 3: nextNumbers.push(static_cast<int>(SND_NUM_30)); break;
									case 4: nextNumbers.push(static_cast<int>(SND_NUM_40)); break;
									case 5: nextNumbers.push(static_cast<int>(SND_NUM_50)); break;
									case 6: nextNumbers.push(static_cast<int>(SND_NUM_60)); break;
									case 7: nextNumbers.push(static_cast<int>(SND_NUM_70)); break;
									case 8: nextNumbers.push(static_cast<int>(SND_NUM_80)); break;
									case 9: nextNumbers.push(static_cast<int>(SND_NUM_90)); break;
									default: break;
								}
							}
							else if (sNumSub.length() == 2 && Util::parseInt(Util::toString(sNumSub[0])) != 0){ // teens
								nextNumbers.push(Util::parseInt(sNumSub));
							}
						}
						if (subStrLength == 1 || (subStrLength > 1 && Util::parseInt(sNumSub) > 20)) { // normal numbers (1 - 9)
							if (                 Util::parseInt(Util::toString(sNumSub[sNumSub.length() - 1])) != 0) {
								nextNumbers.push(Util::parseInt(Util::toString(sNumSub[sNumSub.length() - 1])));
							}
						}
						
						
						if (sn != SND_NUM_NONE)
							nextNumbers.push(static_cast<int>(sn));
						
						//adjust number
						for (int i = 0; i < subStrLength; i++) {
							sNum.erase(0,1);
						}
						
						//adjust numberToPlay
						if (sNum.length() > 0) {
							m_iNumberToPlay = Util::parseInt(sNum);
						} else {
							m_iNumberToPlay = -1; // ended
						}
					}
				}
			}
		}
	}
	
	bool AudioManager::isEffectNumberRunning() {return m_bIsPlayingNumber;}
	
	bool AudioManager::isAnyNumberPlayed() {
		for (int i = 0; i < SDN_NUM_NUMBER; i++) {
			if (m_aSndNum[i].isPlaying()) return true;
		}
		return false;
	}
	
} // namespace Roe


#endif //ROE_COMPILE_AUDIO
