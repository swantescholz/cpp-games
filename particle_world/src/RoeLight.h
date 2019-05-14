#ifndef ROELIGHT_H_
#define ROELIGHT_H_

#define ROE_MAXLIGHTS 8

#include "RoeColor.h"
#include "RoeMatrix.h"
#include <GL/gl.h>

namespace Roe {

	class Light
	{
	private:
		struct S_Material {
			Color amb;
			Color diff;
			Color spec;
		};
	public:
		Light(Vector3 vPos, bool bActive = true, bool bPos = true,
			  Color c1 = Color(0.0f,0.0f,0.0f,1.0f), Color c2 = Color(1.0f,1.0f,1.0f,1.0f), Color c3 = Color(1.0f,1.0f,1.0f,1.0f),
			  float kc = 1.0f, float kl = 0.0f, float kq = 0.0f, Vector3 vDir = Vector3(0.0f,0.0f,1.0f), float fEx = 0.0f, float fCut = 180.0f);
		
		Light(const Light &rhs);
		~Light();
		
		void transform(const Matrix &m);     //transforms the position vector and the spot direction
		void light();                        //the light shines(before objects!, after camera!)
		Vector3 getCenter() const {return m_vPos;}
		
		//activate / deactivate
		void enable   ()       {glEnable (m_eLight); m_bActive = true;}
		void disable  ()       {glDisable(m_eLight); m_bActive = false;}
		bool isEnabled() const {return glIsEnabled(m_eLight);}
		
		//global lightmodel
		static void lightModel   (const Color &col = Color(0.2f,0.2f,0.2f,1.0f), bool local_viewer = false, bool two_sides = false);
		static void lightModelAmb(const Color &col = Color(0.2f,0.2f,0.2f,1.0f)) {glLightModelfv(GL_LIGHT_MODEL_AMBIENT,col.rg);} //ambient world color
		static void lightModelLoc(bool b = false) {glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, b);}   //local viewer?
		static void lightModelTwo(bool b = false) {glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, b);}       //two-sided lighting?
		
		void setPositional(bool b)   {m_bPositional = b;}  //positional light or infinite distance=
		void setActive(bool b)       {m_bActive = b;}
		bool getPositional  () const {return m_bPositional;}
		bool getActive      () const {return m_bActive;}
		
		
		//material
		void setMaterial(Color amb, Color diff, Color spec) {m_mat.amb = amb; m_mat.diff = diff; m_mat.spec = spec;}
		void setMaterialAmb (Color amb)  {m_mat.amb  = amb ;}
		void setMaterialDiff(Color diff) {m_mat.diff = diff;}
		void setMaterialSpec(Color spec) {m_mat.spec = spec;} 
		Color getMaterialAmb () {return m_mat.amb ;}
		Color getMaterialDiff() {return m_mat.diff;}
		Color getMaterialSpec() {return m_mat.spec;}
		
		//attentuation
		void  setAttentuation (float kc = 1.0f, float kl = 0.0f, float kq = 0.0f) {m_fkc = kc; m_fkl = kl; m_fkq = kq;}
		void  setAttentuationC(float kc = 1.0f) {m_fkc = kc;}
		void  setAttentuationL(float kl = 0.0f) {m_fkl = kl;}
		void  setAttentuationQ(float kq = 0.0f) {m_fkq = kq;}
		float getAttentuationC() {return m_fkc;}
		float getAttentuationL() {return m_fkl;}
		float getAttentuationQ() {return m_fkq;}
		
		//spot
		void setSpot(Vector3 vDir = Vector3(0.0f,0.0f,-1.0f), float fEx = 0.0f, float fCut = 180.0f) {m_vSpotDir = vDir; m_fSpotExponent = fEx; m_fSpotCutoff = fCut;}
		void setSpotDirection   (Vector3 vDir = Vector3(0.0f,0.0f,-1.0f))      {m_vSpotDir       = vDir;}
		void setSpotExponent    (float fEx  = 0.0f)                            {m_fSpotExponent  = fEx ;}
		void setSpotCutoff      (float fCut = 180.0f)                          {m_fSpotCutoff    = fCut;}
		Vector3 getSpotDirection () {return m_vSpotDir;}
		float   getSpotExponent  () {return m_fSpotExponent;}
		float   getSpotCutoff    () {return m_fSpotCutoff;}
		
		
		static const int s_ciMaxLights; // = 8
	private:
		static int  s_iCounter;    //number of lights
		static bool s_bLights[8];  //lights activated?
		static int  s_iNumUses[8]; //number of light uses(0->disable, >0 enabled)
		
		Vector3 m_vPos;
		int m_iLight;              //light index
		bool m_bPositional;        //true: positional, false: directional
		bool m_bActive;            //active?
		GLenum m_eLight;           //gl licht (e.g.: GL_LIGHT0)
		S_Material m_mat;      //material
		
		//spot
		Vector3 m_vSpotDir;                     //direction of the spotlight
		float m_fSpotExponent, m_fSpotCutoff;   //exponent, alngle part
		
		//attentuation
		float m_fkc, m_fkl, m_fkq; // constant, linear, quadratic
	};
	
} // namespace Roe

#endif /*GLCLIGHT_H_*/
