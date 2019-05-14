#include "glcParticle.h"

//static:
float glcParticle::c_fElapsed = 0.0f;

glcParticle::glcParticle(bool pbLighted)
{
	m_Col = tbColor(1,1,1,1);
	m_Mat = glcMaterial();
	m_bLighted = pbLighted;
	m_vWH = tbVector2(1.1f,1.1f);
	m_bActive = false;
	m_fLife = 1.0f;
	m_fFade = 0.5f;
	
	m_kfiCol = new glcKeyFrameInterpolator<tbColor>;
	m_kfiMat = new glcKeyFrameInterpolator<glcMaterial>;
}
glcParticle::glcParticle(const glcParticle& rhs) {
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
	m_kfiCol = new glcKeyFrameInterpolator<tbColor>;
	m_kfiMat = new glcKeyFrameInterpolator<glcMaterial>;
	*m_kfiCol = *(rhs.m_kfiCol); //change, before: ptr = ptr;
	*m_kfiMat = *(rhs.m_kfiMat);
}
glcParticle& glcParticle::operator = (const glcParticle& rhs)	{
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


glcParticle::~glcParticle()
{
	GLC_SAFE_DELETE(m_kfiCol);
	GLC_SAFE_DELETE(m_kfiMat);
}

void glcParticle::AddKeyFrameColor   (tbColor     pCol, float fTime) {
	m_kfiCol->AddKeyFrame(pCol, fTime);
}
void glcParticle::AddKeyFrameMaterial(glcMaterial pMat, float fTime) {
	m_kfiMat->AddKeyFrame(pMat, fTime);
}

void glcParticle::Start(tbVector3 pvPos, tbVector3 pvVel, tbVector3 pvAcc) {
	m_vPos = pvPos;
	m_vVel = pvVel;
	m_vAcc = pvAcc;
	m_fLife = 1.0f;
	m_bActive = true;
}

void glcParticle::Update() {
	//decrease life
	m_fLife -= m_fFade * c_fElapsed;
	
	if (m_fLife < 0.0f) {
		m_bActive = false; //dead
	} else {
		//moving
		m_vVel  += m_vAcc * c_fElapsed;
		m_vPos  += m_vVel * c_fElapsed;
		
		//change colors/materials
		//here error possible, when false kfi is chosen, TODO
		if (m_bLighted) {
			m_Mat = m_kfiMat->Interpolate(1.0f - m_fLife); //inverted life
		} else {
			m_Col = m_kfiCol->Interpolate(1.0f - m_fLife); //inverted life
		}
	}
}

void glcParticle::Render() {
	if (m_bActive == false) return;
	
	if (glIsEnabled(GL_TEXTURE_2D)) {
		if (m_bLighted) {
			m_Mat.Render();
		} else {
			glColor3fv(m_Col.c);
		}
		glBegin(GL_TRIANGLE_STRIP); //is faster than quads
			glTexCoord2f(1,1); glVertex3fv((m_vPos+tbVector3( m_vWH.x/2, m_vWH.y/2,0.0)).c);
			glTexCoord2f(0,1); glVertex3fv((m_vPos+tbVector3(-m_vWH.x/2, m_vWH.y/2,0.0)).c);
			glTexCoord2f(1,0); glVertex3fv((m_vPos+tbVector3( m_vWH.x/2,-m_vWH.y/2,0.0)).c);
			glTexCoord2f(0,0); glVertex3fv((m_vPos+tbVector3(-m_vWH.x/2,-m_vWH.y/2,0.0)).c);
		glEnd();
	} else {
		if (m_bLighted) {
			m_Mat.Render();
		} else {
			glColor3fv(m_Col.c);
		}
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3fv((m_vPos+tbVector3( m_vWH.x/2, m_vWH.y/2,0.0)).c);
			glVertex3fv((m_vPos+tbVector3(-m_vWH.x/2, m_vWH.y/2,0.0)).c);
			glVertex3fv((m_vPos+tbVector3( m_vWH.x/2,-m_vWH.y/2,0.0)).c);
			glVertex3fv((m_vPos+tbVector3(-m_vWH.x/2,-m_vWH.y/2,0.0)).c);
		glEnd();
	}
}

//fast methods (not advised)
void glcParticle::UpdateFast() {
	//decrease life
	m_fLife -= m_fFade * c_fElapsed;
	
	if (m_fLife < 0.0f) {
		m_bActive = false; //dead
	} else {
		//moving
		m_vVel  += m_vAcc * c_fElapsed;
		m_vPos  += m_vVel * c_fElapsed;
	}
}

void glcParticle::RenderFast() {
	glBegin(GL_TRIANGLE_STRIP); //is faster than quads
		glTexCoord2f(1,1); glVertex3fv((m_vPos+tbVector3( m_vWH.x/2, m_vWH.y/2,0.0)).c);
		glTexCoord2f(0,1); glVertex3fv((m_vPos+tbVector3(-m_vWH.x/2, m_vWH.y/2,0.0)).c);
		glTexCoord2f(1,0); glVertex3fv((m_vPos+tbVector3( m_vWH.x/2,-m_vWH.y/2,0.0)).c);
		glTexCoord2f(0,0); glVertex3fv((m_vPos+tbVector3(-m_vWH.x/2,-m_vWH.y/2,0.0)).c);
	glEnd();
}
