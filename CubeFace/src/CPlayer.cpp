#include "CPlayer.h"

CPlayer::CPlayer() :
	m_sndJump("jump", 1.0f), m_sndKill("kill", 1.0f), m_sndFinish("finish", 1.0f), m_sndReset("reset", 0.5f)
{
	g_Logfile << "Player wird initialisiert";
	
	m_Tex.Load("player");
	m_bInAir = true;
	m_fYSpeed = 0.0f;
	m_iMaxJumps = 2;	//std: 2 -> double-jump
	m_iNumJumps = 1;	//weil man beim start faellt
	
	//player infos aus string auslesen
	string sLoad = g_pLevel->GetPlayerInitString();
	
	//cam2
	m_Cam2Style = static_cast<ECam2Style> ( strtoul((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL, 10) );
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fCamZoom = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	
	//physik
	m_fYSpeedDecreasePerSec = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fYStartSpeed 			= strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fYMaxSpeed 			= strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fXSpeedDec			= strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fXSpeedAcc			= strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fXMaxSpeed			= strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_fXSpeed = 0.0f;
	
	//position
	m_vWH.y = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_vWH.x = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_vStartPos.y = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	m_vStartPos.x = strtod((sLoad.substr(sLoad.rfind(" ") + 1)).data(), NULL);
	sLoad.erase(sLoad.rfind(" ")); while (sLoad[static_cast<int>(sLoad.length())-1] == ' ') {sLoad.erase(sLoad.rfind(" "));}
	
	m_StartFace = static_cast<ECubeFace> (strtoul(sLoad.data(),NULL,10));
	sLoad.clear();
	
	m_vPos = m_vStartPos;
	m_Face = m_StartFace;
	
	//bei start in block?
	switch (g_pLevel->Collision(m_Face, m_vPos, m_vWH)) {
		case CS_NO:							break;
		
		default:
			throw glcException("player at start in block!");
			break;
	}
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update() {
	if (glcKeyWasDown(SDLK_r)) {	//restart
		Reset();
	} else {
	
		tbVector2 vTmpLastPos = m_vPos;
		ECubeFace tmpLastFace = m_Face;
		EGravitationDirection tmpLastGD = g_pGS->m_GravitationDir;
		
		
		//Bewegung X
		if (glcKeyIsDown(SDLK_LEFT)) 		{
			if (m_fXSpeed > -m_fXMaxSpeed) m_fXSpeed -= m_fXSpeedAcc * ELAPSED;
		} else if (glcKeyIsDown(SDLK_RIGHT)) 	{
			if (m_fXSpeed <  m_fXMaxSpeed) m_fXSpeed += m_fXSpeedAcc * ELAPSED;
		} else {
			m_fXSpeed *= pow(m_fXSpeedDec, ELAPSED);	//abschwaechung
		}
		m_vPos.x += m_fXSpeed * ELAPSED;
		
		SwitchFace(); //seitenwechsel X
		//collision X
		switch (g_pLevel->Collision(m_Face, m_vPos, m_vWH)) {
			case CS_NO:							break;
			
			case CS_SOLID:
				m_fXSpeed = 0.0f;
				m_vPos = vTmpLastPos;
				m_Face = tmpLastFace;
				g_pGS->m_GravitationDir = tmpLastGD;
				break;
			case CS_KILL:	DoKilled();			break;
			case CS_GOAL:	DoFinished();		break;
			
			default: break;
		}
		
		//Bewegung Y
		SwitchFace(); //zum Updaten
		vTmpLastPos = m_vPos;
		tmpLastFace = m_Face;
		if ( (glcKeyWasDown(SDLK_SPACE) || glcKeyWasDown(SDLK_UP) ) && (!m_bInAir || m_iNumJumps < m_iMaxJumps) )		{
			m_sndJump.Play();
			m_iNumJumps++;
			m_bInAir = true;
			m_fYSpeed = m_fYStartSpeed;
		} else if (!m_bInAir) {
			switch (g_pLevel->Collision(m_Face, tbVector2(m_vPos.x, m_vPos.y - 1.0f /*testwert*/), m_vWH)) {
				case CS_NO:
					m_iNumJumps++;	//ein sprung weniger
					m_bInAir = true;
					m_fYSpeed = 0.0f;
					break;
				case CS_KILL:	DoKilled();			break;
				case CS_GOAL:	DoFinished();		break;
				
				default: break;
			}
		}
		if (m_bInAir) {
			if (m_fYSpeed < m_fYMaxSpeed && m_fYSpeed > -m_fYMaxSpeed) {
				m_fYSpeed -= m_fYSpeedDecreasePerSec * ELAPSED;
			}
			m_vPos.y += m_fYSpeed * ELAPSED;
		} else {
			m_iNumJumps = 0;
		}
		
		SwitchFace(); //seitenwechsel Y
		//collision Y
		switch (g_pLevel->Collision(m_Face, m_vPos, m_vWH)) {
			case CS_NO:							break;
			
			case CS_SOLID:
				m_vPos = vTmpLastPos;
				m_Face = tmpLastFace;
				g_pGS->m_GravitationDir = tmpLastGD;
				g_pGS->m_PlayerFace = m_Face;
				if (m_fYSpeed > 0.0f) {
					m_fYSpeed = 0.0f;
				} else {
					m_bInAir = false;
					m_iNumJumps = 0;
				}
				break;
			case CS_KILL:	DoKilled();			break;
			case CS_GOAL:	DoFinished();		break;
			
			default: break;
		}
	}
	
	SetCamera();
}

void CPlayer::Render() {
	//coordinatensystem in die mitte verschieben, (ist einfacher)
	m_vPos -= tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
	
	tbVector2 vTmp[4];
	vTmp[0] = m_vPos;
	vTmp[1] = tbVector2 (m_vPos.x + m_vWH.x, m_vPos.y);
	vTmp[2] = m_vPos + m_vWH;
	vTmp[3] = tbVector2 (m_vPos.x, m_vPos.y + m_vWH.y);
	
	
	tbVector3 vTmp3[4];
	
	//todo wegen gravitation (TODO test)
	for (int i = 0; i < 4; i++)
		vTmp3[i] = tbVector3(vTmp[i].x, vTmp[i].y, CUBE_SIZE / 2.0f);
	//noch drehen
	switch (g_pGS->m_GravitationDir) {
		case GD_UP:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD(180)) );
			break;
		case GD_DOWN:  break;
		case GD_LEFT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD(-90)) );
			break;
		case GD_RIGHT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationZ(TB_DEG_TO_RAD( 90)) );
			break;
			
		default: break;
	}
		
	switch (m_Face) {
		case CF_FRONT: break;
		case CF_RIGHT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationY(TB_DEG_TO_RAD(90)) );
			break;
		case CF_BACK:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationY(TB_DEG_TO_RAD(180)) );
			break;
		case CF_LEFT:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationY(TB_DEG_TO_RAD(-90)) );
			break;
		case CF_TOP:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationX(TB_DEG_TO_RAD(-90)) );
			break;
		case CF_BOTTOM:
			for (int i = 0; i < 4; i++)
				vTmp3[i] = tbVector3TransformCoords(vTmp3[i], tbMatrixRotationX(TB_DEG_TO_RAD( 90)) );
			break;
		
		default: break;
	}
	
	//rendern mit texture-mapping
	m_Tex.Bind();
	glBegin (GL_QUADS);
		glTexCoord2i(0,0); glVertex3fv(vTmp3[0].c);
		glTexCoord2i(1,0); glVertex3fv(vTmp3[1].c);
		glTexCoord2i(1,1); glVertex3fv(vTmp3[2].c);
		glTexCoord2i(0,1); glVertex3fv(vTmp3[3].c);
	glEnd();
	
	//coordinatensystem zurueckverschieben
	m_vPos += tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
}

