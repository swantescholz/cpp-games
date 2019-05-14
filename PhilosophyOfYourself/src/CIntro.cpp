#include "CIntro.h"

int CIntro::c_iNumInstances = 0;

CIntro::CIntro()
{
	m_Font.Load("FreeSans",64);
	m_pLight1 = new glcLight(glcSphere(tbVector3(0.0,0.0,50.0), 0.2), true, true);
	
	m_va[0] = tbVector3(-0.5,0.5,0.5);
	m_va[1] = tbVector3( 0.5,0.5,0.5);
	m_va[2] = tbVector3( 0.5,-0.5,0.5);
	m_va[3] = tbVector3( -0.5,-0.5,0.5);
	m_va[4] = tbVector3( -0.5,0.5,-0.5);
	m_va[5] = tbVector3( 0.5,0.5,-0.5);
	m_va[6] = tbVector3( 0.5,-0.5,-0.5);
	m_va[7] = tbVector3( -0.5,-0.5,-0.5);
	
	//loading the level
	char ch[500];
	string sLine;
	ifstream dat_in;
	dat_in.open("res/lvl/intro.lvl", ios_base::in);
	if (!dat_in) {throw glcException("res/lvl/intro.lvl is missing");}
	while (!dat_in.eof()) {
		dat_in.getline (ch,498);
		sLine = ch;
		if (sLine.find("end")!=string::npos || sLine.find("End")!=string::npos || sLine.find("END")!=string::npos) {  
			break; //the end of the file
		}
		if (sLine.length() > 1) {
			if (sLine[0] != '/') {
				LoadLine(sLine);
			}
		}
	}
	dat_in.close();
	
	//particle effect
	m_aParticles = new glcParticle[m_ciNumParticles];
	glcMaterial mat[6];
	if (m_cIET == IET_RANDOM) {m_cIET = static_cast<EIntroEffectType>(g_pTimer->Random(IET_VOLCANO,IET_RAINBOW));}
	switch(m_cIET) {
		case IET_VOLCANO:
			m_ParticleTex.Load		("particles/fire");
			mat[0] = glcMaterial(tbColor(1.0,1.0,1.0,1.0),tbColor(1.0,1.0,1.0,1.0),tbColor(1.0,1.0,1.0,1.0),50);
			mat[1] = glcMaterial(tbColor(1.0,0.4,0.0,1.0),tbColor(1.0,0.4,0.0,1.0),tbColor(1.0,0.4,0.0,1.0),40);
			mat[2] = glcMaterial(tbColor(0.5,0.4,0.2,0.0),tbColor(0.5,0.4,0.2,0.0),tbColor(0.5,0.4,0.2,0.0), 0);
			for (int i = 0; i < m_ciNumParticles; i++) {
				m_aParticles[i].AddKeyFrameMaterial(mat[0],0.0f);
				m_aParticles[i].AddKeyFrameMaterial(mat[1],0.5f);
				m_aParticles[i].AddKeyFrameMaterial(mat[2],1.0f);
			}
			break;
		case IET_RAIN_CLOUD:
			m_ParticleTex.Load		("particles/snow");
			mat[0] = glcMaterial(tbColor(0.1,0.1,1.0,1.0),tbColor(0.1,0.1,1.0,1.0),tbColor(0.1,0.1,1.0,1.0),50);
			mat[1] = glcMaterial(tbColor(0.3,0.3,1.0,0.8),tbColor(0.3,0.3,1.0,0.8),tbColor(0.3,0.3,1.0,0.8),40);
			mat[2] = glcMaterial(tbColor(0.1,0.1,0.1,0.0),tbColor(0.1,0.1,0.1,0.0),tbColor(0.1,0.1,0.1,0.0), 0);
			for (int i = 0; i < m_ciNumParticles; i++) {
				m_aParticles[i].AddKeyFrameMaterial(mat[0],0.0f);
				m_aParticles[i].AddKeyFrameMaterial(mat[1],0.5f);
				m_aParticles[i].AddKeyFrameMaterial(mat[2],1.0f);
			}
			break;
		case IET_RAINBOW:
			m_ParticleTex.Load		("particles/circle1111");
			mat[0] = glcMaterial(tbColor(1.0,0.0,0.0,0.0),tbColor(1.0,0.0,0.0,0.0),tbColor(1.0,0.0,0.0,0.0), 0);
			mat[1] = glcMaterial(tbColor(0.0,0.0,1.0,0.7),tbColor(0.0,0.0,1.0,0.7),tbColor(0.0,0.0,1.0,0.7),30);
			mat[2] = glcMaterial(tbColor(0.0,1.0,0.0,0.9),tbColor(0.0,1.0,0.0,0.9),tbColor(0.0,1.0,0.0,0.9),50);
			mat[3] = glcMaterial(tbColor(1.0,1.0,0.0,0.9),tbColor(1.0,1.0,0.0,0.9),tbColor(1.0,1.0,0.0,0.9),50);
			mat[4] = glcMaterial(tbColor(1.0,0.0,0.0,0.7),tbColor(1.0,0.0,0.0,0.7),tbColor(1.0,0.0,0.0,0.7),30);
			mat[5] = glcMaterial(tbColor(0.0,0.0,1.0,0.0),tbColor(0.0,0.0,1.0,0.0),tbColor(0.0,0.0,1.0,0.0), 0);
			for (int i = 0; i < m_ciNumParticles; i++) {
				m_aParticles[i].AddKeyFrameMaterial(mat[0],0.0f);
				m_aParticles[i].AddKeyFrameMaterial(mat[1],0.2f);
				m_aParticles[i].AddKeyFrameMaterial(mat[2],0.4f);
				m_aParticles[i].AddKeyFrameMaterial(mat[3],0.6f);
				m_aParticles[i].AddKeyFrameMaterial(mat[4],0.8f);
				m_aParticles[i].AddKeyFrameMaterial(mat[5],1.0f);
			}
			break;
		
		default: throw glcException(string("Intro effect Type is not ok: ") + m_cIET); break;
	};
	
	for (int i = 0; i< m_ciNumParticles; i++) {
		m_aParticles[i].SetLighted(true);
		StartParticle (m_aParticles[i]);
	}
	
	c_iNumInstances++; //counting the instances
}

