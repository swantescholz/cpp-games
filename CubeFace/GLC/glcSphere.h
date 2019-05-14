#ifndef GLCSPHERE_H_
#define GLCSPHERE_H_

#include "glcSDL_GL.h"

class glcSphere
{
public:
	glcSphere();
	glcSphere(const tbVector3 &v, float fScale = 1.0f);
	glcSphere(const tbMatrix &m);
	~glcSphere();
	
	void Transform(const tbMatrix &m);
	void Render();
	
	tbVector3 GetCenter() {return tbVector3TransformCoords(nmath::vZero, m_mMatrix);}
	
	static void Init(int ndiv = 3);
	static void Normalize(GLfloat *a);
	static void Drawtri1(GLfloat *a, GLfloat *b, GLfloat *c, int div);	//mit normalenvektoren(lighting)
	static void Drawtri2(GLfloat *a, GLfloat *b, GLfloat *c, int div);	//ohne normalenvektoren(lighting)
	
private:
	static int c_iDispList; // display list index
	tbMatrix m_mMatrix;	//transfomationsmatrix
};

#endif /*GLCSPHERE_H_*/