void CPlayer::Reset() {
	m_sndReset.Play();
	
	m_vPos = m_vStartPos;
	m_Face = m_StartFace;
	g_pGS->m_GravitationDir = GD_DOWN;
	
	m_bInAir = true;
	m_fXSpeed = 0.0f;
	m_fYSpeed = 0.0f;
	m_iNumJumps = 1;
}
void CPlayer::DoKilled() {
	m_sndKill.Play();
	
	m_vPos = m_vStartPos;
	m_Face = m_StartFace;
	g_pGS->m_GravitationDir = GD_DOWN;
	
	m_bInAir = true;
	m_fXSpeed = 0.0f;
	m_fYSpeed = 0.0f;
	m_iNumJumps = 1;
	
	while (!glcKeyIsDown(SDLK_DOWN) && !glcKeyIsDown(SDLK_SPACE) && !glcKeyIsDown(SDLK_RETURN) && !glcKeyIsDown(SDLK_ESCAPE) && !g_pGL->MouseQuit()) {glcUpdate();}
}

void CPlayer::DoFinished() {
	m_sndFinish.Play();
	while (!glcKeyIsDown(SDLK_DOWN) && !glcKeyIsDown(SDLK_SPACE) && !glcKeyIsDown(SDLK_RETURN) && !glcKeyIsDown(SDLK_ESCAPE) && !g_pGL->MouseQuit()) {glcUpdate();}
	g_pGS->m_GameState = GS_MAIN_MENU;
	g_pGS->m_GravitationDir = GD_DOWN;
	if (g_pGS->m_iLevelToPlay == g_pGS->m_Progress.iNumBeatenLevels)
		g_pGS->m_Progress.iNumBeatenLevels++;
	g_pGS->m_iLevelToPlay = 0;
}

