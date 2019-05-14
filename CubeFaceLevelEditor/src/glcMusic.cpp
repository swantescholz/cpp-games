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
	sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpMusic.find(sPath);
	if (c_mpi == c_mpMusic.end()) {
		throw glcException("angefragte music noch nicht geladen");
	}
	
	m_sPath = sPath;
}

float glcMusic::GetStdVolume()	{
	c_mpi = c_mpMusic.find(m_sPath);
	if (c_mpi == c_mpMusic.end()) {
		throw glcException("std volume kann nicht gefunden werden");
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
			throw glcException("music zum laden existiert im pfad nicht: " + sPath);
		}
	} else {
		throw glcException("music " + sPath + " wurde schon geladen");
	}
}
void glcMusic::DeleteMusic	(string sPath, bool bCompleteThePath)	{
	if (bCompleteThePath)
		sPath = c_sPathStart + sPath + c_sPathEnd;
	
	c_mpi = c_mpMusic.find(sPath);
	if (c_mpi == c_mpMusic.end()) {
		throw glcException("sound to be deleted does not exist");
	} else {
		if(c_mpi->second.music) {
			Mix_FreeMusic(c_mpi->second.music);
			c_mpi->second.music = NULL;
		}
		c_mpMusic.erase(c_mpi);
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
