#include "RoeParticle.h"
#include <algorithm>

namespace Roe {
	
	//static:
	float   Particle::s_fElapsed = 0.0f;
	Vector3 Particle::s_vCamPos  = Vector3(0,0,0);
	
	Particle::Particle(bool pbLighted)
	{
		m_Col = Color(1,1,1,1);
		m_Mat = Material();
		m_bLighted = pbLighted;
		m_vWH = Vector2(1.1f,1.1f);
		m_bActive = false;
		m_fLife = 1.0f;
		m_fFade = 0.5f;
		m_kfiCol = new KeyframeInterpolatorLinear<Color>;
		m_kfiMat = new KeyframeInterpolatorLinear<Material>;
	}
	Particle::Particle(const Particle& rhs) {
		m_Col     = rhs.m_Col;
		m_Mat     = rhs.m_Mat;
		m_bLighted= rhs.m_bLighted;
		m_vWH     = rhs.m_vWH;
		m_bActive = rhs.m_bActive;
		m_fLife   = rhs.m_fLife;
		m_fFade   = rhs.m_fFade;
		m_vPos    = rhs.m_vPos;
		m_vVel    = rhs.m_vVel;
		m_vAcc    = rhs.m_vAcc;
		m_kfiCol = new KeyframeInterpolatorLinear<Color>;
		m_kfiMat = new KeyframeInterpolatorLinear<Material>;
		*m_kfiCol = *(rhs.m_kfiCol); //change, before: ptr = ptr;
		*m_kfiMat = *(rhs.m_kfiMat);
	}
	Particle& Particle::operator = (const Particle& rhs)	{
		m_Col     = rhs.m_Col;
		m_Mat     = rhs.m_Mat;
		m_bLighted= rhs.m_bLighted;
		m_vWH     = rhs.m_vWH;
		m_bActive = rhs.m_bActive;
		m_fLife   = rhs.m_fLife;
		m_fFade   = rhs.m_fFade;
		m_vPos    = rhs.m_vPos;
		m_vVel    = rhs.m_vVel;
		m_vAcc    = rhs.m_vAcc;
		*m_kfiCol = *(rhs.m_kfiCol);
		*m_kfiMat = *(rhs.m_kfiMat);
		return *this;
	}
	
	
	Particle::~Particle()
	{
		ROE_SAFE_DELETE(m_kfiCol);
		ROE_SAFE_DELETE(m_kfiMat);
	}
	
	void Particle::addKeyframeColor   (Color    pCol, float fTime) {m_kfiCol->addKeyframe(fTime, pCol);}
	void Particle::addKeyframeMaterial(Material pMat, float fTime) {m_kfiMat->addKeyframe(fTime, pMat);}
	
	void Particle::start(Vector3 pvPos, Vector3 pvVel, Vector3 pvAcc) {
		m_vPos = pvPos;
		m_vVel = pvVel;
		m_vAcc = pvAcc;
		m_fLife = 1.0f;
		m_bActive = true;
	}
	
	void Particle::update() {
		//decrease life
		m_fLife -= m_fFade * s_fElapsed;
		
		if (m_fLife < 0.0f) {
			m_bActive = false; //dead
		} else {
			//moving
			m_vVel  += m_vAcc * s_fElapsed;
			m_vPos  += m_vVel * s_fElapsed;
			
			//change colors/materials
			//here error possible, when false kfi is chosen, TODO
			if (m_bLighted) {
				m_Mat = m_kfiMat->interpolate(1.0f - m_fLife); //inverted life
			} else {
				m_Col = m_kfiCol->interpolate(1.0f - m_fLife); //inverted life
			}
		}
	}
	
	void Particle::render() {
		if (m_bActive == false) return;
		
		if (glIsEnabled(GL_TEXTURE_2D)) {
			if (m_bLighted) {
				m_Mat.render();
			} else {
				glColor3fv(m_Col.rg);
			}
			glBegin(GL_TRIANGLE_STRIP); //is faster than quads
				glTexCoord2f(1,1); glVertex3fv((m_vPos+Vector3( m_vWH.x/2, m_vWH.y/2,0.0)).rg);
				glTexCoord2f(0,1); glVertex3fv((m_vPos+Vector3(-m_vWH.x/2, m_vWH.y/2,0.0)).rg);
				glTexCoord2f(1,0); glVertex3fv((m_vPos+Vector3( m_vWH.x/2,-m_vWH.y/2,0.0)).rg);
				glTexCoord2f(0,0); glVertex3fv((m_vPos+Vector3(-m_vWH.x/2,-m_vWH.y/2,0.0)).rg);
			glEnd();
		} else {
			if (m_bLighted) {
				m_Mat.render();
			} else {
				glColor3fv(m_Col.rg);
			}
			glBegin(GL_TRIANGLE_STRIP);
				glVertex3fv((m_vPos+Vector3( m_vWH.x/2, m_vWH.y/2,0.0)).rg);
				glVertex3fv((m_vPos+Vector3(-m_vWH.x/2, m_vWH.y/2,0.0)).rg);
				glVertex3fv((m_vPos+Vector3( m_vWH.x/2,-m_vWH.y/2,0.0)).rg);
				glVertex3fv((m_vPos+Vector3(-m_vWH.x/2,-m_vWH.y/2,0.0)).rg);
			glEnd();
		}
	}
	
	//fast methods (not advised)
	void Particle::updateFast() {
		//decrease life
		m_fLife -= m_fFade * s_fElapsed;
		
		if (m_fLife < 0.0f) {
			m_bActive = false; //dead
		} else {
			//moving
			m_vVel  += m_vAcc * s_fElapsed;
			m_vPos  += m_vVel * s_fElapsed;
		}
	}
	
	void Particle::renderFast() {
		glBegin(GL_TRIANGLE_STRIP); //is faster than quads
			glTexCoord2f(1,1); glVertex3fv((m_vPos+Vector3( m_vWH.x/2, m_vWH.y/2,0.0)).rg);
			glTexCoord2f(0,1); glVertex3fv((m_vPos+Vector3(-m_vWH.x/2, m_vWH.y/2,0.0)).rg);
			glTexCoord2f(1,0); glVertex3fv((m_vPos+Vector3( m_vWH.x/2,-m_vWH.y/2,0.0)).rg);
			glTexCoord2f(0,0); glVertex3fv((m_vPos+Vector3(-m_vWH.x/2,-m_vWH.y/2,0.0)).rg);
		glEnd();
	}
	
	void Particle::sortZ(Particle *p, int num, Vector3 vCamPos) {
		s_vCamPos = vCamPos;
		std::sort(p, p+num, Particle::compareFunction1);
	}
	void Particle::sortZ(Particle **p, int num, Vector3 vCamPos) {
		s_vCamPos = vCamPos;
		std::sort(p, p+num, Particle::compareFunction2);
	}
	
} // namespace Roe