void CPlayer::SetCamera() {
	//coordinatensystem in die mitte verschieben, (ist einfacher)
	m_vPos -= tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
	//pos in center von player verschieben (ist besser fuer die camera)
	m_vPos += m_vWH / 2.0f;
	
	//3d-center von player vuer cam ermitteln
	switch (m_Face) {
		case CF_FRONT:
			switch (g_pGS->m_GravitationDir) {
				case GD_UP:			m_vPos3 = tbVector3 (-m_vPos.x,-m_vPos.y, CUBE_SIZE / 2.0f); break;
				case GD_DOWN:		m_vPos3 = tbVector3 ( m_vPos.x, m_vPos.y, CUBE_SIZE / 2.0f); break;
				case GD_LEFT:		m_vPos3 = tbVector3 ( m_vPos.y,-m_vPos.x, CUBE_SIZE / 2.0f); break;
				case GD_RIGHT:		m_vPos3 = tbVector3 (-m_vPos.y, m_vPos.x, CUBE_SIZE / 2.0f); break;
			}
			break;
		case CF_RIGHT:
			switch (g_pGS->m_GravitationDir) {
				case GD_UP:			m_vPos3 = tbVector3 (CUBE_SIZE / 2.0f, -m_vPos.y, m_vPos.x); break;
				case GD_DOWN:		m_vPos3 = tbVector3 (CUBE_SIZE / 2.0f,  m_vPos.y,-m_vPos.x); break;
				case GD_LEFT:		m_vPos3 = tbVector3 (CUBE_SIZE / 2.0f, -m_vPos.x,-m_vPos.y); break;
				case GD_RIGHT:		m_vPos3 = tbVector3 (CUBE_SIZE / 2.0f,  m_vPos.x, m_vPos.y); break;
			}
			break;
		case CF_BACK:
			switch (g_pGS->m_GravitationDir) {
				case GD_UP:			m_vPos3 = tbVector3 ( m_vPos.x,-m_vPos.y,-CUBE_SIZE / 2.0f); break;
				case GD_DOWN:		m_vPos3 = tbVector3 (-m_vPos.x, m_vPos.y,-CUBE_SIZE / 2.0f); break;
				case GD_LEFT:		m_vPos3 = tbVector3 (-m_vPos.y,-m_vPos.x,-CUBE_SIZE / 2.0f); break;
				case GD_RIGHT:		m_vPos3 = tbVector3 ( m_vPos.y, m_vPos.x,-CUBE_SIZE / 2.0f); break;
			}
			break;
		case CF_LEFT:
			switch (g_pGS->m_GravitationDir) {
				case GD_UP:			m_vPos3 = tbVector3 (-CUBE_SIZE / 2.0f, -m_vPos.y,-m_vPos.x); break;
				case GD_DOWN:		m_vPos3 = tbVector3 (-CUBE_SIZE / 2.0f,  m_vPos.y, m_vPos.x); break;
				case GD_LEFT:		m_vPos3 = tbVector3 (-CUBE_SIZE / 2.0f, -m_vPos.x, m_vPos.y); break;
				case GD_RIGHT:		m_vPos3 = tbVector3 (-CUBE_SIZE / 2.0f,  m_vPos.x,-m_vPos.y); break;
			}
			break;
		case CF_TOP:
			switch (g_pGS->m_GravitationDir) {
				case GD_UP:			m_vPos3 = tbVector3 (-m_vPos.x, CUBE_SIZE / 2.0f, m_vPos.y); break;
				case GD_DOWN:		m_vPos3 = tbVector3 ( m_vPos.x, CUBE_SIZE / 2.0f,-m_vPos.y); break;
				case GD_LEFT:		m_vPos3 = tbVector3 ( m_vPos.y, CUBE_SIZE / 2.0f, m_vPos.x); break;
				case GD_RIGHT:		m_vPos3 = tbVector3 (-m_vPos.y, CUBE_SIZE / 2.0f,-m_vPos.x); break;
			}
			break;
		case CF_BOTTOM:
			switch (g_pGS->m_GravitationDir) {
				case GD_UP:			m_vPos3 = tbVector3 (-m_vPos.x,-CUBE_SIZE / 2.0f,-m_vPos.y); break;
				case GD_DOWN:		m_vPos3 = tbVector3 ( m_vPos.x,-CUBE_SIZE / 2.0f, m_vPos.y); break;
				case GD_LEFT:		m_vPos3 = tbVector3 ( m_vPos.y,-CUBE_SIZE / 2.0f,-m_vPos.x); break;
				case GD_RIGHT:		m_vPos3 = tbVector3 (-m_vPos.y,-CUBE_SIZE / 2.0f, m_vPos.x); break;
			}
			break;
		
		default: break;
	}
	
	//pos in ecke von player zurueckverschieben
	m_vPos -= m_vWH / 2.0f;
	//coordinatensystem zurueckverschieben
	m_vPos += tbVector2(CUBE_SIZE / 2.0f, CUBE_SIZE / 2.0f);
	
	
	tbVector3 vPlayer = m_vPos3;
	//camera (TODO: better)
	//vcam2
	VCAM2 = tbVector3(0.0f);
	
	
	//vcam1 und vcam3
	switch (m_Face) {
		case CF_FRONT:
			VCAM2.z = vPlayer.z;
			VCAM1 = tbVector3(vPlayer.x, vPlayer.y, vPlayer.z * m_fCamZoom);
			switch (g_pGS->m_GravitationDir) {
				case GD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case GD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case GD_LEFT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
				case GD_RIGHT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
			}
			break;
		case CF_RIGHT:
			VCAM2.x = vPlayer.x;
			VCAM1 = tbVector3(vPlayer.x * m_fCamZoom, vPlayer.y, vPlayer.z);
			switch (g_pGS->m_GravitationDir) {
				case GD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case GD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case GD_LEFT: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
				case GD_RIGHT: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
			}
			break;
		case CF_BACK:
			VCAM2.z = vPlayer.z;
			VCAM1 = tbVector3(vPlayer.x, vPlayer.y, vPlayer.z * m_fCamZoom);
			switch (g_pGS->m_GravitationDir) {
				case GD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case GD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case GD_LEFT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
				case GD_RIGHT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
			}
			break;
		case CF_LEFT:
			VCAM2.x = vPlayer.x;
			VCAM1 = tbVector3(vPlayer.x * m_fCamZoom, vPlayer.y, vPlayer.z);
			switch (g_pGS->m_GravitationDir) {
				case GD_UP: 	VCAM3 = tbVector3( 0.0f,-1.0f, 0.0f); break;
				case GD_DOWN: 	VCAM3 = tbVector3( 0.0f, 1.0f, 0.0f); break;
				case GD_LEFT: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
				case GD_RIGHT: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
			}
			break;
		case CF_TOP:
			VCAM2.y = vPlayer.y;
			VCAM1 = tbVector3(vPlayer.x, vPlayer.y * m_fCamZoom, vPlayer.z);
			switch (g_pGS->m_GravitationDir) {
				case GD_UP: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
				case GD_DOWN: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
				case GD_LEFT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
				case GD_RIGHT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
			}
			break;
		case CF_BOTTOM:
			VCAM2.y = vPlayer.y;
			VCAM1 = tbVector3(vPlayer.x, vPlayer.y * m_fCamZoom, vPlayer.z);
			switch (g_pGS->m_GravitationDir) {
				case GD_UP: 	VCAM3 = tbVector3( 0.0f, 0.0f,-1.0f); break;
				case GD_DOWN: 	VCAM3 = tbVector3( 0.0f, 0.0f, 1.0f); break;
				case GD_LEFT: 	VCAM3 = tbVector3( 1.0f, 0.0f, 0.0f); break;
				case GD_RIGHT: 	VCAM3 = tbVector3(-1.0f, 0.0f, 0.0f); break;
			}
			break;
	}
	
	//vcam2
	switch (m_Cam2Style) {
		case C2_ZERO:	break;
		case C2_PLAYER:	VCAM2 = vPlayer; break;
		default: break;
	}
}

