#include "CIgnorance.h"

CIgnorance::CIgnorance(tbVector2 pvStartPos, string psThought) : 
	m_sSndDestroy("ignorance_destroy")
{
	m_OT = OT_IGNORANCE;
	m_sThought = psThought;
	m_TexIgnorance.Load("ignorance");
	m_vWH = tbVector2(2.0,2.0);
	m_vPos = tbVector2(pvStartPos.x,pvStartPos.y);
}

CIgnorance::~CIgnorance()
{
}

void CIgnorance::Update(list<S_LevelIntern> *palObjects) {
	return;
}


bool CIgnorance::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	// at lava, so destroy/kill/explode
	if (pObj->GetOT() == OT_SOLID_ARGUMENT || pObj->GetOT() == OT_RASH_ARGUMENT || pObj->GetOT() == OT_LIE ||
		pObj->GetOT() == OT_PLAYER) {
			AddThought(palObjects); // Add the thought
			DoDestroying(palObjects, pObj);
			return false;
	}
	return true;
}

bool CIgnorance::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	// at lava, so destroy/kill/explode
	if (pObj->GetOT() == OT_SOLID_ARGUMENT || pObj->GetOT() == OT_RASH_ARGUMENT || pObj->GetOT() == OT_LIE ||
		pObj->GetOT() == OT_PLAYER) {
			AddThought(palObjects); // Add the thought
			DoDestroying(palObjects, pObj);
			return false;
	}
	return true;
}

void CIgnorance::DoDestroying(list<S_LevelIntern> *palObjects, CObject *pObj) {
	glcSound(m_sSndDestroy).Play();
	if (pObj->GetOT() == OT_SOLID_ARGUMENT	) {dynamic_cast<CSolidArgument*	>(pObj)->DoDestroy(palObjects);}
	if (pObj->GetOT() == OT_RASH_ARGUMENT 	) {dynamic_cast<CRashArgument* 	>(pObj)->DoDestroy(palObjects);}
	if (pObj->GetOT() == OT_LIE 			) {dynamic_cast<CLie* 			>(pObj)->DoExplode(palObjects);}
	if (pObj->GetOT() == OT_PLAYER 			) {dynamic_cast<CPlayer*		>(pObj)->DoKill   (palObjects);}
}

void CIgnorance::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	m_TexIgnorance.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
	glEnd();
}

void CIgnorance::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPos - tbVector2(0.0,m_vWH.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(0.47,0.2,0.0,1.0),tbColor(0.47,0.2,0.0,1.0),tbColor(0.0,0.0,0.0,1.0),0.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

bool CIgnorance::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	return poyCollision2(m_vPos,m_vWH,pvCenter,pvWH);
}

