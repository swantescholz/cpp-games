#include "glcSound.h"

map<string, S_SoundIntern > glcSound::c_mpSound;
map<string, S_SoundIntern >::iterator glcSound::c_mpi;
pair<string, S_SoundIntern > glcSound::c_pair;
string glcSound::c_sPathStart, glcSound::c_sPathEnd = ".ogg";
int glcSound::c_iNumChannels = 0;
int glcSound::c_iMaxChannels = 0;

glcSound::glcSound(const float fVolume)
{
	m_fVolume = fVolume;
	m_iChannel = 0;
	c_iNumChannels++;
	
	if (c_iNumChannels > c_iMaxChannels)
		throw glcException("too much sounds");
}
glcSound::glcSound(const string &sPath, const float fVolume) {
	m_fVolume = fVolume;
	m_iChannel = 0;
	c_iNumChannels++;
	
	if (c_iNumChannels > c_iMaxChannels)
		throw glcException("too much sounds");
		
	Load(sPath);
}
glcSound::glcSound(const glcSound& rhs) {
	m_sPath 	= rhs.m_sPath;
	m_sName 	= rhs.m_sName;
	m_fVolume 	= rhs.m_fVolume;
	m_iChannel 	= rhs.m_iChannel;
	c_iNumChannels++;
}
glcSound& glcSound::operator = (const glcSound& rhs)	{
	m_sPath 	= rhs.m_sPath;
	m_sName 	= rhs.m_sName;
	m_fVolume 	= rhs.m_fVolume;
	m_iChannel 	= rhs.m_iChannel;
	//dont increase numchannels, because its not a new instance
	return *this;
}

glcSound::~glcSound()
{
	c_iNumChannels--;
}

void glcSound::Play(const int iLoops) {
	m_iChannel = Mix_PlayChannel(-1, c_mpSound[m_sPath].chunk, iLoops);
	Mix_Volume(m_iChannel,MIX_MAX_VOLUME * GetStdVolume() * m_fVolume); 
}

void glcSound::Resume() {
	Mix_Resume(m_iChannel);
}

void glcSound::Pause() {
	Mix_Pause(m_iChannel);
}

void glcSound::Restart(bool bPlay) {
	Mix_HaltChannel(m_iChannel);
	if (bPlay)
		Play();
}

void glcSound::Halt() {
	Mix_HaltChannel(m_iChannel);
}


void glcSound::Load(string sPath) {
	m_sName = sPath;
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpSound.find(sPath);
	if (c_mpi == c_mpSound.end()) {
		throw glcException("requested sound not loaded");
	}
	
	m_sPath = sPath;
}

float glcSound::GetStdVolume()	{
	c_mpi = c_mpSound.find(m_sPath);
	if (c_mpi == c_mpSound.end()) {
		throw glcException("std volume cannot be found");
		return -1;
	} else {
		return c_mpi->second.fStdVolume;
	}
}

//klassen-funktionen
void glcSound::DeleteSound	(string sPath, bool bCompleteThePath)	{
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpSound.find(sPath);
	if (c_mpi == c_mpSound.end()) {
		throw glcException("sound to be deleted does not exist: " + sPath);
	} else {
		if(c_mpi->second.chunk) {
			Mix_FreeChunk(c_mpi->second.chunk);
			c_mpi->second.chunk = NULL;
		}
		c_mpSound.erase(c_mpi);
	}
}

void glcSound::LoadSound	(string sPath, const float fVolume)	{
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpSound.find(sPath);
	if (c_mpi == c_mpSound.end()) {
		S_SoundIntern sound;
		sound.chunk = Mix_LoadWAV(sPath.data());
		if (sound.chunk) {
			sound.fStdVolume = fVolume;
			
			//map erweitern
			c_pair = make_pair (sPath, sound);
			c_mpSound.insert(c_pair);
		} else {
			throw glcException("sound to load does not exist in path: " + sPath);
		}
	} else {
		throw glcException("sound " + sPath + " already loaded");
	}
}

void glcSound::Init(const int iMaxChannels, string sStart, string sEnd)	{
	Mix_AllocateChannels(iMaxChannels);
	c_iMaxChannels = iMaxChannels;
	c_sPathStart = sStart;
	c_sPathEnd = sEnd;
}
void glcSound::Quit()	{
	for(c_mpi=c_mpSound.begin(); c_mpi!=c_mpSound.end(); ++c_mpi) {
		if(c_mpi->second.chunk) {
			Mix_FreeChunk(c_mpi->second.chunk);
			c_mpi->second.chunk = NULL;
		}
	}
	c_mpSound.clear();
}
