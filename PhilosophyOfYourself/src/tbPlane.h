#ifndef TBPLANE_H_
#define TBPLANE_H_

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`__\ /\ \ \ \  __\ /\ __\_ /\  __\   /__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbPlane.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration einer Ebenenklasse und von Ebenenfunktionen

	Autor:
	David Scherfgen

********************************************************************/

#include <iostream>
#include "tbVector3.h"
using namespace std;

// Die Ebenenklasse
class tbPlane
{
public:
	// Variablen
	
	tbVector3 	n; // Normalenvektor
	float 		d; // Distanz
	
	/*union // mit tbVector3 in union klappt nicht!
	{
		struct
		{
			float a;		// Variablen der Ebenengleichung
			float b;
			float c;
			float d;
		};

		struct
		{
			tbVector3	n;	// Normalenvektor
			float		d;	// Distanz
		};

		float v[4];			// Array der Variablen
	};*/

	// Konstruktoren
	tbPlane()																								{}
	tbPlane(const tbPlane& p) : n(p.n), d(p.d)																{}
	tbPlane(const float _a, const float _b, const float _c, const float _d) : n(_a, _b, _c), d(_d)			{}
	tbPlane(const float* pfValue) : n(pfValue[0], pfValue[1], pfValue[2]), d(pfValue[3])					{}
	tbPlane(const tbVector3& _n, float _d) : n(_n), d(_d)													{}

	// Casting-Operatoren
	//operator float* () {return (float*)(v);}

	// Zuweisungsoperatoren
	tbPlane& operator = (const tbPlane& p) {n = p.n; d = p.d; return *this;}
};

// Vergleichsoperatoren
inline bool operator == (const tbPlane& a, const tbPlane& b) {if(a.n != b.n) return false; return a.d == b.d;}
inline bool operator != (const tbPlane& a, const tbPlane& b) {if(a.n != b.n) return true;  return a.d != b.d;}

// ******************************************************************
// Funktionen deklarieren
inline tbPlane	tbPlaneNormalize(const tbPlane& p)													{const float fLength = tbVector3Length(p.n); return tbPlane(p.n / fLength, p.d / fLength);}
inline float	tbPlaneDotNormal(const tbPlane& p, const tbVector3& v)								{return p.n.x * v.x + p.n.y * v.y + p.n.z * v.z;}
inline float	tbPlaneDotCoords(const tbPlane& p, const tbVector3& v)								{return p.n.x * v.x + p.n.y * v.y + p.n.z * v.z + p.d;}
inline tbPlane	tbPlaneFromPointNormal(const tbVector3& p, const tbVector3& n)						{return tbPlane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
inline tbPlane	tbPlaneFromPoints(const tbVector3& v1, const tbVector3& v2, const tbVector3& v3)	{return tbPlaneFromPointNormal(v1, tbVector3Cross(v3 - v2, v1 - v2));}

//selbstgemacht
inline ostream & operator<<(ostream & os, const tbPlane & p) {os << "n: " << p.n.x << ", " << p.n.y << ", " << p.n.z << ", p: " << p.d; return os;}
// ******************************************************************

#endif /*TBPLANE_H_*/
