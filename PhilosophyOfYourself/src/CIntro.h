#ifndef CINTRO_H_
#define CINTRO_H_

#include "../GLC/GLC.h"
#include "CGameAbstract.h"
#include "CGameState.h"

//possibilities for level-load file
enum EIntroLevelLine {
	ILL_BG = 0,
	ILL_MUSIC,
	ILL_CAM1,
	ILL_CAM2,
	ILL_BOX,
	ILL_BOX_ROT,
	ILL_BOX_TEX,
	ILL_BOX_TEXT,
	ILL_NUM_PARTICLES,
	ILL_PARTICLE_EFFECT,
	ILL_CAM_MOVEMENT,
	ILL_SHOW_FPS
};

//the three possible particle effects, or random
enum EIntroEffectType {
	IET_RANDOM = 0,
	IET_VOLCANO,
	IET_RAIN_CLOUD,
	IET_RAINBOW
};

//the first game-state, a nice intro with particle effect
class CIntro : public CGameAbstract
{
public:
	CIntro();
	~CIntro();
	
	void Update();
	void Render();
	
private:
	void StartParticle(glcParticle &p); //starts the single particle p
	void UpdateParticle(glcParticle &p);//updates a single particle p
	void RenderParticles();				//renders ALL particles
	void LoadLine(string psLine);		//loads a line from res/lvl/intro.lvl
	
	glcFont 	m_Font;				//the used font
	glcTexture 	m_BoxTex;			//the texture on the box
	glcTexture 	m_ParticleTex;		//the texture for the particles
	glcLight   *m_pLight1;			//a pointer on a light
	glcMusic 	m_Music;			//music
	
	tbVector3 	m_va[8];				//8 vectors of box
	double 		m_cdRot;				//rotation of cuboid
	string 		m_asBoxText[4];			//the four texts on the box
	tbColor 	m_aBoxTextCol[4];		//the colours of the four texts
	int 		m_iCamMovement;			//1->centered, 0->none
	int 		m_iShowFPS;				//0->no, 1->yess
	
	int 				m_ciNumParticles;	//consider it const!
	EIntroEffectType 	m_cIET;				//effect type
	glcParticle 	   *m_aParticles;		//the particles
	
	static int c_iNumInstances; //number of me
};

#endif /*CINTRO_H_*/
