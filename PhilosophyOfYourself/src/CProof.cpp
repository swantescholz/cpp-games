#include "CProof.h"

CProof::CProof(tbVector2 pvStartPos, string psThought) :
	m_sSndProven("proof_proven")
{
	m_OT = OT_PROOF;
	m_sThought = psThought;
	m_TexGround.Load("proof_ground_1");
	m_TexTop.Load   ("proof_top_1");
	m_vWHGround  = tbVector2(2.0,2.0);
	m_vWHTop     = tbVector2(2.0,2.0);
	m_vPosGround = tbVector2(pvStartPos.x,pvStartPos.y);
	m_vPosTop    = m_vPosGround + tbVector2(0.0f,m_vWHGround.y/2.0f+m_vWHGround.y/2.0f);
	
	m_bSavedHere = false;
}

CProof::~CProof()
{
}

void CProof::Update(list<S_LevelIntern> *palObjects) {
	return;
}


bool CProof::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPosTop   ,m_vWHTop)    == true && pObj->GetOT() == OT_PLAYER) {
		Save(palObjects,dynamic_cast<CPlayer*>(pObj));
	}
	if (pObj->Collision(m_vPosGround,m_vWHGround) == false) return true;
	
	int iSize = 4;
	int aiOrder[] = {OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			pObj->SetVel(tbVector2(pObj->GetVel().x,0.0f));
			pObj->SetPos(poyAdjustPushingRectY(pObj->GetPos(),pObj->GetWH(),m_vPosGround,m_vWHGround));
			return false;
		}
	}
	return true;
}

bool CProof::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPosTop   ,m_vWHTop)    == true && pObj->GetOT() == OT_PLAYER) {
		Save(palObjects,dynamic_cast<CPlayer*>(pObj));
	}
	if (pObj->Collision(m_vPosGround,m_vWHGround) == false) return true;
	
	int iSize = 4;
	int aiOrder[] = {OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			pObj->SetVel(tbVector2(0.0f,pObj->GetVel().y));
			pObj->SetPos(poyAdjustPushingRectX(pObj->GetPos(),pObj->GetWH(),m_vPosGround,m_vWHGround));
			return false;
		}
	}
	return true;
}

void CProof::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	m_TexGround.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPosGround.x-m_vWHGround.x/2.0f,m_vPosGround.y-m_vWHGround.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPosGround.x+m_vWHGround.x/2.0f,m_vPosGround.y-m_vWHGround.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPosGround.x+m_vWHGround.x/2.0f,m_vPosGround.y+m_vWHGround.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPosGround.x-m_vWHGround.x/2.0f,m_vPosGround.y+m_vWHGround.y/2.0f,fZValue).c);
	glEnd();
	m_TexTop.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPosTop.x-m_vWHTop.x/2.0f,m_vPosTop.y-m_vWHTop.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPosTop.x+m_vWHTop.x/2.0f,m_vPosTop.y-m_vWHTop.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPosTop.x+m_vWHTop.x/2.0f,m_vPosTop.y+m_vWHTop.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPosTop.x-m_vWHTop.x/2.0f,m_vPosTop.y+m_vWHTop.y/2.0f,fZValue).c);
	glEnd();
}

void CProof::Save(list<S_LevelIntern> *palObjects, CPlayer *pPlayer) {
	if (poyCollision2(pPlayer->GetPos(),pPlayer->GetWH(),tbVector2InterpolateCoords(m_vPosGround,m_vPosTop,0.5f+g_pGS->m_cfOffset),tbVector2(m_vWHTop)*0.1f)) {
		if (g_pGL->KeyWasDown(SDLK_s) == true) {
			if (m_bSavedHere == false) {
				if (g_pGS->m_bSavingLevel == false) {
					SetSaved(true, palObjects);
					g_pGS->m_bSavingLevel = true;
				}
			} else {//not saving
				SetSaved(false,palObjects); //resetting to unsaved-state
			}
		}
	}
}

void CProof::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPosGround - tbVector2(0.0,m_vWHGround.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(0.59,0.34,0.12,1.0),tbColor(0.0,0.0,0.0,1.0),tbColor(0.0,0.0,0.0,1.0),20.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

void CProof::SetSaved(bool pb, list<S_LevelIntern> *palObjects) {
	if (pb == true) {
		if (palObjects != NULL) {
			m_bSavedHere = true;
			m_TexGround.Load("proof_ground_2");
			m_TexTop.Load   ("proof_top_2");
			glcSound(m_sSndProven).Play();
			//disable other proof-savings
			for (list<S_LevelIntern>::iterator it1 = palObjects[OT_PROOF].begin(); it1 != palObjects[OT_PROOF].end(); it1++) {
				CProof *proof = dynamic_cast<CProof*>(it1->pObj);
				if (proof->IsSaved() == true && proof != this) {
					proof->SetSaved(false, NULL);
				}
			}
			AddThought(palObjects); //add save/proof thought
		}
	} else {
		m_bSavedHere = false;
		m_TexGround.Load("proof_ground_1");
		m_TexTop.Load   ("proof_top_1");
	}
}

bool CProof::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	if (poyCollision2(m_vPosGround,m_vWHGround,pvCenter,pvWH) == true) return true;
	if (poyCollision2(m_vPosTop   ,m_vWHTop   ,pvCenter,pvWH) == true) return true;
	return false;
}

bool CProof::CanBePassedThrough(tbVector2 pvCenter, tbVector2 pvWH) {
	if (poyCollision2(m_vPosGround,m_vWHGround,pvCenter,pvWH) == true) return false;
	//if (poyCollision2(m_vPosTop,m_vWHTop   ,pvCenter,pvWH) == true) return false;
	return true;
}


