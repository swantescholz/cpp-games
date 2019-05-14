#include "CGS.h"

CGS::CGS()
{
	m_UserFace		= CF_FRONT;
	m_DownDir		= DD_DOWN;
	m_fCamZoom		= 3.0f;
	m_UserBT		= BT_SOLID;
	m_fRasterSize	= CUBE_SIZE / 10.0f;
}

CGS::~CGS()
{
	
}
