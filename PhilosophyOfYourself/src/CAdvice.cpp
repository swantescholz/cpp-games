#include "CAdvice.h"

const float 	CAdvice::c_cfItemInternetLightRadiusFactor 	= 1.5f;
const tbColor 	CAdvice::c_cThoughtStartCol					= tbColor(1.0, 0.8, 0.6, 1.0);

CAdvice::CAdvice(EAdviceType pAT, tbVector2 pvStartPos, string psThought) :
	m_sSndTaken("advice_taken")
{
	m_OT = OT_ADVICE;
	m_AT = pAT;
	m_sThought = psThought;
	m_TexGround.Load("advice_ground");
	m_TexTop.Load   ("advice_top");
	switch (m_AT) {
		case AT_NEWSPAPER: m_TexItem.Load   ("advice_item_newspaper"); break;
		case AT_INTERNET : m_TexItem.Load   ("advice_item_internet" ); break;
		case AT_BOOK 	 : m_TexItem.Load   ("advice_item_book" 	); break;
		default: break;
	}
	m_vWHGround  = tbVector2(2.0,2.0);
	m_vWHTop     = tbVector2(2.0,2.0);
	m_vWHItem	 = tbVector2(0.8,0.8);
	m_vPosGround = tbVector2(pvStartPos.x,pvStartPos.y);
	m_vPosTop    = m_vPosGround + tbVector2(0.0f,m_vWHGround.y/2.0f+m_vWHGround.y/2.0f);
	m_vPosItem 	 = m_vPosTop;
	
	m_bUsed 	 = false;
}

CAdvice::~CAdvice()
{
}

void CAdvice::Update(list<S_LevelIntern> *palObjects) {
	return;
}


bool CAdvice::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPosTop   ,m_vWHTop)    == true && pObj->GetOT() == OT_PLAYER) {
		UseAdvice(palObjects,dynamic_cast<CPlayer*>(pObj));
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

bool CAdvice::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPosTop   ,m_vWHTop)    == true && pObj->GetOT() == OT_PLAYER) {
		UseAdvice(palObjects,dynamic_cast<CPlayer*>(pObj));
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

void CAdvice::Render() {
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
	fZValue += g_pGS->m_cfLayerGapZ;
	if (m_bUsed == false) { //item
		m_TexItem.Bind();
		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPosItem.x-m_vWHItem.x/2.0f,m_vPosItem.y-m_vWHItem.y/2.0f,fZValue).c);
			glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPosItem.x+m_vWHItem.x/2.0f,m_vPosItem.y-m_vWHItem.y/2.0f,fZValue).c);
			glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPosItem.x+m_vWHItem.x/2.0f,m_vPosItem.y+m_vWHItem.y/2.0f,fZValue).c);
			glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPosItem.x-m_vWHItem.x/2.0f,m_vPosItem.y+m_vWHItem.y/2.0f,fZValue).c);
		glEnd();
	} else if (m_AT == AT_NEWSPAPER) { //arrow
		m_TexItem.Bind();
		glPushAttrib(GL_LIGHTING_BIT);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE	, tbColor(1.0,0.2,0.2,0.8).c);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR				, tbColor(1.0,0.2,0.2,0.8).c);
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS			, 5.0f);
		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glTexCoord2f(1,1); glVertex3fv(tbVector3( (m_vPosItem +  m_vArrowDir * m_vWHItem.y/2.0f * 2.0f + tbVector2NormalLeft ( m_vArrowDir) * m_vWHItem.x/2.0f) ,fZValue).c);
			glTexCoord2f(0,1); glVertex3fv(tbVector3( (m_vPosItem + -m_vArrowDir * m_vWHItem.y/2.0f * 2.0f + tbVector2NormalRight(-m_vArrowDir) * m_vWHItem.x/2.0f) ,fZValue).c);
			glTexCoord2f(0,0); glVertex3fv(tbVector3( (m_vPosItem + -m_vArrowDir * m_vWHItem.y/2.0f * 2.0f + tbVector2NormalLeft (-m_vArrowDir) * m_vWHItem.x/2.0f) ,fZValue).c);
			glTexCoord2f(1,0); glVertex3fv(tbVector3( (m_vPosItem +  m_vArrowDir * m_vWHItem.y/2.0f * 2.0f + tbVector2NormalRight( m_vArrowDir) * m_vWHItem.x/2.0f) ,fZValue).c);
		glEnd();
		glPopAttrib();
	}
}

void CAdvice::UseAdvice(list<S_LevelIntern> *palObjects, CPlayer *pPlayer) {
	if (poyCollision2(pPlayer->GetPos(),pPlayer->GetWH(),tbVector2InterpolateCoords(m_vPosGround,m_vPosTop,0.5f+g_pGS->m_cfOffset),tbVector2(m_vWHTop)*0.1f)) {
		if (g_pGL->KeyWasDown(SDLK_d) == true) {
			if (m_bUsed == false) {
				m_bUsed = true;
				glcSound(m_sSndTaken).Play();
				switch (m_AT) {
					case AT_NEWSPAPER: 	//shows direction
						m_TexItem.Load("arrow");
						SetArrowDir(palObjects);
						break;
					case AT_INTERNET:	//more light radius
						g_pGS->m_fLightAngle *= c_cfItemInternetLightRadiusFactor;
						break;
					default: throw glcException(string("wrong advise_type: ") + m_AT); break;
				}
				AddThought(palObjects);
			}
		}
	}
}

void CAdvice::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPosGround - tbVector2(0.0,m_vWHGround.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(0.5,0.2,0.1,1.0),tbColor(0.5,0.3,0.0,1.0),tbColor(0.3,0.3,0.0,1.0),80.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

void CAdvice::SetArrowDir(list<S_LevelIntern> *palObjects) {
	for (list<S_LevelIntern>::iterator it1 = palObjects[OT_ANSWER].begin(); it1 != palObjects[OT_ANSWER].end(); it1++) {
		m_vArrowDir = tbVector2Normalize( dynamic_cast<CAnswer*>(it1->pObj)->GetCenter() - m_vPosItem);
		break;
	}
}

bool CAdvice::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	if (poyCollision2(m_vPosGround,m_vWHGround,pvCenter,pvWH) == true) return true;
	if (poyCollision2(m_vPosTop   ,m_vWHTop   ,pvCenter,pvWH) == true) return true;
	return false;
}

bool CAdvice::CanBePassedThrough(tbVector2 pvCenter, tbVector2 pvWH) {
	if (poyCollision2(m_vPosGround,m_vWHGround,pvCenter,pvWH) == true) return false;
	//if (poyCollision2(m_vPosTop,m_vWHTop   ,pvCenter,pvWH) == true) return false;
	return true;
}