void CPlayer::SwitchFace() {
	//seitenwechsel
	if (m_vPos.x < -0.5f - m_vWH.x / 2.0f) {
		m_vPos.x += CUBE_SIZE;
		switch (m_Face) {
			case CF_FRONT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_RIGHT; 	break;
					case GD_DOWN : m_Face = CF_LEFT; 	break;
					case GD_LEFT : m_Face = CF_TOP; 	break;
					case GD_RIGHT: m_Face = CF_BOTTOM;	break;
					default: break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BACK; 	break;
					case GD_DOWN : m_Face = CF_FRONT; 	break;
					case GD_LEFT : m_Face = CF_TOP;    g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_RIGHT: m_Face = CF_BOTTOM; g_pGS->m_GravitationDir = GD_DOWN; break;//changed from up
					default: break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_LEFT; 	break;
					case GD_DOWN : m_Face = CF_RIGHT; 	break;
					case GD_LEFT : m_Face = CF_TOP;    g_pGS->m_GravitationDir = GD_RIGHT; 	break;
					case GD_RIGHT: m_Face = CF_BOTTOM; g_pGS->m_GravitationDir = GD_LEFT; 	break;
					default: break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_FRONT; 	break;
					case GD_DOWN : m_Face = CF_BACK; 	break;
					case GD_LEFT : m_Face = CF_TOP;    g_pGS->m_GravitationDir = GD_UP; break;
					case GD_RIGHT: m_Face = CF_BOTTOM; g_pGS->m_GravitationDir = GD_UP; break;
					default: break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_RIGHT; g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_DOWN : m_Face = CF_LEFT;  g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_LEFT : m_Face = CF_BACK;  g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_RIGHT: m_Face = CF_FRONT; g_pGS->m_GravitationDir = GD_RIGHT; break;
					default: break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_RIGHT; g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_DOWN : m_Face = CF_LEFT;  g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_LEFT : m_Face = CF_FRONT; g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_RIGHT: m_Face = CF_BACK;  g_pGS->m_GravitationDir = GD_LEFT; break;
					default: break;
				}
				break;
				
			default: break;
		}
	} else if (m_vPos.x > 0.5 + CUBE_SIZE - m_vWH.x / 2.0f) {
		m_vPos.x -= CUBE_SIZE;
		switch (m_Face) {
			case CF_FRONT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_LEFT; 	break;
					case GD_DOWN : m_Face = CF_RIGHT; 	break;
					case GD_LEFT : m_Face = CF_BOTTOM; 	break;
					case GD_RIGHT: m_Face = CF_TOP; 	break;
					default: break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_FRONT; 	break;
					case GD_DOWN : m_Face = CF_BACK; 	break;
					case GD_LEFT : m_Face = CF_BOTTOM;  g_pGS->m_GravitationDir = GD_UP; 	break;
					case GD_RIGHT: m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_UP; 	break;
					default: break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_RIGHT; break;
					case GD_DOWN : m_Face = CF_LEFT;  break;
					case GD_LEFT : m_Face = CF_BOTTOM;  g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_RIGHT: m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_LEFT;  break;
					default: break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BACK;  break;
					case GD_DOWN : m_Face = CF_FRONT; break;
					case GD_LEFT : m_Face = CF_BOTTOM;  g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_RIGHT: m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_DOWN; break;
					default: break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_LEFT;   g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_DOWN : m_Face = CF_RIGHT;  g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_LEFT : m_Face = CF_FRONT;  g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_RIGHT: m_Face = CF_BACK;   g_pGS->m_GravitationDir = GD_LEFT; break;
					default: break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_LEFT;   g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_DOWN : m_Face = CF_RIGHT;  g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_LEFT : m_Face = CF_BACK;   g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_RIGHT: m_Face = CF_FRONT;  g_pGS->m_GravitationDir = GD_RIGHT; break;
					default: break;
				}
				break;
				
			default: break;
		}
	} else if (m_vPos.y < -0.5f - m_vWH.y / 2.0f) {
		m_vPos.y += CUBE_SIZE;
		switch (m_Face) {
			case CF_FRONT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_TOP; 	break;
					case GD_DOWN : m_Face = CF_BOTTOM; 	break;
					case GD_LEFT : m_Face = CF_LEFT; 	break;
					case GD_RIGHT: m_Face = CF_RIGHT; 	break;
					default: break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_LEFT;	break;
					case GD_DOWN : m_Face = CF_BOTTOM; 	g_pGS->m_GravitationDir = GD_LEFT;  break;
					case GD_LEFT : m_Face = CF_FRONT;  	break;
					case GD_RIGHT: m_Face = CF_BACK; 	break;
					default: break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_DOWN : m_Face = CF_BOTTOM;  g_pGS->m_GravitationDir = GD_UP; break;
					case GD_LEFT : m_Face = CF_RIGHT;    break;
					case GD_RIGHT: m_Face = CF_LEFT;     break;
					default: break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_DOWN : m_Face = CF_BOTTOM;  g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_LEFT : m_Face = CF_BACK;   break;
					case GD_RIGHT: m_Face = CF_FRONT;  break;
					default: break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BACK;   g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_DOWN : m_Face = CF_FRONT;  g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_LEFT : m_Face = CF_LEFT;   g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_RIGHT: m_Face = CF_RIGHT;  g_pGS->m_GravitationDir = GD_DOWN; break;
					default: break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_FRONT;   g_pGS->m_GravitationDir = GD_UP; break;
					case GD_DOWN : m_Face = CF_BACK;    g_pGS->m_GravitationDir = GD_UP; break;
					case GD_LEFT : m_Face = CF_LEFT;    g_pGS->m_GravitationDir = GD_UP; break;
					case GD_RIGHT: m_Face = CF_RIGHT;   g_pGS->m_GravitationDir = GD_UP; break;
					default: break;
				}
				break;
				
			default: break;
		}
	} else if (m_vPos.y > 0.5 + CUBE_SIZE - m_vWH.y / 2.0f) {
		m_vPos.y -= CUBE_SIZE;
		switch (m_Face) {
			case CF_FRONT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BOTTOM; 	break;
					case GD_DOWN : m_Face = CF_TOP; 	break;
					case GD_LEFT : m_Face = CF_RIGHT; 	break;
					case GD_RIGHT: m_Face = CF_LEFT; 	break;
					default: break;
				}
				break;
			case CF_RIGHT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BOTTOM; 	g_pGS->m_GravitationDir = GD_RIGHT;	break;
					case GD_DOWN : m_Face = CF_TOP; 	g_pGS->m_GravitationDir = GD_RIGHT; break;
					case GD_LEFT : m_Face = CF_BACK;  	break;
					case GD_RIGHT: m_Face = CF_FRONT; 	break;
					default: break;
				}
				break;
			case CF_BACK:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BOTTOM; g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_DOWN : m_Face = CF_TOP;    g_pGS->m_GravitationDir = GD_UP; break;
					case GD_LEFT : m_Face = CF_LEFT;    break;
					case GD_RIGHT: m_Face = CF_RIGHT; 	break;
					default: break;
				}
				break;
			case CF_LEFT:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BOTTOM; g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_DOWN : m_Face = CF_TOP;    g_pGS->m_GravitationDir = GD_LEFT; break;
					case GD_LEFT : m_Face = CF_FRONT; break;
					case GD_RIGHT: m_Face = CF_BACK;  break;
					default: break;
				}
				break;
			case CF_TOP:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_FRONT; g_pGS->m_GravitationDir = GD_UP; break;
					case GD_DOWN : m_Face = CF_BACK;  g_pGS->m_GravitationDir = GD_UP; break;
					case GD_LEFT : m_Face = CF_RIGHT; g_pGS->m_GravitationDir = GD_UP; break;
					case GD_RIGHT: m_Face = CF_LEFT;  g_pGS->m_GravitationDir = GD_UP; break;
					default: break;
				}
				break;
			case CF_BOTTOM:
				switch (g_pGS->m_GravitationDir) {
					case GD_UP   : m_Face = CF_BACK;   g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_DOWN : m_Face = CF_FRONT;  g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_LEFT : m_Face = CF_RIGHT;  g_pGS->m_GravitationDir = GD_DOWN; break;
					case GD_RIGHT: m_Face = CF_LEFT;   g_pGS->m_GravitationDir = GD_DOWN; break;
					default: break;
				}
				break;
				
			default: break;
		}
	}
	
	g_pGS->m_PlayerFace = m_Face;
	g_pLevel->UpdateBlockGD();
}

