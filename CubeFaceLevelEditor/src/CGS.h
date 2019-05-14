#ifndef CGS_H_
#define CGS_H_

#include "GLC.h"

#define g_pGS CGS::Get()
#define CUBE_SIZE 100.0f	//von -50,-50,-50 bis 50,50,50 NOTE: do not change


enum ECubeFace {
	CF_FRONT = 0,
	CF_RIGHT,
	CF_BACK,
	CF_LEFT,
	CF_TOP,
	CF_BOTTOM
};

enum EDownDir {
	DD_DOWN,
	DD_LEFT,
	DD_UP,
	DD_RIGHT
};

enum EBlockType {
	BT_SOLID,
	BT_GOAL,
	BT_KILL,
	
	BT_LAST
};

class CGS : public TSingleton<CGS>
{
public:
	CGS();
	~CGS();
	
	ECubeFace	m_UserFace;
	EDownDir	m_DownDir;
	float		m_fCamZoom;
	string		m_sArg0, m_sArg1, m_sPlayerInit;
	float		m_fRasterSize;		//w / h von einem raster
	EBlockType	m_UserBT;
};

#endif /*CGS_H_*/
