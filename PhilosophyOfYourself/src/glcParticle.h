#ifndef GLCPARTICLE_H_
#define GLCPARTICLE_H_

#include "glcSDL_GL.h"
#include "glcKeyFrameInterpolator.h"
#include "glcTexture.h"
#include "glcMaterial.h"

class glcParticle
{
public:
	glcParticle(bool pbLighted = true);
	glcParticle(const glcParticle& rhs);
	~glcParticle();
	
	glcParticle& operator = (const glcParticle& rhs);
	
	bool  GetLighted() const			{return m_bLighted;	}
	void  SetLighted(bool b) 			{m_bLighted = b;	}
	bool  GetActive () const			{return m_bActive;	}
	void  SetActive (bool b) 			{m_bActive = b;		}
	float GetFade   () const			{return m_fFade;	}
	void  SetFade   (float f)			{m_fFade = f;		}
	float GetLife	() const			{return m_fLife;	}
	void  SetLife	(float f)			{m_fLife = f;		}
	tbVector2 GetWH ()	const			{return m_vWH; }
	tbVector3 GetPos()	const			{return m_vPos;}
	tbVector3 GetVel()	const			{return m_vVel;}
	tbVector3 GetAcc()	const			{return m_vAcc;}
	void SetWH (tbVector2 pv)			{m_vWH  = pv;}
	void SetPos(tbVector3 pv)			{m_vPos = pv;}
	void SetVel(tbVector3 pv)			{m_vVel = pv;}
	void SetAcc(tbVector3 pv)			{m_vAcc = pv;}
	
	void AddKeyFrameColor   (tbColor     pCol, float fTime);
	void AddKeyFrameMaterial(glcMaterial pMat, float fTime);
	
	void Start(tbVector3 pvPos, tbVector3 pvVel, tbVector3 pvAcc);	//(re-)activate particle
	void Update();		//update the particle (moving, life, material/color)
	void Render();		//normal rendering
	void UpdateFast();	//fast updating, only moving and life checking
	void RenderFast();	//fast rendering(no materials/colors extra, no checking, with texcoord)
	
	static void SetElapsed(float pf) {c_fElapsed = pf;}
private:
	bool m_bActive;		//am I alive?
	bool m_bLighted;	//lighted?
	float m_fLife;		//life
	float m_fFade;		//losing life
	tbColor   m_Col;	//the color - if not lighted
	glcMaterial m_Mat;	//the material - if ligthed
	glcKeyFrameInterpolator<tbColor>     *m_kfiCol; //never use non-pointers when you can sue pointers
	glcKeyFrameInterpolator<glcMaterial> *m_kfiMat; //using her pointers was VERY important(sizeof(me) == constant for qsort)
	tbVector3 m_vPos; 	//current position
	tbVector3 m_vVel; 	//actual speed
	tbVector3 m_vAcc; 	//gravity or wind etc.
	tbVector2 m_vWH;	//width and heigth
	
	static float c_fElapsed; //static for all particles->faster
};

#endif /*GLCPARTICLE_H_*/
