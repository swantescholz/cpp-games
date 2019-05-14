#include "glcMusic.h"

map<string, S_MusicIntern > glcMusic::c_mpMusic;
map<string, S_MusicIntern >::iterator glcMusic::c_mpi;
pair<string, S_MusicIntern > glcMusic::c_pair;
string glcMusic::c_sPathStart, glcMusic::c_sPathEnd = ".ogg";


glcMusic::glcMusic(const float fVolume)
{
	m_fVolume = fVolume;
}

glcMusic::glcMusic(const string &sPath, const float fVolume) {
	m_fVolume = fVolume;
		
	Load(sPath);
}

glcMusic::~glcMusic()
{
}

void glcMusic::Load(string sPath) {
	m_sName = sPath;
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpMusic.find(sPath);
	if (c_mpi == c_mpMusic.end()) {
		throw glcException("requested music not loaded");
	}
	
	m_sPath = sPath;
}

float glcMusic::GetStdVolume()	{
	c_mpi = c_mpMusic.find(m_sPath);
	if (c_mpi == c_mpMusic.end()) {
		throw glcException("std volume cannot be found");
		return -1;
	} else {
		return c_mpi->second.fStdVolume;
	}
}

void glcMusic::Play(const int loops, const double dFadeIn, const double dPos) {
	Mix_VolumeMusic(MIX_MAX_VOLUME * GetStdVolume() * m_fVolume);
	Mix_FadeInMusicPos(c_mpMusic[m_sPath].music, loops, static_cast<int> (dFadeIn * 1000) , dPos);
}



void glcMusic::LoadMusic(string sPath, const float fVolume, const double dFadeIn, const double dPos) {
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpMusic.find(sPath);
	if (c_mpi == c_mpMusic.end()) {
		S_MusicIntern music;
		music.music = Mix_LoadMUS(sPath.data());
		if (music.music) {
			music.fStdVolume = fVolume;
			music.dStdFadeIn = dFadeIn;
			music.dStdPos	 = dPos;
			
			//map erweitern
			c_pair = make_pair (sPath, music);
			c_mpMusic.insert(c_pair);
		} else {
			throw glcException("music to load does not exist in path: " + sPath);
		}
	} else {
		throw glcException("music " + sPath + " already loaded");
	}
}
void glcMusic::DeleteMusic	(string sPath, bool bCompleteThePath)	{
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpMusic.find(sPath);
	if (c_mpi == c_mpMusic.end()) {
		throw glcException("music to be deleted does not exist");
	} else {
		if(c_mpi->second.music) {
			Mix_FreeMusic(c_mpi->second.music);
			c_mpi->second.music = NULL;
		}
		c_mpMusic.erase(c_mpi);
	}
}
bool glcMusic::IsMusicLoaded(string sPath, bool bCompleteThePath) {
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpMusic.find(sPath);
	if (c_mpi == c_mpMusic.end()) {
		return false;
	} else {
		return true;
	}
}

void glcMusic::Init(string sStart, string sEnd)	{
	c_sPathStart = sStart;
	c_sPathEnd = sEnd;
}
void glcMusic::Quit()	{
	for(c_mpi=c_mpMusic.begin(); c_mpi!=c_mpMusic.end(); ++c_mpi) {
		if(c_mpi->second.music) {
			Mix_FreeMusic(c_mpi->second.music);
			c_mpi->second.music = NULL;
		}
	}
	c_mpMusic.clear();
}
