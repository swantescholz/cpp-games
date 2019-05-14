#include "CPlayer.h"

CPlayer::CPlayer(tbVector2 pvStartPos, int piNumMaxJumps, float pfJumpSpeed, tbVector2 pvAcc, tbVector2 pvMaxSpeed) :
	m_sSndJump("player_jump"), m_sSndDie("player_die"), m_ciNumMaxJumps(piNumMaxJumps), m_cvMaxSpeed(pvMaxSpeed)
{
	m_OT = OT_PLAYER;
	m_TexPlayer.Load("player");
	m_vWH = tbVector2(1.5,3.0);
	m_vPos = tbVector2(pvStartPos.x,pvStartPos.y);
	m_vLastPos = m_vPos;
	m_fJumpSpeed = pfJumpSpeed;
	
	m_vAcc = pvAcc;
	m_vVel = tbVector2(0,0);
	m_iNumJumps = 0;
	
	m_bIsShrunken = false;
	m_bShouldUnshrink = false;
	m_fShrinkFactor = 2.0f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update(list<S_LevelIntern> *palObjects) {
	m_vLastPos = m_vPos;
	
	//y-movement
	if (m_vVel.y == 0.0f && m_vLastVel.y <= 0.0f) {	//on the ground?
		m_iNumJumps = 0;
	} else if (m_iNumJumps == 0) { //falling jump--
		m_iNumJumps = 1;
	}
	m_vLastVel = m_vVel;	//saving last velocity
	if (g_pGL->KeyWasDown(SDLK_UP)) { //jumping
		if (m_iNumJumps > 0) {
			if (m_iNumJumps < m_ciNumMaxJumps) {
				m_iNumJumps++;
				m_vVel.y += m_fJumpSpeed;
				glcSound(m_sSndJump).Play();
			}
		} else {
			if (m_vVel.y == 0.0f && m_vLastVel.y <= 0.0f) {
				m_iNumJumps++;
				m_vVel.y += m_fJumpSpeed;
				glcSound(m_sSndJump).Play();
			}
		}
	} else if (g_pGL->KeyWasUp(SDLK_UP)) { //slow down flight
		if (m_iNumJumps > 0 && m_vVel.y > 0.0f) {
			m_vVel.y *= 0.5f;
		}
	}
	m_vVel.y -= m_vAcc.y * ELAPSED;	//gravity
	if (m_vVel.y >  m_cvMaxSpeed.y) m_vVel.y =  m_cvMaxSpeed.y;
	if (m_vVel.y < -m_cvMaxSpeed.y) m_vVel.y = -m_cvMaxSpeed.y;
	
	//x-movement
	if (g_pGL->KeyIsDown(SDLK_LEFT)) {
		m_vVel.x = -m_vAcc.x;
	} else if (g_pGL->KeyIsDown(SDLK_RIGHT)) {
		m_vVel.x =  m_vAcc.x;
	} else {
		m_vVel.x = 0.0f;
	}
	if (m_vVel.x >  m_cvMaxSpeed.x) m_vVel.x =  m_cvMaxSpeed.x;
	if (m_vVel.x < -m_cvMaxSpeed.x) m_vVel.x = -m_cvMaxSpeed.x;
	
	//shrinking
	if (g_pGL->KeyWasDown(SDLK_DOWN) && m_bIsShrunken == false) {
		/*
		m_vPos.y -= m_vWH.y * 0.5f;
		m_vWH.y  /= m_fShrinkFactor;
		m_vPos.y += m_vWH.y * 0.5f;
		m_bIsShrunken = true;
		m_bShouldUnshrink = false;
		*/
	} else if (g_pGL->KeyWasUp(SDLK_DOWN) && m_bIsShrunken == true) {
		if (CanUnshrink(palObjects)) {
			m_vPos.y -= m_vWH.y * 0.5f;
			m_vWH.y  *= m_fShrinkFactor;
			m_vPos.y += m_vWH.y * 0.5f;
			m_bIsShrunken = false;
		} else {
			m_bShouldUnshrink = true;
		}
	}
	
	//moving, should happen before unshrinking
	MoveY(palObjects);
	MoveX(palObjects);
	
	//unshrinking without events
	if (m_bShouldUnshrink == true) {
		if (CanUnshrink(palObjects)) {
			m_vPos.y -= m_vWH.y * 0.5f;
			m_vWH.y  *= m_fShrinkFactor;
			m_vPos.y += m_vWH.y * 0.5f;
			m_bShouldUnshrink = false;
			m_bIsShrunken = false;
		}
	}
}

void CPlayer::MoveY(list<S_LevelIntern> *palObjects) {
	m_vPos.y += m_vVel.y * ELAPSED;
	if (m_vPos.y == m_vLastPos.y) return; //no moving occurs, so no pushing occurs, so return
	
	int iSize = 10;
	int aiOrder[] = {OT_PROOF,OT_ADVICE,OT_ANSWER,OT_AXIOM,OT_UNKNOWN,OT_FALSE_BELIEF,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_IGNORANCE};
	for (int i = 0; i < iSize; i++) {
		for (list<S_LevelIntern>::iterator it1 = palObjects[aiOrder[i] ].begin(); it1 != palObjects[aiOrder[i] ].end(); it1++) {
			if (it1->pObj->Collision(m_vPos,m_vWH) && it1->pObj != this) {
				if (it1->pObj->PushY(palObjects,this) == false) {
					break;
				}
			}
		}
	}
}

void CPlayer::MoveX(list<S_LevelIntern> *palObjects) {
	m_vPos.x += m_vVel.x * ELAPSED;
	if (m_vPos.x == m_vLastPos.x) return; //no moving occurs, so no pushing occurs, so return
	
	int iSize = 10;
	int aiOrder[] = {OT_PROOF,OT_ADVICE,OT_ANSWER,OT_AXIOM,OT_UNKNOWN,OT_FALSE_BELIEF,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_IGNORANCE};
	for (int i = 0; i < iSize; i++) {
		for (list<S_LevelIntern>::iterator it1 = palObjects[aiOrder[i] ].begin(); it1 != palObjects[aiOrder[i] ].end(); it1++) {
			if (it1->pObj->Collision(m_vPos,m_vWH) && it1->pObj != this) {
				if (it1->pObj->PushX(palObjects,this) == false) {
					if (aiOrder[i] == OT_RASH_ARGUMENT || aiOrder[i] == OT_SOLID_ARGUMENT || aiOrder[i] == OT_LIE) {
						//do nothing (before here was a break; that was VERY important to delete!, because maybe the player pushes other solid arguments, too)
					} else {
						break; //further pushing shall NOT be possible
					}
				}
			}
		}
	}
}

bool CPlayer::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	int iSize = 4;
	int aiOrder[] = {OT_LIE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			if (pObj->GetVel().y < 0.0f) {
				if (m_vVel.y == 0.0f && pObj->GetPos().y > m_vPos.y && pObj->GetVel().y < 0.0f) {
					DoKill(palObjects); //solid argument is pressing player down to ground -> dead
				} else if (m_vPos.y >= pObj->GetPos().y){
					m_vPos   = poyAdjustPushingRectY(m_vPos,m_vWH,pObj->GetPos(),pObj->GetWH());
					m_vVel.y = pObj->GetVel().y;
				} else if (m_vPos.y <  pObj->GetPos().y){
					m_vPos   = poyAdjustPushingRectY(m_vPos,m_vWH,pObj->GetPos(),pObj->GetWH());
					m_vVel.y = pObj->GetVel().y;
				}
			} else {
				g_pLogfile->Textout(RED,"unexpected positive ypushing type " + aiOrder[i]);
			}
			return false;
		}
	}
	return true;
}

