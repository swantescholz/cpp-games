#include "RoeLight.h"
#include "RoeException.h"

namespace Roe {
	
	const int Light::s_ciMaxLights = 8;
	int  Light::s_iCounter   = 0;
	bool Light::s_bLights [] = {false,false,false,false,false,false,false,false};
	int  Light::s_iNumUses[] = {0,0,0,0, 0,0,0,0};
	
	Light::Light(Vector3 vPos, bool bActive, bool bPos, Color c1, Color c2, Color c3, float kc, float kl, float kq, Vector3 vDir, float fEx, float fCut)
		: m_vPos(vPos)
	{
		s_iCounter++;
		setActive      (bActive);
		setPositional  (bPos);
		setMaterial    (c1,c2,c3);
		setAttentuation(kc,kl,kq);
		setSpot        (vDir,fEx,fCut);
		
		m_iLight = -1;
		
		for (int i = 0; i < s_ciMaxLights; i++) {
			if (s_bLights[i] == false) {
				m_iLight = i;
				s_bLights[i] = true;
				break;
			}
		}
		if (m_iLight == -1) throw Exception("too many lights", "Light()", "RoeLight.h");
		if (m_iLight == 0) m_eLight = GL_LIGHT0;
		else if (m_iLight == 1) m_eLight = GL_LIGHT1;
		else if (m_iLight == 2) m_eLight = GL_LIGHT2;
		else if (m_iLight == 3) m_eLight = GL_LIGHT3;
		else if (m_iLight == 4) m_eLight = GL_LIGHT4;
		else if (m_iLight == 5) m_eLight = GL_LIGHT5;
		else if (m_iLight == 6) m_eLight = GL_LIGHT6;
		else if (m_iLight == 7) m_eLight = GL_LIGHT7;
		
		if (!glIsEnabled(m_eLight)) glEnable(m_eLight);
		else throw Exception("warning: new light already enabled", "Light()", "RoeLight.h");
		
		s_iNumUses[m_iLight]++;
	}
	
	Light::Light(const Light &rhs) : m_vPos(rhs.m_vPos) {
		s_iCounter++;
		setActive      (rhs.m_bActive);
		setPositional  (rhs.m_bPositional);
		setMaterial    (rhs.m_mat.amb,rhs.m_mat.diff,rhs.m_mat.spec);
		setAttentuation(rhs.m_fkc,rhs.m_fkl,rhs.m_fkq);
		setSpot        (rhs.m_vSpotDir,rhs.m_fSpotExponent,rhs.m_fSpotCutoff);
		
		m_iLight = -1;
		
		for (int i = 0; i < s_ciMaxLights; i++) {
			if (s_bLights[i] == false) {
				m_iLight = i;
				s_bLights[i] = true;
				break;
			}
		}
		if (m_iLight == -1) throw Exception("too many lights", "Light(Light&)", "RoeLight.h");
		if (m_iLight == 0) m_eLight = GL_LIGHT0;
		else if (m_iLight == 1) m_eLight = GL_LIGHT1;
		else if (m_iLight == 2) m_eLight = GL_LIGHT2;
		else if (m_iLight == 3) m_eLight = GL_LIGHT3;
		else if (m_iLight == 4) m_eLight = GL_LIGHT4;
		else if (m_iLight == 5) m_eLight = GL_LIGHT5;
		else if (m_iLight == 6) m_eLight = GL_LIGHT6;
		else if (m_iLight == 7) m_eLight = GL_LIGHT7;
		
		if (!glIsEnabled(m_eLight)) glEnable(m_eLight);
		else throw Exception("warning: new light already enabled", "Light(Light&)", "RoeLight.h");
		
		s_iNumUses[m_iLight]++;
	}
	
	Light::~Light()
	{
		s_iNumUses[m_iLight]--;
		if (s_iNumUses[m_iLight] <= 0) {
			glDisable(m_eLight);
			s_iCounter--;
			s_bLights[m_iLight] = false;
		}
	}
	
	void Light::lightModel(const Color &col, bool local_viewer, bool two_sides) {
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT     , col.rg);
		glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, local_viewer);
		glLightModeli (GL_LIGHT_MODEL_TWO_SIDE    , two_sides);
	}
	
	
	void Light::transform(const Matrix &m) {
		Matrix::transformCoords(m_vPos, m);
		Matrix::transformNormal(m_vSpotDir, m);
	}
	
	void Light::light() {
		if (m_bActive && glIsEnabled(GL_LIGHTING)) {
			//light character
			glLightfv(m_eLight, GL_AMBIENT , m_mat.amb.rg);
			glLightfv(m_eLight, GL_DIFFUSE , m_mat.diff.rg);
			glLightfv(m_eLight, GL_SPECULAR, m_mat.spec.rg);
			
			//position
			//c_vLightCenter[m_iLight] = m_Sphere.GetCenter(); // die statischen positionen updaten
			GLfloat light_position[] = { m_vPos.x, m_vPos.y, m_vPos.z, m_bPositional?1.0f:0.0f };
			glLightfv(m_eLight, GL_POSITION, light_position);
			
			//attentuation
			glLightf(m_eLight, GL_CONSTANT_ATTENUATION, m_fkc);
			glLightf(m_eLight, GL_LINEAR_ATTENUATION, m_fkl);
			glLightf(m_eLight, GL_QUADRATIC_ATTENUATION, m_fkq);
			
			//spot
			glLightf (m_eLight, GL_SPOT_EXPONENT, m_fSpotExponent);
			glLightf (m_eLight, GL_SPOT_CUTOFF  , m_fSpotCutoff);
			glLightfv(m_eLight,GL_SPOT_DIRECTION, m_vSpotDir.rg);
			
			
		}
	}

} // namespace Roe

