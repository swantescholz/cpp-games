#include "CAnswer.h"

CAnswer::CAnswer(tbVector2 pvStartPos, string psThought) : 
	m_sSndReached("answer_reached")
{
	m_OT   = OT_ANSWER;
	m_sThought = psThought;
	m_TexAnswer.Load("answer");
	m_vPos = pvStartPos;
	m_vWH  = tbVector2(2.0f,6.0f);
}

CAnswer::~CAnswer()
{
}

void CAnswer::Update(list<S_LevelIntern> *palObjects) {
	return;
}

void CAnswer::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	m_TexAnswer.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
	glEnd();
}

bool CAnswer::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->GetOT() == OT_PLAYER) {
		CPlayer *player = dynamic_cast<CPlayer*>(pObj);
		if (player->Collision(m_vPos,m_vWH)) {
			DoReached();
			return true;
		}
	}
			
	return true;
}
bool CAnswer::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj->GetOT() == OT_PLAYER) {
		CPlayer *player = dynamic_cast<CPlayer*>(pObj);
		if (player->Collision(m_vPos,m_vWH)) {
			DoReached();
			return true;
		}
	}
			
	return true;
}

void CAnswer::DoReached() {
	if (g_pGS->m_iLevelToPlay + 1 == g_pGS->m_iAnswerPlace) { 	//when last not finished level,
		g_pGS->m_iAnswerPlace++;								//then answer higher
	}
	g_pGS->m_iLevelToPlay++;	//set level to play right
	g_pGS->m_GS = GS_MAIN_MENU;	//start in main_menu
	glcSound(m_sSndReached).Play();
	glcDelay(0.8f);
	throw glcException("answer was found"); //interrupt updating/pushing/moving(this was once VERY inportant)
}

void CAnswer::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPos - tbVector2(0.0,m_vWH.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(1.0,1.0,0.0,1.0),tbColor(1.0,1.0,0.0,1.0),tbColor(1.0,1.0,0.0,1.0),20.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

bool CAnswer::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	//return false;
	return poyCollision2(m_vPos,m_vWH,pvCenter,pvWH);
}

