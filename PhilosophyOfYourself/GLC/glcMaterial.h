#ifndef GLCMATERIAL_H_
#define GLCMATERIAL_H_

#include <iostream>
#include "tbColor.h"
using namespace std;

class glcMaterial
{
public:
	glcMaterial(tbColor pAmb  = tbColor(1,1,1,1),tbColor pDiff = tbColor(1,1,1,1),
				tbColor pSpec = tbColor(1,1,1,1),float pfShininess = 0.0f) : a(pAmb), b(pDiff), c(pSpec), s(pfShininess) 	{}
	glcMaterial(const glcMaterial& m) : a(m.a),b(m.b),c(m.c),s(m.s) 															{}
	~glcMaterial() {}
	
	tbColor a;	//ambient
	tbColor b;	//diffuse
	tbColor c;	//specular
	float s;	//shininess
	
	void Render() const {
		glMaterialfv(GL_FRONT, GL_AMBIENT , a.c);
		glMaterialfv(GL_FRONT, GL_DIFFUSE , b.c);
		glMaterialfv(GL_FRONT, GL_SPECULAR, c.c);
		glMaterialf (GL_FRONT, GL_SHININESS, s);
	}
	
	// Zuweisungsoperatoren
	glcMaterial& operator =  (const glcMaterial& m)	{a=m.a; b=m.b; c=m.c; s=m.s; return *this;}
	glcMaterial& operator += (const glcMaterial& m)	{a += m.a; b += m.b; c += m.c; s += m.s; return *this;}
	glcMaterial& operator -= (const glcMaterial& m)	{a -= m.a; b -= m.b; c -= m.c; s -= m.s; return *this;}
	glcMaterial& operator *= (const glcMaterial& m)	{a *= m.a; b *= m.b; c *= m.c; s *= m.s; return *this;}
	glcMaterial& operator *= (const float f)		{a *= f;   b *= f;   c *= f;   s *= f;   return *this;}
	glcMaterial& operator /= (const glcMaterial& m)	{a /= m.a; b /= m.b; c /= m.c; s /= m.s; return *this;}
	glcMaterial& operator /= (const float f)		{a /= f;   b /= f;   c /= f;   s /= f;   return *this;}
		
};

// Arithmetische Operatoren
inline glcMaterial operator + (const glcMaterial& ma, const glcMaterial& mb)	{return glcMaterial(ma.a + mb.a, ma.b + mb.b, ma.c + mb.c, ma.s + mb.s);}
inline glcMaterial operator - (const glcMaterial& ma, const glcMaterial& mb)	{return glcMaterial(ma.a - mb.a, ma.b - mb.b, ma.c - mb.c, ma.s - mb.s);}
inline glcMaterial operator - (const glcMaterial& mc)							{return glcMaterial(-mc.a, -mc.b, -mc.c, mc.s);}
inline glcMaterial operator * (const glcMaterial& ma, const glcMaterial& mb)	{return glcMaterial(ma.a * mb.a, ma.b * mb.b, ma.c * mb.c, ma.s * mb.s);}
inline glcMaterial operator * (const glcMaterial& mc, const float f)			{return glcMaterial(mc.a * f, mc.b * f, mc.c * f, mc.s * f);}
inline glcMaterial operator * (const float f, const glcMaterial& mc)			{return glcMaterial(mc.a * f, mc.b * f, mc.c * f, mc.s * f);}
inline glcMaterial operator / (const glcMaterial& ma, const glcMaterial& mb)	{return glcMaterial(ma.a / mb.a, ma.b / mb.b, ma.c / mb.c, ma.s / mb.s);}
inline glcMaterial operator / (const glcMaterial& mc, const float f)			{return glcMaterial(mc.a / f, mc.b / f, mc.c / f, mc.s / f);}

inline ostream & operator<<(ostream & os, const glcMaterial & m) {os << "amb:\t" << m.a << endl << "diff:\t" << m.b << endl << "spec:\t" << m.c << endl << "shininess: " << m.s; return os;}

#endif /*GLCMATERIAL_H_*/
