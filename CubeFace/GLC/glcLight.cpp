#include "glcLight.h"

int glcLight::c_iCounter = 0;
bool glcLight::c_bLights[] = {false,false,false,false,false,false,false,false};
tbVector3 glcLight::c_vLightCenter[] = {tbVector3(0.0f,0.0f,0.0f),tbVector3(0.0f,0.0f,0.0f),tbVector3(0.0f,0.0f,0.0f),tbVector3(0.0f,0.0f,0.0f),
								tbVector3(0.0f,0.0f,0.0f),tbVector3(0.0f,0.0f,0.0f),tbVector3(0.0f,0.0f,0.0f),tbVector3(0.0f,0.0f,0.0f)};
bool glcLight::c_bPositional[] = {false,false,false,false,false,false,false,false};

glcLight::glcLight(const glcSphere &sphere, bool bActive, bool bPos, tbColor c1, tbColor c2, tbColor c3, float kc, float kl, float kq, tbVector3 vDir, float fEx, float fCut)
	: m_Sphere(sphere)
{
	c_iCounter++;
	SetActive(bActive);
	SetPositional(bPos);
	SetMaterial(c1,c2,c3);
	SetAttentuation(kc,kl,kq);
	SetSpot(vDir,fEx,fCut);
	
	m_iLight = -1;
	
	for (int i = 0; i < MAXLIGHTS; i++) {
		if (!c_bLights[i]) {
			m_iLight = i;
			c_bLights[i] = true;
			break;
		}
	}
	if (m_iLight == -1) throw "too many lights";
	if (m_iLight == 0) m_eLight = GL_LIGHT0;
	else if (m_iLight == 1) m_eLight = GL_LIGHT1;
	else if (m_iLight == 2) m_eLight = GL_LIGHT2;
	else if (m_iLight == 3) m_eLight = GL_LIGHT3;
	else if (m_iLight == 4) m_eLight = GL_LIGHT4;
	else if (m_iLight == 5) m_eLight = GL_LIGHT5;
	else if (m_iLight == 6) m_eLight = GL_LIGHT6;
	else if (m_iLight == 7) m_eLight = GL_LIGHT7;
	
	if (!glIsEnabled(m_eLight)) glEnable(m_eLight);
	
	c_bPositional[m_iLight] = bPos;
}

glcLight::~glcLight()
{
	glDisable(m_eLight);
	c_iCounter--;
	c_bLights[m_iLight] = false;
}

void glcLight::LightModel(const tbColor &col, bool local_viewer, bool two_sides) {
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,col.c);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, local_viewer);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, two_sides);
}
float glcLight::DistanceMin(const tbVector3 &v) {
	float f = -1.0f, f2;
	for (int i = 0; i < MAXLIGHTS; i++) {
		if (c_bLights[i] && c_bPositional[i]) {
			f2 = tbVector3Length(v - c_vLightCenter[i]);
			if (f2 < f || f == -1.0f)
				f = f2;
		}
	}
	return f;
}

void glcLight::Transform(const tbMatrix &m) {
	m_Sphere.Transform(m);
}

void glcLight::Render() {
	if (m_bActive) {
		//light character
		glLightfv(m_eLight, GL_AMBIENT, m_mat.ambient.c);
		glLightfv(m_eLight, GL_DIFFUSE, m_mat.diffuse.c);
		glLightfv(m_eLight, GL_SPECULAR, m_mat.specular.c);
		
		//position
		c_vLightCenter[m_iLight] = m_Sphere.GetCenter(); // die statischen positionen updaten
		GLfloat light_position[] = { m_Sphere.GetCenter().x, m_Sphere.GetCenter().y, m_Sphere.GetCenter().z, m_bPositional };
		glLightfv(m_eLight, GL_POSITION, light_position);
		
		//attentuation
		glLightf(m_eLight, GL_CONSTANT_ATTENUATION, m_fkc);
		glLightf(m_eLight, GL_LINEAR_ATTENUATION, m_fkl);
		glLightf(m_eLight, GL_QUADRATIC_ATTENUATION, m_fkq);
		
		//spot
		glLightf(m_eLight, GL_SPOT_EXPONENT, m_fSpotExponent);
		glLightf(m_eLight, GL_SPOT_CUTOFF, m_fSpotCutoff);
		glLightfv(m_eLight,GL_SPOT_DIRECTION, m_vSpotDirection.c);
		
		glDisable(GL_LIGHTING);
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			m_Sphere.Render();
		glEnable(GL_LIGHTING);	
	}
}