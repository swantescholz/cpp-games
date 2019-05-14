#include "CLie.h"

const float CLie::c_fMinVelToExplode	= -4.0f;
const tbVector2 CLie::c_vMaxSpeed 		= tbVector2(15.0,25.0);
const tbVector2 CLie::c_vAcceleration 	= tbVector2(5.0,9.81);
const float CLie::c_fExplosionMaxTime	= 0.8f;
const float CLie::c_fExplosionMaxRadius	= 4.5f;
const float CLie::c_fExplosionEndVel	= 0.8f;

CLie::CLie(const CLie &rhs) : m_ciNumParticles(rhs.m_ciNumParticles), m_sSndExplode(rhs.m_sSndExplode)
{
	m_OT				= rhs.m_OT;
	m_bDeleted			= rhs.m_bDeleted;
	m_TexLie 			= rhs.m_TexLie;
	m_TexParticle		= rhs.m_TexParticle;
	m_vWH				= rhs.m_vWH;
	m_vPos				= rhs.m_vPos;
	m_vLastPos			= rhs.m_vLastPos;
	m_vVel				= rhs.m_vVel;
	m_bExploding		= rhs.m_bExploding;
	m_fExplosionTime 	= rhs.m_fExplosionTime;
	m_fExplosionRadius 	= rhs.m_fExplosionRadius;
	m_iCanShowThought	= rhs.m_iCanShowThought;
	m_sThought			= rhs.m_sThought; //derived
	if (rhs.m_bExploding == false) {
		m_aParticles = NULL;
	} else {
		m_aParticles	= new glcParticle[m_ciNumParticles];
		for (int i = 0; i< m_ciNumParticles; i++) {
			m_aParticles[i] = rhs.m_aParticles[i];
		}
	}
}

CLie::CLie(tbVector2 pvStartPos, string psThought) : m_ciNumParticles(500), m_sSndExplode("lie_explode")
{
	m_OT = OT_LIE;
	m_sThought = psThought;
	m_TexLie.Load("lie");
	m_TexParticle.Load("particles/circle1111");
	m_vWH = tbVector2(2.0,2.0);
	m_vPos = tbVector2(pvStartPos.x,pvStartPos.y);
	m_vLastPos = m_vPos;
	m_vVel = tbVector2(0,0);
	m_bExploding = false;
	m_aParticles = NULL;
	m_iCanShowThought = 0;
}

CLie::~CLie()
{
	GLC_SAFE_DELETE_ARRAY(m_aParticles);
}