bool CPlayer::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	int iSize = 3;
	int aiOrder[] = {OT_SOLID_ARGUMENT,OT_RASH_ARGUMENT,OT_LIE};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			pObj->SetVel(tbVector2(0.0f,pObj->GetVel().y));
			pObj->SetPos(poyAdjustPushingRectX(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH));
			return false;
		}
	}
	return true;
}

void CPlayer::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	m_TexPlayer.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
	glEnd();
}

bool CPlayer::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	return poyCollision2(tbVector2(m_vPos.x,m_vPos.y),m_vWH,pvCenter,pvWH);
}


void CPlayer::DoKill(list<S_LevelIntern> *palObjects) {
	g_pGS->m_bRespawn = true;
	glcSound(m_sSndDie).Play();
	throw glcException("player was killed");
}

bool CPlayer::CanUnshrink(list<S_LevelIntern> *palObjects) {
	if (m_bIsShrunken == false) return true;
	tbVector2 vWH = m_vWH * tbVector2(1.0f,m_fShrinkFactor);
	tbVector2 vPos= m_vPos - tbVector2(0.0,m_vWH.y / 2.0f) + tbVector2(0.0f,vWH.y / 2.0f);
	
	for (int i = 0; i < OT_LAST; i++) {
		for (list<S_LevelIntern>::iterator it1 = palObjects[i].begin(); it1 != palObjects[i].end(); it1++) {
			if (it1->pObj->CanBePassedThrough(vPos,vWH) == false) {
				if (it1->pObj->GetOT() == m_OT) {if(dynamic_cast<CPlayer*>(it1->pObj) == this) continue;} //its me!
				return false;
			}
		}
	}
	return true;
}


