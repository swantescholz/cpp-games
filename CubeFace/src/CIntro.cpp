#include "CIntro.h"

CIntro::CIntro(float fAng) : m_Music("intro", 1.0f), m_fRotAngle(fAng)
{
	m_ttfSans.Load("FreeSans", 60);
	m_Wood.Load("wood");
	m_Music.Play(-1);
	VCAM1 = tbVector3(0.0f, 80.0f, 200.0f);
	
	glClearColor(0,0,0,0);
}

CIntro::~CIntro()
{
	
}

void CIntro::Update() {
	if (glcKeyWasDown(SDLK_RETURN) || glcKeyWasDown(SDLK_SPACE) || glcKeyWasDown(SDLK_ESCAPE) ||
		g_pGL->MouseWasDown(SDL_BUTTON_LEFT) || g_pGL->MouseWasDown(SDL_BUTTON_RIGHT) || g_pGL->MouseQuit()) {
		g_pGS->m_GameState = GS_MAIN_MENU;
	}
	VCAM1 = tbVector3TransformCoords(VCAM1, tbMatrixRotationY(TB_DEG_TO_RAD(m_fRotAngle * ELAPSED) ) );
}

void CIntro::Render() {
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// We don't want to modify the projection matrix.
	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
	
	//cameramatrix festsetzen (zuallererst!)
	g_pGL->LookAt();
	
	
	//cube
	m_Wood.Bind();
	tbVector3 va[8];
	va[0] = tbVector3(-50,50,50);
	va[1] = tbVector3( 50,50,50);
	va[2] = tbVector3( 50,-50,50);
	va[3] = tbVector3( -50,-50,50);
	va[4] = tbVector3( -50,50,-50);
	va[5] = tbVector3( 50,50,-50);
	va[6] = tbVector3( 50,-50,-50);
	va[7] = tbVector3( -50,-50,-50);
	
	glBegin(GL_QUADS);
		glNormal3i(0,0,1);
		glTexCoord2i(0,0); glVertex3fv(va[3].c);
		glTexCoord2i(1,0); glVertex3fv(va[2].c);
		glTexCoord2i(1,1); glVertex3fv(va[1].c);
		glTexCoord2i(0,1); glVertex3fv(va[0].c);
		
		glNormal3i(1,0,0);
		glTexCoord2i(0,0); glVertex3fv(va[2].c);
		glTexCoord2i(1,0); glVertex3fv(va[6].c);
		glTexCoord2i(1,1); glVertex3fv(va[5].c);
		glTexCoord2i(0,1); glVertex3fv(va[1].c);
		
		glNormal3i(0,0,-1);
		glTexCoord2i(0,0); glVertex3fv(va[6].c);
		glTexCoord2i(1,0); glVertex3fv(va[7].c);
		glTexCoord2i(1,1); glVertex3fv(va[4].c);
		glTexCoord2i(0,1); glVertex3fv(va[5].c);
		
		glNormal3i(-10,0,0);
		glTexCoord2i(0,0); glVertex3fv(va[7].c);
		glTexCoord2i(1,0); glVertex3fv(va[3].c);
		glTexCoord2i(1,1); glVertex3fv(va[0].c);
		glTexCoord2i(0,1); glVertex3fv(va[4].c);
		
		glNormal3i(0,1,0);
		glTexCoord2i(0,0); glVertex3fv(va[0].c);
		glTexCoord2i(1,0); glVertex3fv(va[1].c);
		glTexCoord2i(1,1); glVertex3fv(va[5].c);
		glTexCoord2i(0,1); glVertex3fv(va[4].c);
		
		glNormal3i(0,-1,0);
		glTexCoord2i(0,0); glVertex3fv(va[7].c);
		glTexCoord2i(1,0); glVertex3fv(va[6].c);
		glTexCoord2i(1,1); glVertex3fv(va[2].c);
		glTexCoord2i(0,1); glVertex3fv(va[3].c);
	glEnd();
	
	
	m_ttfSans.RenderText("Cube", tbColor(0.0f,0.0f,1.0f,1.0f), va[3], va[2], va[1], va[0], false, 0.1f);
	m_ttfSans.RenderText("Face", tbColor(0.0f,0.0f,1.0f,1.0f), va[2], va[6], va[5], va[1], false, 0.1f);
	m_ttfSans.RenderText("Cube", tbColor(0.0f,0.0f,1.0f,1.0f), va[6], va[7], va[4], va[5], false, 0.1f);
	m_ttfSans.RenderText("Face", tbColor(0.0f,0.0f,1.0f,1.0f), va[7], va[3], va[0], va[4], false, 0.1f);
	
	m_ttfSans.RenderText("Press Return", tbColor(1.0f,1.0f,0.0f,1.0f), tbVector3(-80.0f, -50.0f,100.0f), tbVector3( 80.0f, -50.0f, 100.0f),
						 											   tbVector3( 80.0f, -50.0f, 80.0f), tbVector3(-80.0f, -50.0f, 80.0f));
						 											   
	glcFlip();
}