CIntro::~CIntro()
{
	if (m_Music.IsLoaded() == true && c_iNumInstances == 1) { //deleting music, because last instance
		glcMusic::Halt(); //stop playing first
		glcMusic::DeleteMusic(m_Music.GetName());
	}
	GLC_SAFE_DELETE(m_pLight1);
	GLC_SAFE_DELETE_ARRAY(m_aParticles);
	
	c_iNumInstances--; //counting the instances
}

void CIntro::StartParticle(glcParticle &p) {
	tbVector3 vPos = tbVector3(0,0,0);
	tbVector3 vVel = tbVector3(0,0,0);
	tbVector2 vTmp = tbVector2(0,0);
	tbVector3 vAcc = tbVector3(0,-10,0); //gravitation
	tbVector2 vWH  = tbVector2(1,1);
	float fLife    = 1.0f;
	float fFade    = 0.5f;
	
	switch (m_cIET) {
		case IET_VOLCANO:
			vPos = (tbVector3Random() * g_pTimer->Random(0.0f,1.0f)) * 1.5f + tbVector3(0,10.0f,0);
			vTmp = (tbVector2Random() * g_pTimer->Random(0.0f,1.0f)) * 4.0f;
			vVel = tbVector3Normalize(tbVector3(vTmp.x,10.0,vTmp.y)) * 18.0f;
			vAcc = tbVector3(0.0f,-9.81f,0.0f) * g_pTimer->Random(0.8f,1.2f);
			fFade = 0.2f * g_pTimer->Random(0.8f,1.2f);
			vWH = tbVector2(0.5) * g_pTimer->Random(0.8f,1.2f);
			break;
		case IET_RAIN_CLOUD:
			vPos = (tbVector3Random() * g_pTimer->Random(0.0f,1.0f)) * tbVector3(25,0.1,20) + tbVector3(0,20.0f,10);
			vTmp = tbVector2Random() * g_pTimer->Random(0.0f,0.05f);
			vVel = tbVector3Normalize(tbVector3(vTmp.x,1.0,vTmp.y)) * 2.0f;
			vAcc = tbVector3(0.0f,-1.81f,0.0f) * g_pTimer->Random(0.8f,1.2f);
			fFade = 0.1f * g_pTimer->Random(0.5f,2.0f);
			vWH = tbVector2(0.5) * g_pTimer->Random(0.8f,1.2f);
			break;
		case IET_RAINBOW: {
			float fInnerRadius = 20.0f;
			float fWidth = 2.5f;
			float fSpeed = 0.4f;
			float fZWidth= 4.0f;
			float fYMove = 0.0f;
			float fZMove = -10.0f;
			vTmp  = tbVector2Random();
			vTmp *= tbVector2(1,(vTmp.y < 0.0f) ? (-1.0f) : (1.0f));
			vTmp *= (fInnerRadius + g_pTimer->Random(0.0f, fWidth));
			vPos = tbVector3(vTmp.x,vTmp.y,g_pTimer->Random(-fZWidth/2.0f,fZWidth/2.0f));
			vVel = tbVector3(tbVector2Normalize(vTmp) * fSpeed,0.0f);
			vPos += tbVector3(0,fYMove,fZMove);
			vAcc = tbVector3(0.0,0.0,0.0);
			fFade = tbVector3Length(vVel) / fWidth;
			fLife = 1-((tbVector2Length(vTmp)-fInnerRadius) / fWidth);
			vWH = tbVector2(0.8) * g_pTimer->Random(0.8f,1.2f);
		} break;
		
		default: throw glcException(string("Intro effect Type is not ok: ") + m_cIET); break;
	}	
	
	p.SetWH(vWH);
	p.SetFade(fFade);
	p.Start(vPos,vVel,vAcc);
	p.SetLife(fLife);
}

