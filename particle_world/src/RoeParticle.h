#ifndef ROEPARTICLE_H_
#define ROEPARTICLE_H_

#include "RoeKeyframeInterpolator.h"
#include "RoeMaterial.h"
#include "RoeColor.h"

namespace Roe {

	class Particle
	{
	public:
		Particle(bool pbLighted = true);
		Particle(const Particle& rhs);
		~Particle();
		
		Particle& operator = (const Particle& rhs);
		
		bool  getLighted() const  {return m_bLighted;}
		bool  getActive () const  {return m_bActive;}
		float getFade   () const  {return m_fFade;}
		float getLife   () const  {return m_fLife;}
		void  setLighted(bool b)  {m_bLighted = b;}
		void  setActive (bool b)  {m_bActive = b;}
		void  setFade   (float f) {m_fFade = f;}
		void  setLife   (float f) {m_fLife = f;}
		Vector2 getWH () const  {return m_vWH; }
		Vector3 getPos() const  {return m_vPos;}
		Vector3 getVel() const  {return m_vVel;}
		Vector3 getAcc() const  {return m_vAcc;}
		void setWH (Vector2 pv) {m_vWH  = pv;}
		void setPos(Vector3 pv) {m_vPos = pv;}
		void setVel(Vector3 pv) {m_vVel = pv;}
		void setAcc(Vector3 pv) {m_vAcc = pv;}
		
		void addKeyframeColor   (Color    pCol, float fTime);
		void addKeyframeMaterial(Material pMat, float fTime);
		
		void start(Vector3 pvPos, Vector3 pvVel = Vector3::ZERO, Vector3 pvAcc = Vector3::ZERO); //(re-)activate particle
		void update();      //update the particle (moving, life, material/color)
		void render();      //normal rendering
		void updateFast();  //fast updating, only moving and life checking
		void renderFast();  //fast rendering(no materials/colors extra, no checking, with texcoord)
		
		static void setElapsed(float pf) {s_fElapsed = pf;}
		static void sortZ(Particle *p , int num, Vector3 vCamPos); //for normal arrays
		static void sortZ(Particle **p, int num, Vector3 vCamPos); //for arrays of pointers
		static int compareFunction1(const Particle& p1, const Particle& p2) {
			return p1.getPos().squaredDistance(s_vCamPos) <
			       p2.getPos().squaredDistance(s_vCamPos);}
		static int compareFunction2(const Particle* p1, const Particle* p2) {
			return p1->getPos().squaredDistance(s_vCamPos) <
			       p2->getPos().squaredDistance(s_vCamPos);}
		
	private:
		bool m_bActive;     //am I alive?
		bool m_bLighted;    //lighted?
		float m_fLife;      //life
		float m_fFade;      //losing life
		Color   m_Col;    //the color - if not lighted
		Material m_Mat;  //the material - if ligthed
		KeyframeInterpolatorLinear<Color>    *m_kfiCol; //never use non-pointers when you can sue pointers
		KeyframeInterpolatorLinear<Material> *m_kfiMat; //using her pointers was VERY important(sizeof(me) == constant for qsort)
		Vector3 m_vPos;   //current position
		Vector3 m_vVel;   //actual speed
		Vector3 m_vAcc;   //gravity or wind etc.
		Vector2 m_vWH;    //width and heigth
		
		static float s_fElapsed; //static for all particles->faster
		static Vector3 s_vCamPos;
	};
	
} // namespace Roe

#endif /*ROEPARTICLE_H_*/
