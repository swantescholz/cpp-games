#ifndef GLCLIGHT_H_
#define GLCLIGHT_H_

#define GLC_MAXLIGHTS 8

#include "glcSphere.h"
#include "glcSDL_GL.h"

class glcLight
{
public:
	glcLight(const glcSphere &sphere, bool bActive = true, bool bPos = true,
			tbColor c1 = tbColor(0.0f,0.0f,0.0f,1.0f), tbColor c2 = tbColor(1.0f,1.0f,1.0f,1.0f), tbColor c3 = tbColor(1.0f,1.0f,1.0f,1.0f),
			float kc = 1.0f, float kl = 0.0f, float kq = 0.0f, tbVector3 vDir = tbVector3(0.0f,0.0f,1.0f), float fEx = 0.0f, float fCut = 180.0f);
	
	glcLight(const glcLight &rhs);
	~glcLight();
	
	void Transform(const tbMatrix &m);
	void Render();										//das licht leuchtet(vor objekten, nach camera)
	tbVector3 GetCenter() const {return m_Sphere.GetCenter();}
	
	//aktivieren/ deaktivieren
	void Enable() {glEnable(m_eLight); m_bActive = true;}
	void Disable() {glDisable(m_eLight); m_bActive = false;}
	bool IsEnabled() const {return glIsEnabled(m_eLight);}
	
	//das lightmodel festsetzen
	static void LightModel(const tbColor &col = tbColor(0.2f,0.2f,0.2f,1.0f), bool local_viewer = false, bool two_sides = false);
	static void LightModelAmb(const tbColor &col = tbColor(0.2f,0.2f,0.2f,1.0f)) {glLightModelfv(GL_LIGHT_MODEL_AMBIENT,col.c);}	//ambient color
	static void LightModelLoc(bool b = false) {glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, b);}				//local viewer?
	static void LightModelTwo(bool b = false) {glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, b);}					//two-sided lighting?
	
	void SetPositional(bool b) 	{m_bPositional = b;}	//positionales lichts oder infinite
	void SetActive(bool b) 		{m_bActive = b;}		//scheint das licht?
	bool GetPositional	() const	{return m_bPositional;}
	bool GetActive		() const 	{return m_bActive;}
	
	
	//das material festsetzen
	void SetMaterial(S_Material_ads mat)	{m_mat = mat;}
	void SetMaterial(tbColor amb, tbColor diff, tbColor spec) {m_mat.ambient = amb; m_mat.diffuse = diff; m_mat.specular = spec;}
	void SetMaterialAmb (tbColor amb)  {m_mat.ambient  = amb ;}
	void SetMaterialDiff(tbColor diff) {m_mat.diffuse  = diff;}
	void SetMaterialSpec(tbColor spec) {m_mat.specular = spec;} 
	tbColor GetMaterialAmb ()	{return m_mat.ambient ;}
	tbColor GetMaterialDiff()	{return m_mat.diffuse ;}
	tbColor GetMaterialSpec()	{return m_mat.specular;}
	
	//attentuation festsetzen
	void SetAttentuation (float kc = 1.0f, float kl = 0.0f, float kq = 0.0f)	{m_fkc = kc; m_fkl = kl; m_fkq = kq;}
	void SetAttentuationC(float kc = 1.0f)	{m_fkc = kc;}
	void SetAttentuationL(float kl = 0.0f)	{m_fkl = kl;}
	void SetAttentuationQ(float kq = 0.0f)	{m_fkq = kq;}
	float GetAttentuationC() {return m_fkc;}
	float GetAttentuationL() {return m_fkl;}
	float GetAttentuationQ() {return m_fkq;}
	
	//spot festsetzen
	void SetSpot(tbVector3 vDir = tbVector3(0.0f,0.0f,-1.0f), float fEx = 0.0f, float fCut = 180.0f)	{m_vSpotDirection = vDir; m_fSpotExponent = fEx; m_fSpotCutoff = fCut;}
	void SetSpotDirection	(tbVector3 vDir = tbVector3(0.0f,0.0f,-1.0f))		{m_vSpotDirection = vDir;}
	void SetSpotExponent	(float fEx  = 0.0f)									{m_fSpotExponent  = fEx ;}
	void SetSpotCutoff		(float fCut = 180.0f)								{m_fSpotCutoff	  = fCut;}
	tbVector3 	GetSpotDirection ()	{return m_vSpotDirection;	}
	float 		GetSpotExponent	 ()	{return m_fSpotExponent;	}
	float 		GetSpotCutoff	 ()	{return m_fSpotCutoff;		}
	
	//fuer objekte:
	//static float DistanceMin(const tbVector3 &v); //die entfernung zum naechsten licht
	
private:
	glcSphere m_Sphere;					// kugel fuers licht
	static int  c_iCounter; 			// anzahl an lichtern
	static bool c_bLights[8]; 			//aktivierte lichter
	//static tbVector3 c_vLightCenter[8]; //statische lichtpositionen
	//static bool c_bPositional[8];		//statisch , licht positional?
	static int  c_iNumUses[8];			//number of light uses(0->disable, >0 enabled)
	int m_iLight; 						// stelle des eigenen lichts
	bool m_bPositional;					//true: positional, false: directional
	bool m_bActive;						//aktiviert?
	GLenum m_eLight;					//gl licht (z.B.: GL_LIGHT0)
	S_Material_ads m_mat;				//lichteigenschaften
	
	//spot
	tbVector3 m_vSpotDirection;				//richtung des spots
	float m_fSpotExponent, m_fSpotCutoff;	//exponent, winkelausschnitt
	
	//attentuation
	float m_fkc, m_fkl, m_fkq; // constant, linear, quadratic
};

#endif /*GLCLIGHT_H_*/