void CIntro::UpdateParticle(glcParticle &p) {
	switch (m_cIET) {
		case IET_VOLCANO:
			break;
		case IET_RAIN_CLOUD:
			if      (p.GetPos().y < 19.0f) p.SetAcc(tbVector3(p.GetAcc().x,-9.81,p.GetAcc().z));
			else if (p.GetPos().y < 10.0f) p.SetAcc(tbVector3(p.GetAcc().x,    0,p.GetAcc().z));
			break;
		case IET_RAINBOW:
			
			break;
		default: throw glcException(string("Intro effect Type is not ok: ") + m_cIET); break;
	}
	
	p.Update();
	if (p.GetActive() == false) {
		StartParticle(p);
	}
}

void CIntro::RenderParticles() {
	m_ParticleTex.Bind();
	glNormal3fv(tbVector3(0,0,1).c);
	//glBegin(GL_QUADS);
		for (int i= 0; i < m_ciNumParticles; i++) {
			m_aParticles[i].Render();
		}
	//glEnd();
}

void CIntro::Update() {
	if (g_pGL->KeyWasDown(SDLK_RETURN) || g_pGL->KeyWasDown(SDLK_SPACE) || g_pGL->KeyWasDown(SDLK_ESCAPE)) {
		g_pGS->m_GS = GS_MAIN_MENU;
	}
	for (int i= 0; i < 8; i++) {
		m_va[i] = tbVector3TransformCoords(m_va[i],tbMatrixRotationX(-TB_DEG_TO_RAD((m_cdRot * ELAPSED))));
	}
	
	
	//particles
	glcParticle::SetElapsed(g_pTimer->GetElapsed());
	for (int i= 0; i< m_ciNumParticles; i++) {
		UpdateParticle(m_aParticles[i]);
	}
	qsort(m_aParticles,m_ciNumParticles,sizeof(glcParticle),CompareFunction);
	
	
	if (m_iCamMovement == 1) {
		tbVector3 vTmp;
		tbMatrix mSca, mRot, mRot2;
		if (g_pGL->KeyIsDown(SDLK_DOWN)) 		mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(SCALINGPERSECCAM*ELAPSED),1+(SCALINGPERSECCAM*ELAPSED),1+(SCALINGPERSECCAM*ELAPSED))));
		else if (g_pGL->KeyIsDown(SDLK_UP)) 	mSca = tbMatrix(tbMatrixScaling(tbVector3(1+(-SCALINGPERSECCAM*ELAPSED),1+(-SCALINGPERSECCAM*ELAPSED),1+(-SCALINGPERSECCAM*ELAPSED))));
		if (g_pGL->KeyIsDown(SDLK_LEFT)) 				mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
		else if (g_pGL->KeyIsDown(SDLK_RIGHT))			mRot = tbMatrix(tbMatrixRotationY(TB_DEG_TO_RAD(-ROTATIONPERSECCAM*ELAPSED)));
		if (g_pGL->KeyIsDown(SDLK_LCTRL))				mRot2 = tbMatrix(tbMatrixRotationAxis(tbVector3Cross(VCAM1, tbVector3(0.0f,1.0f,0.0f)),-TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
		else if (g_pGL->KeyIsDown(SDLK_LALT))			mRot2 = tbMatrix(tbMatrixRotationAxis(tbVector3Cross(VCAM1, tbVector3(0.0f,1.0f,0.0f)), TB_DEG_TO_RAD(ROTATIONPERSECCAM*ELAPSED)));
		if (g_pGL->KeyIsDown(SDLK_LEFT) || g_pGL->KeyIsDown(SDLK_RIGHT)) 	{VCAM1 -= VCAM2; VCAM1 = tbVector3TransformCoords(VCAM1,mRot); VCAM1 += VCAM2;} 
		if (g_pGL->KeyIsDown(SDLK_DOWN) || g_pGL->KeyIsDown(SDLK_UP)) 		{VCAM1 -= VCAM2; VCAM1 = tbVector3TransformCoords(VCAM1,mSca); VCAM1 += VCAM2;}
		if (g_pGL->KeyIsDown(SDLK_LCTRL) || g_pGL->KeyIsDown(SDLK_LALT))		{VCAM1 -= VCAM2; vTmp = VCAM1; VCAM1 = tbVector3TransformCoords(VCAM1,mRot2); if (nmath::Sign(VCAM1.x) != nmath::Sign(vTmp.x) || nmath::Sign(VCAM1.z) != nmath::Sign(vTmp.z)) VCAM1 = vTmp; VCAM1 += VCAM2;}
		
		if (glcKeyIsDown(SDLK_d)) m_pLight1->Transform(tbMatrixTranslation(tbVector3( 1,0,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_a)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(-1,0,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_e)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0, 1,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_q)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,-1,0) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_w)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,0,-1) * 5.0 * g_pTimer->GetElapsed()));
		if (glcKeyIsDown(SDLK_s)) m_pLight1->Transform(tbMatrixTranslation(tbVector3(0,0, 1) * 5.0 * g_pTimer->GetElapsed()));
	} else {
		
	}
}

