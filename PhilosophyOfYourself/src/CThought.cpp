#include "CThought.h"

CThought::CThought(string psThought,string psFont,int piSize,glcMaterial pMat,
			 tbVector2 pv1,tbVector2 pv2,tbVector2 pv3,tbVector2 pv4)
{
	m_OT = OT_THOUGHT;
	m_sThought = psThought;
	m_Mat = pMat;
	m_vPos[0] = pv1;m_vPos[1] = pv2;m_vPos[2] = pv3;m_vPos[3] = pv4;
	m_fLife = 1.0f;
	m_fFade = 0.0f;
	m_fZValue = (OT_LAST - m_OT) * 0.001f;
	m_fZValueVel = -0.001f;
	m_bActive = false;
	
	m_Font.Load(psFont,piSize);
}

CThought::~CThought()
{
}

void CThought::Start(tbVector2 pvPos, tbVector2 pvVel, tbVector2 pvAcc, float pfFade) {
	for (int i = 0; i < 4; i++) {
		m_vPos[i] += pvPos;
	}
	m_vVel = pvVel;
	m_vAcc = pvAcc;
	m_fFade = pfFade;
	if (m_fFade == 0.0f) {
		m_fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ; //background text, so at the end
	} else {
		m_fZValue = (OT_LAST +    1) * g_pGS->m_cfLayerGapZ; //foreground text, so in front of everything else (1 is estimated good)
	}
	m_fZValueVel = -0.001f;
	m_bActive = true;
}

void CThought::Update(list<S_LevelIntern> *palObjects) {
	if (m_bActive == true) {
		const float fTmp = m_fFade * ELAPSED;
		m_fLife -= fTmp;
		if (m_fLife <= 0.0f) {
			m_bActive = false;
			m_bDeleted = true;
			return;
		}
		m_fZValue += m_fZValueVel * fTmp;
		
		if (m_fLife <= 0.5f) 	m_Mat.a.a = m_Mat.b.a = m_Mat.c.a = m_fLife * 2.0f;	//late going into alpha
		else					m_Mat.a.a = m_Mat.b.a = m_Mat.c.a = 1.0f   ;		//full opaque
		
		//moving/acceleration
		m_vVel += m_vAcc * ELAPSED;
		for (int i = 0; i < 4; i++) {
			m_vPos[i] += m_vVel * ELAPSED;
		}
	}
}

void CThought::Render() {
	if (m_bActive == true && m_sThought.length() > 0) {
		glNormal3f(0,0,1);
		if (m_fFade == 0.0f) { //background text, so also not lighted
			m_Font.RenderText(m_sThought,m_Mat.a+m_Mat.b,//m_Mat.c, //edited
				tbVector3(m_vPos[0],m_fZValue),
				tbVector3(m_vPos[1],m_fZValue),
				tbVector3(m_vPos[2],m_fZValue),
				tbVector3(m_vPos[3],m_fZValue));
		} else { //small thought text, so no light to be good seen
			m_Font.RenderText(m_sThought,m_Mat.a+m_Mat.b,//m_Mat.c, //edited
				tbVector3(m_vPos[0],m_fZValue),
				tbVector3(m_vPos[1],m_fZValue),
				tbVector3(m_vPos[2],m_fZValue),
				tbVector3(m_vPos[3],m_fZValue));
		}
	}
}

bool CThought::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	return true;
}
bool CThought::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	return true;
}

bool CThought::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	return false;
	//return poyCollision2(m_vPos,m_vWH,pvCenter,pvWH);
}

