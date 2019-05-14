#include "RoeSound.h"

#ifdef ROE_COMPILE_AUDIO

#include "RoeException.h"

namespace Roe {

	std::map <std::string, S_SoundIntern >              Sound::s_mpSound;
	std::map <std::string, S_SoundIntern >::iterator    Sound::s_mpi;
	std::pair<std::string, S_SoundIntern >              Sound::s_pair;
	std::map <std::string, int >                        Sound::s_mpSoundChannel;
	std::map <std::string, bool>                        Sound::s_mpSoundPlaying;
	std::string                                         Sound::s_sPathStart = "";
	std::string                                         Sound::s_sPathEnd   = ".ogg";
	int   Sound::s_iMaxChannels     = 0;
	bool *Sound::s_abChannelPlaying = NULL;
	
	Sound::Sound(const float fVolume)
	{
		m_fVolume = fVolume;
		m_iChannel = 0;
	}
	Sound::Sound(const std::string &sPath, const float fVolume) {
		m_fVolume = fVolume;
		m_iChannel = 0;
		
		load(sPath);
	}
	Sound::Sound(const Sound& rhs) {
		m_sPath     = rhs.m_sPath;
		m_sName     = rhs.m_sName;
		m_fVolume   = rhs.m_fVolume;
		m_iChannel  = rhs.m_iChannel;
	}
	Sound& Sound::operator = (const Sound& rhs) {
		m_sPath     = rhs.m_sPath;
		m_sName     = rhs.m_sName;
		m_fVolume   = rhs.m_fVolume;
		m_iChannel  = rhs.m_iChannel;
		//dont increase numchannels, because its not a new instance
		return *this;
	}
	
	Sound::~Sound()
	{
	}
	
	void Sound::play(const int iLoops) {
		if (getNumberOfChannelsPlaying() >= s_iMaxChannels)
			throw Exception("too many requested channels for sounds", "play", "RoeSound.cpp");
			
		m_iChannel = Mix_PlayChannel(-1, s_mpSound[m_sPath].chunk, iLoops);
		Mix_Volume(m_iChannel, MIX_MAX_VOLUME * getStdVolume() * m_fVolume);
		s_abChannelPlaying[m_iChannel] = true;
		s_mpSoundChannel[m_sPath]      = m_iChannel;
		s_mpSoundPlaying[m_sPath]      = true;
	}
	
	void Sound::resume() {
		Mix_Resume(m_iChannel);
	}
	
	void Sound::pause() {
		Mix_Pause(m_iChannel);
	}
	
	void Sound::restart(bool bPlay) {
		Mix_HaltChannel(m_iChannel);
		if (bPlay)
			play();
	}
	
	void Sound::halt() {
		Mix_HaltChannel(m_iChannel);
	}
	
	
	void Sound::load(std::string sPath) {
		m_sName = sPath;
		sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpSound.find(sPath);
		if (s_mpi == s_mpSound.end()) {
			throw Exception("requested sound does not exist: " + sPath, "load", "RoeSound.cpp");
		}
		
		m_sPath = sPath;
	}
	
	float Sound::getStdVolume() {
		s_mpi = s_mpSound.find(m_sPath);
		if (s_mpi == s_mpSound.end()) {
			throw Exception("standard volume could not be found", "getStdVolume", "RoeSound.cpp");
			return -1;
		} else {
			return s_mpi->second.fStdVolume;
		}
	}
	
	//static
	void Sound::loadSound (std::string sPath, const float fVolume)	{
		sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpSound.find(sPath);
		if (s_mpi == s_mpSound.end()) {
			S_SoundIntern sound;
			sound.chunk = Mix_LoadWAV(sPath.c_str());
			if (sound.chunk) {
				sound.fStdVolume = fVolume;
				
				//map inserting
				s_pair = make_pair (sPath, sound);
				s_mpSound.insert(s_pair);
				
				std::pair<std::string, int > p1;
				p1 = make_pair(sPath, -1);
				s_mpSoundChannel.insert(p1);
				
				std::pair<std::string, bool> p2;
				p2 = make_pair(sPath, false);
				s_mpSoundPlaying.insert(p2);
			} else {
				throw Exception("sound to load does not exist in path: " + sPath, "loadSound", "RoeSound.cpp");
			}
		} else {
			throw Exception("sound already loaded: " + sPath, "loadSound", "RoeSound.cpp");
		}
	}
	
	void Sound::deleteSound	(std::string sPath, bool bCompleteThePath)	{
		if (bCompleteThePath)
			sPath = s_sPathStart + sPath + s_sPathEnd;
		
		s_mpi = s_mpSound.find(sPath);
		if (s_mpi == s_mpSound.end()) {
			throw Exception("sound to be deleted does not exist: " + sPath, "deleteSound", "RoeSound.cpp");
		} else {
			if(s_mpi->second.chunk) {
				Mix_FreeChunk(s_mpi->second.chunk);
				s_mpi->second.chunk = NULL;
			}
			s_mpSound.erase(s_mpi);
			s_mpSoundChannel.erase(sPath);
			s_mpSoundPlaying.erase(sPath);
		}
	}
	
	bool Sound::isChannelPlaying (int channel) {
		if (channel < 0) return false;
		return Mix_Playing(channel);
	}
	
	int Sound::getNumberOfChannelsPlaying () {
		return Mix_Playing(-1);
	}
	
	
	void Sound::init(const int iMaxChannels, std::string sStart, std::string sEnd)	{
		Mix_AllocateChannels(iMaxChannels);
		s_iMaxChannels = iMaxChannels;
		s_sPathStart   = sStart;
		s_sPathEnd     = sEnd;
		s_abChannelPlaying = new bool[s_iMaxChannels];
		for (int i = 0; i < s_iMaxChannels; i++) {
			s_abChannelPlaying[i] = false;
		}
		
		Mix_ChannelFinished(channelFinished); // register callback method
	}
	void Sound::quit()	{
		for(s_mpi = s_mpSound.begin(); s_mpi != s_mpSound.end(); ++s_mpi) {
			if(s_mpi->second.chunk) {
				Mix_FreeChunk(s_mpi->second.chunk);
				s_mpi->second.chunk = NULL;
			}
		}
		s_mpSound.clear();
		s_mpSoundChannel.clear();
		s_mpSoundPlaying.clear();
		ROE_SAFE_DELETE_ARRAY(s_abChannelPlaying);
	}
	
	void Sound::channelFinished (int channel) {
		s_abChannelPlaying[channel] = false;
		std::map<std::string, int>::iterator it;
		for (it = s_mpSoundChannel.begin(); it != s_mpSoundChannel.end(); it++) {
			if (it->second == channel) {
				it->second = -1;
				s_mpSoundPlaying[it->first] = false;
			}
		}
	}

} // namespace Roe

#endif //ROE_COMPILE_AUDIO