void CIntro::Render() {
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// We don't want to modify the projection matrix.
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
	
	//cameramatrix festsetzen (zuallererst!)
	g_pGL->LookAt();
	
	//light
	m_pLight1->Render();
	
	
	//cuboid
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tbColor(1,1,1,1).c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tbColor(1,1,1,1).c);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
	m_BoxTex.Bind();
	
	glBegin(GL_QUADS);
		glNormal3fv(tbVector3Normalize( tbVector3Cross(  (m_va[2] - m_va[3]),(m_va[1] - m_va[3]) ) ).c);
		glTexCoord2i(0,0); glVertex3fv(m_va[3].c);
		glTexCoord2i(1,0); glVertex3fv(m_va[2].c);
		glTexCoord2i(1,1); glVertex3fv(m_va[1].c);
		glTexCoord2i(0,1); glVertex3fv(m_va[0].c);
		
		glNormal3fv(tbVector3Normalize( tbVector3Cross(  (m_va[6] - m_va[2]),(m_va[5] - m_va[2]) ) ).c);
		glTexCoord2i(0,0); glVertex3fv(m_va[2].c);
		glTexCoord2i(1,0); glVertex3fv(m_va[6].c);
		glTexCoord2i(1,1); glVertex3fv(m_va[5].c);
		glTexCoord2i(0,1); glVertex3fv(m_va[1].c);
		
		glNormal3fv(tbVector3Normalize( tbVector3Cross(  (m_va[7] - m_va[6]),(m_va[4] - m_va[6]) ) ).c);
		glTexCoord2i(0,0); glVertex3fv(m_va[6].c);
		glTexCoord2i(1,0); glVertex3fv(m_va[7].c);
		glTexCoord2i(1,1); glVertex3fv(m_va[4].c);
		glTexCoord2i(0,1); glVertex3fv(m_va[5].c);
		
		glNormal3fv(tbVector3Normalize( tbVector3Cross(  (m_va[3] - m_va[7]),(m_va[0] - m_va[7]) ) ).c);
		glTexCoord2i(0,0); glVertex3fv(m_va[7].c);
		glTexCoord2i(1,0); glVertex3fv(m_va[3].c);
		glTexCoord2i(1,1); glVertex3fv(m_va[0].c);
		glTexCoord2i(0,1); glVertex3fv(m_va[4].c);
		
		glNormal3fv(tbVector3Normalize( tbVector3Cross(  (m_va[1] - m_va[0]),(m_va[5] - m_va[0]) ) ).c);
		glTexCoord2i(0,0); glVertex3fv(m_va[0].c);
		glTexCoord2i(1,0); glVertex3fv(m_va[1].c);
		glTexCoord2i(1,1); glVertex3fv(m_va[5].c);
		glTexCoord2i(0,1); glVertex3fv(m_va[4].c);
		
		glNormal3fv(tbVector3Normalize( tbVector3Cross(  (m_va[6] - m_va[7]),(m_va[2] - m_va[7]) ) ).c);
		glTexCoord2i(0,0); glVertex3fv(m_va[7].c);
		glTexCoord2i(1,0); glVertex3fv(m_va[6].c);
		glTexCoord2i(1,1); glVertex3fv(m_va[2].c);
		glTexCoord2i(0,1); glVertex3fv(m_va[3].c);
	glEnd();
	
	m_Font.Load("FreeSans",64);
	m_Font.RenderText(m_asBoxText[0], m_aBoxTextCol[0], m_va[3], m_va[2], m_va[1], m_va[0], false, 0.1f);
	m_Font.RenderText(m_asBoxText[1], m_aBoxTextCol[1], m_va[7], m_va[6], m_va[2], m_va[3], false, 0.1f);
	m_Font.RenderText(m_asBoxText[2], m_aBoxTextCol[2], m_va[4], m_va[5], m_va[6], m_va[7], false, 0.1f);
	m_Font.RenderText(m_asBoxText[3], m_aBoxTextCol[3], m_va[0], m_va[1], m_va[5], m_va[4], false, 0.1f);
	
	
	if (m_iShowFPS == 1) {
		m_Font.Load("FreeSans",16);
		glcFont::Begin();
			m_Font.RenderText(string("FPS: ") + g_pTimer->GetFPS(), tbColor(1,0,0,1), tbVector2(0,0));
		glcFont::End();
	}
	
	//particles
	RenderParticles();
	
	glcFlip();
}

