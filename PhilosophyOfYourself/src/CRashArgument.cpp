#include "CRashArgument.h"

const tbVector2 CRashArgument::c_vMaxSpeed 	= tbVector2(15.0,25.0);
const tbVector2 CRashArgument::c_vAcceleration = tbVector2(5.0,9.81);

CRashArgument::CRashArgument(tbVector2 pvStartPos, string psThought)
{
	m_OT = OT_RASH_ARGUMENT;
	m_sThought = psThought;
	m_TexSolidArgument.Load("rash_argument");
	m_vWH = tbVector2(2.0,2.0);
	m_vPos = tbVector2(pvStartPos.x,pvStartPos.y);
	m_vLastPos = m_vPos;
	m_vVel = tbVector2(0,0);
	m_bIsPushed = false;
	m_iCanShowThought = 0;
}

CRashArgument::~CRashArgument()
{
}

void CRashArgument::Update(list<S_LevelIntern> *palObjects) {
	if (tbVector3Length(m_vPos-m_vLastPos) > 1.0f) {
		g_pLogfile->fTextout(RED,"Jumping solid argument!: lx: %f  ly: %f    nx: %f  ny: %f",m_vLastPos.x,m_vLastPos.y,m_vPos.x,m_vPos.y);
	}
	m_vLastPos = m_vPos;	//saving last position
	m_vLastVel = m_vVel;	//saving last velocity
	
	m_vVel.y -= c_vAcceleration.y * ELAPSED; //falling
	
	//y-movement-checking
	if (m_vVel.y >  c_vMaxSpeed.y) m_vVel.y =  c_vMaxSpeed.y;
	if (m_vVel.y < -c_vMaxSpeed.y) m_vVel.y = -c_vMaxSpeed.y;
	
	//x-movement-checking
	if (m_vVel.x >  c_vMaxSpeed.x) m_vVel.x =  c_vMaxSpeed.x;
	if (m_vVel.x < -c_vMaxSpeed.x) m_vVel.x = -c_vMaxSpeed.x;
	
	if (m_vVel.x != 0.0f) {
		m_bIsPushed = false; //maybe sliding
	}
	
	//moving
	MoveY(palObjects);
	MoveX(palObjects);
	
	//updating thoughts show or not
	if (m_iCanShowThought == 2) {
		m_iCanShowThought = 1;
	} else if (m_iCanShowThought == 0) {
		m_iCanShowThought = 2;
	}
}

