#include "glcSphere.h"

int glcSphere::c_iDispList = -1;

glcSphere::glcSphere()						: m_mMatrix(tbMatrixIdentity()) 	{}
glcSphere::glcSphere(const tbVector3 &v, float fScale) 	: m_mMatrix(tbMatrixScaling(tbVector3(fScale,fScale,fScale)) * tbMatrixTranslation(v)) {}
glcSphere::glcSphere(const tbMatrix &m)		: m_mMatrix(m)						{}

glcSphere::~glcSphere()
{
}

void glcSphere::Transform(const tbMatrix &m) {
	m_mMatrix *= m;
}

void glcSphere::Render() {
	if (m_mMatrix == tbMatrixScaling(tbVector3(0.0f,0.0f,0.0f)) * tbMatrixTranslation(tbVector3TransformCoords(tbVector3(0,0,0),m_mMatrix) ) ) {
		return; //zero scale, so no rendering
	}
	bool bTexWas = glIsEnabled(GL_TEXTURE_2D);
	if (bTexWas) {glDisable(GL_TEXTURE_2D);}
	glPushMatrix();
		g_pGL->MultMatrix(m_mMatrix);
		glCallList(c_iDispList);
	glPopMatrix();
	if (bTexWas) {glEnable(GL_TEXTURE_2D);}
}

//static methoden
void glcSphere::Init(int ndiv) {
	if (c_iDispList != -1) {
		throw glcException("doppelte CSphere-initialisierung");
	}
	
	c_iDispList = glGenLists(1);
	if (c_iDispList == 0) {
		throw glcException("sphere-display-list konnte nicht generiert werden");
	}
	
	const double dX = 0.525731112119133606;
	const double dZ = 0.850650808352039932;
	
	static GLfloat vdata[12][3] = {    
    	{-dX, 0.0, dZ}, {dX, 0.0, dZ}, {-dX, 0.0, -dZ}, {dX, 0.0, -dZ},    
    	{0.0, dZ, dX}, {0.0, dZ, -dX}, {0.0, -dZ, dX}, {0.0, -dZ, -dX},    
    	{dZ, dX, 0.0}, {-dZ, dX, 0.0}, {dZ, -dX, 0.0}, {-dZ, -dX, 0.0} };
	static GLuint tindices[20][3] = { 
	    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
	    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
	    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
	    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
	
	
	if (glIsEnabled(GL_LIGHTING)) {
		glNewList(c_iDispList, GL_COMPILE);
			glBegin(GL_TRIANGLES);
				for (int i=0;i<20;i++)
        			Drawtri1(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv);
			glEnd();
		glEndList();
	} else {
		glNewList(c_iDispList, GL_COMPILE);
			glBegin(GL_TRIANGLES);
				for (int i=0;i<20;i++)
        			Drawtri2(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv);
			glEnd();
		glEndList();
	}
}

void glcSphere::Normalize(GLfloat *a) {
    GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]/=d; a[1]/=d; a[2]/=d;
}
void glcSphere::Drawtri1(GLfloat *a, GLfloat *b, GLfloat *c, int div) {
	if (div<=0) {
		glNormal3fv(a); glVertex3f(a[2], a[1], a[0]); //rueckwaerts wegen cw - culling
		glNormal3fv(b); glVertex3f(b[2], b[1], b[0]);
		glNormal3fv(c); glVertex3f(c[2], c[1], c[0]);
	} else {
		GLfloat ab[3], ac[3], bc[3];
		for (int i=0;i<3;i++) {
			ab[i]=(a[i]+b[i])/2;
			ac[i]=(a[i]+c[i])/2;
			bc[i]=(b[i]+c[i])/2;
		}
		Normalize(ab); Normalize(ac); Normalize(bc);
		Drawtri1(a, ab, ac, div-1);
		Drawtri1(b, bc, ab, div-1);
		Drawtri1(c, ac, bc, div-1);
		Drawtri1(ab, bc, ac, div-1);  //<--Comment this line and sphere looks really cool!
	}
}
void glcSphere::Drawtri2(GLfloat *a, GLfloat *b, GLfloat *c, int div) {
	if (div<=0) {
		glVertex3f(a[2], a[1], a[0]);
		glVertex3f(b[2], b[1], b[0]);
		glVertex3f(c[2], c[1], c[0]);
	} else {
		GLfloat ab[3], ac[3], bc[3];
		for (int i=0;i<3;i++) {
			ab[i]=(a[i]+b[i])/2;
			ac[i]=(a[i]+c[i])/2;
			bc[i]=(b[i]+c[i])/2;
		}
		Normalize(ab); Normalize(ac); Normalize(bc);
		Drawtri2(a, ab, ac, div-1);
		Drawtri2(b, bc, ab, div-1);
		Drawtri2(c, ac, bc, div-1);
		Drawtri2(ab, bc, ac, div-1);  //<--Comment this line and sphere looks really cool!
	}
}