void CIntro::LoadLine(string psLine) {
	float af[40];
	int ai[20];
	string as[20];
	switch (strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10)) {
		case ILL_BG: //setting clearcolor
			for (int i = 0; i < 4; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			glClearColor(af[0],af[1],af[2],af[3]);
			break;
		case ILL_MUSIC:
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(psLine);
			for (int i = 0; i < 1; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			if (m_Music.IsLoaded() == false) {
				if (glcMusic::IsMusicLoaded(as[0]) == false) glcMusic::LoadMusic(as[0],g_pGS->m_cfStdMusicVolume);
				m_Music.Load(as[0]);
				m_Music.SetVolume(af[0]); 
				m_Music.Play(-1);
			}
			break;
		case ILL_CAM1: //setting cam1
			for (int i = 0; i < 3; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			VCAM1 = tbVector3(af[0],af[1],af[2]);
			break;
		case ILL_CAM2: //setting cam2
			for (int i = 0; i < 3; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			VCAM2 = tbVector3(af[0],af[1],af[2]);
			break;
		case ILL_BOX: //setting box size
			for (int i = 0; i < 3; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			for (int i = 0; i < 8; i++) {m_va[i] *= tbVector3(af[0],af[1],af[2]);}
			break;
		case ILL_BOX_ROT: //setting cam2
			for (int i = 0; i < 1; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			m_cdRot = af[0];
			break;
		case ILL_BOX_TEX: //loading the box-texture
			for (int i = 0; i < 1; i++)  as[i] = poyGetFirstStringOfStringAndErase(psLine);
			glcTexture::LoadTexture(as[0]);
			m_BoxTex.Load(as[0]);
			break;
		case ILL_BOX_TEXT: //setting one box text
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_asBoxText[ ai[0] ] = poyGetFirstStringOfStringAndErase(psLine);
			for (int i = 0; i < 4; i++) {af[i] = strtod(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL);}
			m_aBoxTextCol[ ai[0] ] = tbColor(af[0],af[1],af[2],af[3]);
			break;
		case ILL_NUM_PARTICLES: //setting the number of particles
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_ciNumParticles = ai[0];
			break;
		case ILL_PARTICLE_EFFECT: //choose particle effect
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_cIET = static_cast<EIntroEffectType>(ai[0]);
			break;
		case ILL_CAM_MOVEMENT: //setting the number of particles
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_iCamMovement = ai[0];
			break;
		case ILL_SHOW_FPS: //setting the number of particles
			for (int i = 0; i < 1; i++) {ai[i] = strtoul(poyGetFirstNumberOfStringAndErase(psLine).data(),NULL,10);}
			m_iShowFPS = ai[0];
			break;
		default: break;
		
	}
}


	
	