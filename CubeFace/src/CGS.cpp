#include "CGS.h"

CGS::CGS()
{
	m_GameState			= GS_INTRO;
	m_PlayerFace		= CF_FRONT;
	m_GravitationDir	= GD_DOWN;
	
	ifstream dat_in;
	dat_in.open("save/progress.bin", ios::binary);
	if (dat_in == NULL) {
		m_Progress.iNumBeatenLevels = 0;
	} else {
		dat_in.read((char*) &m_Progress, sizeof(m_Progress));
	}
	dat_in.close();
}

CGS::~CGS()
{
	ofstream dat_out("save/progress.bin", ios::binary);
	dat_out.write((char*) &m_Progress, sizeof(m_Progress));
	dat_out.close();
}
