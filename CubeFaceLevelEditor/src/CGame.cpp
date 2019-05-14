#include "CGame.h"

CGame::CGame() : m_fCamMoveSpeed(CUBE_SIZE / 10.0f)
{
	
	
	glClearColor(0.5,0.7,0.9,0);
	
	glFogi (GL_FOG_MODE, GL_LINEAR);
	glFogf (GL_FOG_START, ((CUBE_SIZE / 2.0f) * g_pGS->m_fCamZoom ) - (CUBE_SIZE / 2.0f) );
	glFogf (GL_FOG_END  , ((CUBE_SIZE / 2.0f) * g_pGS->m_fCamZoom ) + ( (CUBE_SIZE / 2.0f) * 1.5f ) ); //wert btw: 1-2.5 (std: 1.5)
	
	//startwert
	VCAM2 = tbVector3 (0.0f, 0.0f, 0.0f);
	switch (g_pGS->m_UserFace) {
		case CF_FRONT:	VCAM2 = tbVector3( 0.0f, 0.0f, CUBE_SIZE / 2.0f); break;
		case CF_RIGHT:	VCAM2 = tbVector3( CUBE_SIZE / 2.0f, 0.0f, 0.0f); break;
		case CF_BACK:	VCAM2 = tbVector3( 0.0f, 0.0f,-CUBE_SIZE / 2.0f); break;
		case CF_LEFT:	VCAM2 = tbVector3(-CUBE_SIZE / 2.0f, 0.0f, 0.0f); break;
		case CF_TOP:	VCAM2 = tbVector3( 0.0f, CUBE_SIZE / 2.0f, 0.0f); break;
		case CF_BOTTOM:	VCAM2 = tbVector3( 0.0f,-CUBE_SIZE / 2.0f, 0.0f); break;
	}
}

CGame::~CGame()
{
	
}