void CRashArgument::MoveY(list<S_LevelIntern> *palObjects) {
	m_vPos.y += m_vVel.y * ELAPSED;
	if (m_vPos.y == m_vLastPos.y) return; //no moving occurs, so no pushing occurs, so return
	
	int iSize = 10;
	int aiOrder[] = {OT_AXIOM,OT_UNKNOWN,OT_FALSE_BELIEF,OT_PROOF,OT_ADVICE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_LIE,OT_IGNORANCE,OT_PLAYER};
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

void CRashArgument::MoveX(list<S_LevelIntern> *palObjects) {
	m_vPos.x += m_vVel.x * ELAPSED;
	if (m_vPos.x == m_vLastPos.x) return; //no moving occurs, so no pushing occurs, so return
	
	int iSize = 10;
	int aiOrder[] = {OT_AXIOM,OT_UNKNOWN,OT_FALSE_BELIEF,OT_PROOF,OT_ADVICE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_PLAYER,OT_LIE,OT_IGNORANCE};
	for (int i = 0; i < iSize; i++) {
		for (list<S_LevelIntern>::iterator it1 = palObjects[aiOrder[i] ].begin(); it1 != palObjects[aiOrder[i] ].end(); it1++) {
			if (it1->pObj->Collision(m_vPos,m_vWH) && it1->pObj != this) {
				if (it1->pObj->PushX(palObjects,this) == false) {
					if (aiOrder[i] == OT_RASH_ARGUMENT || aiOrder[i] == OT_SOLID_ARGUMENT || aiOrder[i] == OT_PLAYER || aiOrder[i] == OT_LIE) {
						//nothing (pushing maybe other blocks)
					} else {
						break; //further pushing shall NOT be possible
					}
				}
			}
		}
	}
}

bool CRashArgument::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj == this) return true; //its me!
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	int iSize = 4;
	int aiOrder[] = {OT_LIE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			if (pObj->GetVel().y == 0.0f) {
				//does not occur
			} else if (pObj->GetPos().y >= m_vPos.y){
				//the offset here was once VERY important
				pObj->SetPos(poyAdjustPushingRectY(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH+tbVector2(g_pGS->m_cfOffset)));
				pObj->SetVel(tbVector2(pObj->GetVel().x,m_vVel.y));
				if (pObj->GetOT() == OT_LIE) {
					if (dynamic_cast<CLie*>(pObj)->GetLastVel().y < CLie::c_fMinVelToExplode)
						dynamic_cast<CLie*>(pObj)->DoExplode(palObjects); //lie is falling on solid_argument, so explode
				}
			} else if (pObj->GetPos().y <  m_vPos.y){
				pObj->SetPos(poyAdjustPushingRectY(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH));
				pObj->SetVel(tbVector2(pObj->GetVel().x,m_vVel.y));
			}
			
			if (pObj->GetOT() == OT_PLAYER) {
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

bool CRashArgument::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj == this) return true; //its me!
	if (pObj->Collision(m_vPos,m_vWH) == false) return true;
	
	bool bReturn = true; //pushing successful?
	
	int iSize = 4;
	int aiOrder[] = {OT_LIE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			if (pObj->GetOT() != OT_RASH_ARGUMENT) {
				m_bIsPushed = true; // I am pushed in x, not just sliding
				if (pObj->GetVel().x >= 0.0f) {	//here was the >= instead of just > VERY important
					m_vPos = poyAdjustPushingRectX(m_vPos,m_vWH,pObj->GetPos(),pObj->GetWH());
				} else if (pObj->GetVel().x <  0.0f) {
					m_vPos = poyAdjustPushingRectX(m_vPos,m_vWH,pObj->GetPos(),pObj->GetWH());
				}
				for (int i = 0; i < OT_LAST; i++) {
					for (list<S_LevelIntern>::iterator it1 = palObjects[i].begin(); it1 != palObjects[i].end(); it1++) {
						if (it1->pObj->Collision(m_vPos,m_vWH)) {
							if (it1->pObj == this) continue; //its me!
							if (it1->pObj != pObj) { //not the prameter object
								if (it1->pObj->PushX(palObjects, this) == false) {
									pObj->SetVel(tbVector2(0.0f,pObj->GetVel().y));
									pObj->SetPos(poyAdjustPushingRectX(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH));
									bReturn = false; //here was the not just return false VERY important
								}
							}
						}
					}
				}
				if (bReturn == true) {m_vVel.x = pObj->GetVel().x;}
				
				if (pObj->GetOT() == OT_PLAYER) {
					if (m_iCanShowThought == 1) {
						AddThought(palObjects);
					}
					m_iCanShowThought = 0;
				}
			} else { //RASH_ARGUMENT
				m_bIsPushed = true; // I am pushed in x, not just sliding
				CRashArgument *rashArgument = dynamic_cast<CRashArgument*>(pObj);
				if (rashArgument->GetVel().x >= 0.0f) {	//here was the >= istead of just > VERY important
					m_vPos = poyAdjustPushingRectX(m_vPos,m_vWH,rashArgument->GetPos(),rashArgument->GetWH());
					m_vVel.x = rashArgument->GetVel().x;
					m_bIsPushed = rashArgument->IsPushed(); //same state than pusher
				} else if (rashArgument->GetVel().x <  0.0f) {
					m_vPos = poyAdjustPushingRectX(m_vPos,m_vWH,rashArgument->GetPos(),rashArgument->GetWH());
					m_vVel.x = rashArgument->GetVel().x;
					m_bIsPushed = rashArgument->IsPushed(); //same state than pusher
				}
				for (int i = 0; i < OT_LAST; i++) {
					for (list<S_LevelIntern>::iterator it1 = palObjects[i].begin(); it1 != palObjects[i].end(); it1++) {
						if (it1->pObj->Collision(m_vPos,m_vWH)) {
							if (it1->pObj->GetOT() == m_OT) {if(dynamic_cast<CRashArgument*>(it1->pObj) == this) continue;} //its me!
							if (it1->pObj != pObj) { //not the prameter object
								if (it1->pObj->PushX(palObjects, this) == false) {
									rashArgument->SetVel(tbVector2(0.0f,rashArgument->GetVel().y));
									rashArgument->SetPos(poyAdjustPushingRectX(rashArgument->GetPos(),rashArgument->GetWH(),m_vPos,m_vWH));
									bReturn = false; //here was the not just return false VERY important
								}
							}
						}
					}
				}
				if (bReturn == true) {m_vVel.x = rashArgument->GetVel().x;}
			}
		}
	}
	return bReturn;
}

void CRashArgument::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	m_TexSolidArgument.Bind();
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
		glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
	glEnd();
}

void CRashArgument::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPos - tbVector2(0.0,m_vWH.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(0.33,0.33,0.33,1.0),tbColor(0.33,0.33,0.33,1.0),tbColor(0.0,0.0,0.0,1.0),0.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

void CRashArgument::DoDestroy(list<S_LevelIntern> *palObjects) {
	AddThought(palObjects);
	Delete();
}

bool CRashArgument::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	if (m_bDeleted == true) return false;
	return poyCollision2(m_vPos,m_vWH,pvCenter,pvWH);
}

