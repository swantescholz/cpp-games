#include "CFalseBelief.h"

CFalseBelief::CFalseBelief(tbVector2 pvStartPos, string psThought)
{
	m_OT = OT_FALSE_BELIEF;
	m_sThought = psThought;
	m_TexFalseBelief.Load("false_belief");
	m_vWH = tbVector2(2.0,2.0);
	m_vPos = tbVector2(pvStartPos.x,pvStartPos.y);
	m_iCanShowThought = 0;
}

CFalseBelief::~CFalseBelief()
{
}

void CFalseBelief::Update(list<S_LevelIntern> *palObjects) {
	if (m_iCanShowThought == 2) {
		m_iCanShowThought = 1;
	} else if (m_iCanShowThought == 0) {
		m_iCanShowThought = 2;
	}
	return;
}


bool CFalseBelief::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	int iSize = 4;
	int aiOrder[] = {OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			if (aiOrder[i] != OT_PLAYER) {
				pObj->SetVel(tbVector2(pObj->GetVel().x,0.0f));
				pObj->SetPos(poyAdjustPushingRectY(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH));
			}
			if (m_iCanShowThought == 1) {
				AddThought(palObjects);
			}
			m_iCanShowThought = 0;
			return false;
		}
	}
	return true;
}

bool CFalseBelief::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	int iSize = 4;
	int aiOrder[] = {OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			if (pObj->GetOT() != OT_PLAYER) {
				pObj->SetVel(tbVector2(0.0f,pObj->GetVel().y));
				pObj->SetPos(poyAdjustPushingRectX(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH));
			} else {
				if (m_iCanShowThought == 1) {
					AddThought(palObjects);
				}
				m_iCanShowThought = 0;
			}
			return false;
		}
	}
	return true;
}

void CFalseBelief::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	m_TexFalseBelief.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
	glEnd();
}

void CFalseBelief::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPos - tbVector2(0.0,m_vWH.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(0.34,0.49,0.22,1.0),tbColor(0.34,0.49,0.21,1.0),tbColor(0.0,0.0,0.0,1.0),0.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

bool CFalseBelief::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	return poyCollision2(m_vPos,m_vWH,pvCenter,pvWH);
}