void CGame::Update() {
	g_pLevel->Update();
	
	if (glcKeyWasDown(SDLK_F1)) {		//speichern
		g_pLevel->SaveLevel();
	} else if (glcKeyWasDown(SDLK_F2)) { //reload
		string sTmp = g_pGS->m_sArg1;
		CGS::Del();
		CLevel::Del();
		g_pLevel->LoadLevel(sTmp);
		g_pGS->m_sArg1 = sTmp;
		//startwert
		VCAM2 = tbVector3 (0.0f, 0.0f, 0.0f);
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:	VCAM2 = tbVector3( 0.0f, 0.0f, CUBE_SIZE / 2.0f); break;
			case CF_RIGHT:	VCAM2 = tbVector3( CUBE_SIZE / 2.0f, 0.0f, 0.0f); break;
			case CF_BACK:	VCAM2 = tbVector3( 0.0f, 0.0f,-CUBE_SIZE / 2.0f); break;
			case CF_LEFT:	VCAM2 = tbVector3(-CUBE_SIZE / 2.0f, 0.0f, 0.0f); break;
			case CF_TOP:	VCAM2 = tbVector3( 0.0f, CUBE_SIZE / 2.0f, 0.0f); break;
			case CF_BOTTOM:	VCAM2 = tbVector3( 0.0f,-CUBE_SIZE / 2.0f, 0.0f); break;
		}
	}
	if (glcKeyWasDown(SDLK_KP1)) {		//drehen
		switch (g_pGS->m_DownDir) {
			case DD_DOWN: 	g_pGS->m_DownDir = DD_RIGHT;break;
			case DD_RIGHT: 	g_pGS->m_DownDir = DD_UP; 	break;
			case DD_UP: 	g_pGS->m_DownDir = DD_LEFT; break;
			case DD_LEFT: 	g_pGS->m_DownDir = DD_DOWN; break;
		}
	} else if (glcKeyWasDown(SDLK_KP2)) {
		switch (g_pGS->m_DownDir) {
			case DD_DOWN: 	g_pGS->m_DownDir = DD_LEFT; break;
			case DD_RIGHT: 	g_pGS->m_DownDir = DD_DOWN;	break;
			case DD_UP: 	g_pGS->m_DownDir = DD_RIGHT;break;
			case DD_LEFT: 	g_pGS->m_DownDir = DD_UP;   break;
		}
	}
	
	if (glcKeyWasDown(SDLK_KP4)) {		//blocktyp waehlen
		if (g_pGS->m_UserBT == 0)
			g_pGS->m_UserBT = BT_LAST;
		g_pGS->m_UserBT = static_cast<EBlockType>(g_pGS->m_UserBT - 1);
	} else if (glcKeyWasDown(SDLK_KP5)) {
		g_pGS->m_UserBT = static_cast<EBlockType>(g_pGS->m_UserBT + 1);
		if (g_pGS->m_UserBT == BT_LAST)
			g_pGS->m_UserBT = static_cast<EBlockType>(0);;
	}
	
	if (glcKeyWasDown(SDLK_KP7)) {		//raster veraendern
		if (g_pGS->m_fRasterSize > CUBE_SIZE / 100.0f)		//testwert
			g_pGS->m_fRasterSize /= 2.0f;
	} else if (glcKeyWasDown(SDLK_KP8)) {
		if (g_pGS->m_fRasterSize < CUBE_SIZE / 2.0f)		//testwert
			g_pGS->m_fRasterSize *= 2.0f;
	}
	
	if (g_pGL->MouseWasDown(SDL_BUTTON_WHEELUP)) {	//camera zoomen
		if (g_pGS->m_fCamZoom > 1.0f) {
			g_pGS->m_fCamZoom *= 0.9f;
		}
	} else if (g_pGL->MouseWasDown(SDL_BUTTON_WHEELDOWN)) {
		if (g_pGS->m_fCamZoom < 20.0f) {	//testwert
			g_pGS->m_fCamZoom *= 1.1f;
		}
	}
	
	if (g_pGL->MouseIsDown(SDL_BUTTON_RIGHT)) {		//block loeschen und einfuegen und verschieben
		g_pLevel->DeleteBlock();
	} else if (g_pGL->MouseIsDown(SDL_BUTTON_LEFT)) {
		g_pLevel->LeftClick();
	}
	
	
	bool bFaceSwitched = true;
	if (glcKeyWasDown(SDLK_UP))  {						//face wechseln durch user
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:
			case CF_RIGHT:
			case CF_BACK:
			case CF_LEFT:	g_pGS->m_UserFace = CF_TOP; break;
			case CF_TOP:	g_pGS->m_UserFace = CF_BACK; break;
			case CF_BOTTOM:	g_pGS->m_UserFace = CF_FRONT; break;
		}
	} else if (glcKeyWasDown(SDLK_DOWN)) {
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:
			case CF_RIGHT:
			case CF_BACK:
			case CF_LEFT:	g_pGS->m_UserFace = CF_BOTTOM; break;
			case CF_TOP:	g_pGS->m_UserFace = CF_FRONT; break;
			case CF_BOTTOM:	g_pGS->m_UserFace = CF_BACK; break;
		}
	}  else if (glcKeyWasDown(SDLK_LEFT)) {
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:	g_pGS->m_UserFace = CF_LEFT; break;
			case CF_RIGHT:	g_pGS->m_UserFace = CF_FRONT; break;
			case CF_BACK:	g_pGS->m_UserFace = CF_RIGHT; break;
			case CF_LEFT:	g_pGS->m_UserFace = CF_BACK; break;
			case CF_TOP:	g_pGS->m_UserFace = CF_LEFT; break;
			case CF_BOTTOM:	g_pGS->m_UserFace = CF_LEFT; break;
		}
	}  else if (glcKeyWasDown(SDLK_RIGHT)) {
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:	g_pGS->m_UserFace = CF_RIGHT; break;
			case CF_RIGHT:	g_pGS->m_UserFace = CF_BACK; break;
			case CF_BACK:	g_pGS->m_UserFace = CF_LEFT; break;
			case CF_LEFT:	g_pGS->m_UserFace = CF_FRONT; break;
			case CF_TOP:	g_pGS->m_UserFace = CF_RIGHT; break;
			case CF_BOTTOM:	g_pGS->m_UserFace = CF_RIGHT; break;
		}
	} else {
		bFaceSwitched = false;
	}
	
	
	
	//cams
	if (bFaceSwitched) {							//cam2 bestimmen
		VCAM2 = tbVector3 (0.0f, 0.0f, 0.0f);
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:	VCAM2 = tbVector3( 0.0f, 0.0f, CUBE_SIZE / 2.0f); break;
			case CF_RIGHT:	VCAM2 = tbVector3( CUBE_SIZE / 2.0f, 0.0f, 0.0f); break;
			case CF_BACK:	VCAM2 = tbVector3( 0.0f, 0.0f,-CUBE_SIZE / 2.0f); break;
			case CF_LEFT:	VCAM2 = tbVector3(-CUBE_SIZE / 2.0f, 0.0f, 0.0f); break;
			case CF_TOP:	VCAM2 = tbVector3( 0.0f, CUBE_SIZE / 2.0f, 0.0f); break;
			case CF_BOTTOM:	VCAM2 = tbVector3( 0.0f,-CUBE_SIZE / 2.0f, 0.0f); break;
		}
	}
	
	
	switch (g_pGS->m_UserFace) {					//cam3 bestimmen
		case CF_FRONT:
			switch (g_pGS->m_DownDir) {
				case DD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case DD_RIGHT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
				case DD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case DD_LEFT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
			}
			break;
		case CF_RIGHT:
			switch (g_pGS->m_DownDir) {
				case DD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case DD_RIGHT: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
				case DD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case DD_LEFT: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
			}
			break;
		case CF_BACK:
			switch (g_pGS->m_DownDir) {
				case DD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case DD_RIGHT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
				case DD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case DD_LEFT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
			}
			break;
		case CF_LEFT:
			switch (g_pGS->m_DownDir) {
				case DD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case DD_RIGHT: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
				case DD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case DD_LEFT: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
			}
			break;
		case CF_TOP:
			switch (g_pGS->m_DownDir) {
				case DD_DOWN: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
				case DD_RIGHT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
				case DD_UP: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
				case DD_LEFT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
			}
			break;
		case CF_BOTTOM:
			switch (g_pGS->m_DownDir) {
				case DD_DOWN: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
				case DD_RIGHT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
				case DD_UP: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
				case DD_LEFT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
			}
			break;
	}
	
	
	
	if (glcKeyIsDown(SDLK_w)) {								//cam1 verschieben und bestimmen
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
		}
	} else if (glcKeyIsDown(SDLK_s)) {
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
		}
	}
	if (glcKeyIsDown(SDLK_d)) {
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
		}
	} else if (glcKeyIsDown(SDLK_a)) {
		switch (g_pGS->m_UserFace) {
			case CF_FRONT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.y -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.y += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_DownDir) {
					case DD_DOWN: 	VCAM2.x -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_RIGHT: 	VCAM2.z -= m_fCamMoveSpeed * ELAPSED; break;
					case DD_UP: 	VCAM2.x += m_fCamMoveSpeed * ELAPSED; break;
					case DD_LEFT: 	VCAM2.z += m_fCamMoveSpeed * ELAPSED; break;
				}
				break;
		}
	}
	
	VCAM1 = VCAM2;
	switch (g_pGS->m_UserFace) {
		case CF_FRONT: 	VCAM1.z = VCAM2.z * g_pGS->m_fCamZoom; break;
		case CF_RIGHT: 	VCAM1.x = VCAM2.x * g_pGS->m_fCamZoom; break;
		case CF_BACK: 	VCAM1.z = VCAM2.z * g_pGS->m_fCamZoom; break;
		case CF_LEFT: 	VCAM1.x = VCAM2.x * g_pGS->m_fCamZoom; break;
		case CF_TOP: 	VCAM1.y = VCAM2.y * g_pGS->m_fCamZoom; break;
		case CF_BOTTOM: VCAM1.y = VCAM2.y * g_pGS->m_fCamZoom; break;
	}
}

void CGame::Render() {
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// We don't want to modify the projection matrix.
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
	
	//cameramatrix festsetzen (zuallererst!)
	g_pGL->LookAt();
	
	//todo
	g_pLevel->Render();
	
	
	glcFlip();
}