void CLie::Update(list<S_LevelIntern> *palObjects) {
	if (m_bExploding == false) { //NO EXPLOSION
		if (tbVector3Length(m_vPos-m_vLastPos) > 1.0f) {
			g_pLogfile->fTextout(RED,"Jumping Lie!: lx: %f  ly: %f    nx: %f  ny: %f",m_vLastPos.x,m_vLastPos.y,m_vPos.x,m_vPos.y);
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
		
		//moving
		MoveY(palObjects);
		MoveX(palObjects);
		
		//updating thoughts show or not
		if (m_iCanShowThought == 2) {
			m_iCanShowThought = 1;
		} else if (m_iCanShowThought == 0) {
			m_iCanShowThought = 2;
		}
	} else { //EXPLOSION
		m_fExplosionTime += ELAPSED;
		if (m_fExplosionTime > c_fExplosionMaxTime) {
			Delete(); //explosion over, so delete me
		} else {
			m_fExplosionRadius += ELAPSED * (c_fExplosionMaxRadius / c_fExplosionMaxTime);
			
			int iSize = 4;
			int aiOrder[] = {OT_LIE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_PLAYER};
			for (int i = 0; i < iSize; i++) {
				for (list<S_LevelIntern>::iterator it1 = palObjects[aiOrder[i] ].begin(); it1 != palObjects[aiOrder[i] ].end(); it1++) {
					if (tbVector2Length(it1->pObj->GetPos() - m_vPos) < m_fExplosionRadius) {
						// within explosion, so destroy/kill/explode
						if (it1->pObj->GetOT() == OT_SOLID_ARGUMENT	) dynamic_cast<CSolidArgument*	>(it1->pObj)->DoDestroy(palObjects);
						if (it1->pObj->GetOT() == OT_RASH_ARGUMENT 	) dynamic_cast<CRashArgument* 	>(it1->pObj)->DoDestroy(palObjects);
						if (it1->pObj->GetOT() == OT_LIE 			) dynamic_cast<CLie* 			>(it1->pObj)->DoExplode(palObjects);
						if (it1->pObj->GetOT() == OT_PLAYER 		) dynamic_cast<CPlayer*			>(it1->pObj)->DoKill   (palObjects);
					}
				}
			}
			
			//updating particles
			glcParticle::SetElapsed(ELAPSED);
			for (int i= 0; i < m_ciNumParticles; i++) {
				m_aParticles[i].Update();
			}
			qsort(m_aParticles,m_ciNumParticles,sizeof(glcParticle),CompareFunction); //sorting
		}
	}
}

void CLie::Render() {
	float fZValue = (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ;
	if (m_bExploding == false) {
		m_TexLie.Bind();
		glBegin(GL_QUADS);
			glNormal3f(0,0,1);
			glTexCoord2f(0,0); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
			glTexCoord2f(1,0); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y-m_vWH.y/2.0f,fZValue).c);
			glTexCoord2f(1,1); glVertex3fv(tbVector3(m_vPos.x+m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
			glTexCoord2f(0,1); glVertex3fv(tbVector3(m_vPos.x-m_vWH.x/2.0f,m_vPos.y+m_vWH.y/2.0f,fZValue).c);
		glEnd();
	} else {
		m_TexParticle.Bind();
		glNormal3fv(tbVector3(0,0,1).c);
		glPushAttrib(GL_LIGHTING_BIT); //because meterial changes
		for (int i= 0; i < m_ciNumParticles; i++) {
			m_aParticles[i].Render();
		}
		glPopAttrib();
	}
}

void CLie::DoExplode(list<S_LevelIntern> *palObjects) {
	if (m_bExploding == false) {
		glcSound(m_sSndExplode).Play();
		AddThought(palObjects);
		
		m_bExploding 		= true;
		m_fExplosionTime 	= 0.0f;
		m_fExplosionRadius 	= 0.0f;
		
		m_aParticles = new glcParticle[m_ciNumParticles]; //generating particles
		
		//adding materials
		glcMaterial mat[3];
		mat[0] = glcMaterial(tbColor(1.0,0.0,0.0,1.0),tbColor(1.0,0.0,0.0,1.0),tbColor(1.0,0.0,0.0,1.0),50);
		mat[1] = glcMaterial(tbColor(0.8,0.4,0.1,1.0),tbColor(0.8,0.4,0.1,1.0),tbColor(0.8,0.4,0.1,1.0),40);
		mat[2] = glcMaterial(tbColor(0.5,0.4,0.2,0.0),tbColor(0.5,0.4,0.2,0.0),tbColor(0.5,0.4,0.2,0.0), 0);
		for (int i = 0; i < m_ciNumParticles; i++) {
			m_aParticles[i].AddKeyFrameMaterial(mat[0],0.0f);
			m_aParticles[i].AddKeyFrameMaterial(mat[1],0.7f);
			m_aParticles[i].AddKeyFrameMaterial(mat[2],1.0f);
		}
		
		//starting
		tbVector3 vPos = tbVector3(0,0,0);
		tbVector3 vVel = tbVector3(0,0,0);
		tbVector2 vTmp = tbVector2(0,0);
		tbVector3 vAcc = tbVector3(0,-10,0); //gravitation
		tbVector2 vWH  = tbVector2(1,1);
		float fLife    = 1.0f;
		float fFade    = 0.5f;
		float fMaxRadius 	= 0.0f; //special for a bit of randomness
		
		for (int i = 0; i < m_ciNumParticles; i++) {
			vPos  = m_vPos + tbVector3((tbVector2Random() * g_pTimer->Random(0.0f,m_vWH.x / 4.0f)), (OT_LAST - m_OT) * g_pGS->m_cfLayerGapZ * g_pTimer->Random(0.99f,1.01f)); //for z
			fMaxRadius	= c_fExplosionMaxRadius * g_pTimer->Random(0.1f, 1.0f);
			vTmp  = (tbVector2Random() * (2.0f * fMaxRadius / c_fExplosionMaxTime - c_fExplosionEndVel) );
			vVel  = tbVector3(vTmp.x,vTmp.y,0.0f);
			vAcc  = tbVector3Normalize(-vVel) * ( (tbVector3Length(vVel) - c_fExplosionEndVel) / c_fExplosionMaxTime);
			//vAcc  = tbVector3Normalize(-vVel) * ( (2.0f * c_fExplosionMaxRadius) / (c_fExplosionMaxTime * c_fExplosionMaxTime) -
			//			(2.0f * c_fExplosionEndVel / c_fExplosionMaxTime) ) * g_pTimer->Random(0.99f,1.6f);
			fFade = (1.0f / c_fExplosionMaxTime) * g_pTimer->Random(0.99f,1.01f);
			vWH   = tbVector2(0.5) * g_pTimer->Random(0.8f,1.2f);
			
			m_aParticles[i].SetWH(vWH);
			m_aParticles[i].SetFade(fFade);
			m_aParticles[i].Start(vPos,vVel,vAcc);
			m_aParticles[i].SetLife(fLife);
		}
	}
}

void CLie::MoveY(list<S_LevelIntern> *palObjects) {
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

void CLie::MoveX(list<S_LevelIntern> *palObjects) {
	return;
}

bool CLie::PushY(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj == this) return true; //its me!
	if (pObj->Collision(m_vPos,m_vWH) == false) return true; //no collision, so pObj my move
	
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
				if (pObj->GetOT() != OT_PLAYER && pObj->GetLastVel().y < c_fMinVelToExplode) {
					DoExplode(palObjects); //pressing me from above so i explode
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

bool CLie::PushX(list<S_LevelIntern> *palObjects, CObject *pObj) {
	if (pObj == this) return true; //its me!
	if (pObj->Collision(m_vPos,m_vWH) == false) return true; //no collision, so pObj my move
	
	bool bReturn = true; //pushing successful?
	
	int iSize = 4;
	int aiOrder[] = {OT_LIE,OT_RASH_ARGUMENT,OT_SOLID_ARGUMENT,OT_PLAYER};
	for (int i = 0; i < iSize; i++) {
		if (pObj->GetOT() == aiOrder[i]) {
			if ( (pObj->GetOT() != OT_RASH_ARGUMENT) ? (true) : (dynamic_cast<CRashArgument*>(pObj)->IsPushed())) { 
				if (pObj->GetVel().x >= 0.0f) {	//here was the >= istead of just > VERY important
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
			} else {
				if (pObj->GetOT() == OT_RASH_ARGUMENT) {
					pObj->SetVel(tbVector2(0.0f,pObj->GetVel().y));
					pObj->SetPos(poyAdjustPushingRectX(pObj->GetPos(),pObj->GetWH(),m_vPos,m_vWH));
					bReturn = false; //here was the not just return false VERY important
				}
			}
			if (bReturn == true) {m_vVel.x = pObj->GetVel().x;}
			
			if (pObj->GetOT() == OT_PLAYER) {
				if (m_iCanShowThought == 1) {
					AddThought(palObjects);
				}
				m_iCanShowThought = 0;
			}
		}
	}
	return bReturn;
}

void CLie::AddThought(list<S_LevelIntern> *palObjects) {
	S_LevelIntern tmpObj;
	int iSize 		= 16;
	float fHeight   = g_pGS->m_fStdThoughtH;
	float fWidth    = fHeight * g_pGS->m_cfLetterWidth * m_sThought.length(); //estimate width of text
	tbVector2 vWH   = tbVector2(fWidth, fHeight);
	tbVector2 vPos  = m_vPos - tbVector2(0.0,m_vWH.y/2.0f);
	tbVector2 vVel  = tbVector2(0.0,-1.0);
	tbVector2 vAcc  = tbVector2(0.0,0.0);
	float fFade		= g_pGS->m_cfStdThoughtFade;
	
	tmpObj.pObj = new CThought(m_sThought, "FreeSans", iSize, glcMaterial(tbColor(0.01,0.01,0.01,1.0),tbColor(0.01,0.01,0.01,1.0),tbColor(0.0,0.0,0.0,1.0),0.0),
					tbVector2(-vWH.x,-vWH.y), tbVector2(vWH.x,-vWH.y), tbVector2(vWH.x,vWH.y), tbVector2(-vWH.x,vWH.y) );
	dynamic_cast<CThought*>(tmpObj.pObj)->Start(vPos,vVel,vAcc,fFade);
	palObjects[OT_THOUGHT].push_back(tmpObj);
}

bool CLie::Collision(tbVector2 pvCenter, tbVector2 pvWH) {
	if (m_bDeleted	 == true) return false;
	if (m_bExploding == true) return false;
	return poyCollision2(m_vPos,m_vWH,pvCenter,pvWH);
}



