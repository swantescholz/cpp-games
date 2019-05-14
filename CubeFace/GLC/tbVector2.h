#ifndef TBVECTOR2_H_
#define TBVECTOR2_H_

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`__\ /\ \ \ \  __\ /\ __\_ /\  __\   /__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVector2.h
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rechnen mit 2D-Vektoren

	Autor:
	David Scherfgen

********************************************************************/
#include <iostream>
#include "glcTimer.h"
using namespace std;

// Verwendete Symbole vordefinieren
class tbMatrix;

// ******************************************************************
// Die 2D-Vektor-Klasse
class tbVector2
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
		};

		float c[2];		// Array der Koordinaten
	};

	// Konstruktoren
	tbVector2()																	{}
	tbVector2(const tbVector2& v) : x(v.x), y(v.y)								{}
	tbVector2(const float f) : x(f), y(f)										{}
	tbVector2(const float _x, const float _y) : x(_x), y(_y)					{}
	tbVector2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	operator float* () {return (float*)(c);}

	// Zuweisungsoperatoren
	tbVector2& operator = (const tbVector2& v)	{x = v.x; y = v.y; return *this;}
	tbVector2& operator += (const tbVector2& v)	{x += v.x; y += v.y; return *this;}
	tbVector2& operator -= (const tbVector2& v)	{x -= v.x; y -= v.y; return *this;}
	tbVector2& operator *= (const tbVector2& v)	{x *= v.x; y *= v.y; return *this;}
	tbVector2& operator *= (const float f)		{x *= f; y *= f; return *this;}
	tbVector2& operator /= (const tbVector2& v)	{x /= v.x; y /= v.y; return *this;}
	tbVector2& operator /= (const float f)		{x /= f; y /= f; return *this;}
};

// Arithmetische Operatoren
inline tbVector2 operator + (const tbVector2& a, const tbVector2& b)	{return tbVector2(a.x + b.x, a.y + b.y);}
inline tbVector2 operator - (const tbVector2& a, const tbVector2& b)	{return tbVector2(a.x - b.x, a.y - b.y);}
inline tbVector2 operator - (const tbVector2& v)						{return tbVector2(-v.x, -v.y);}
inline tbVector2 operator * (const tbVector2& a, const tbVector2& b)	{return tbVector2(a.x * b.x, a.y * b.y);}
inline tbVector2 operator * (const tbVector2& v, const float f)			{return tbVector2(v.x * f, v.y * f);}
inline tbVector2 operator * (const float f, const tbVector2& v)			{return tbVector2(v.x * f, v.y * f);}
inline tbVector2 operator / (const tbVector2& a, const tbVector2& b)	{return tbVector2(a.x / b.x, a.y / b.y);}
inline tbVector2 operator / (const tbVector2& v, const float f)			{return tbVector2(v.x / f, v.y / f);}

// Vergleichsoperatoren
inline bool operator == (const tbVector2& a, const tbVector2& b) {if(a.x != b.x) return false; return a.y == b.y;}
inline bool operator != (const tbVector2& a, const tbVector2& b) {if(a.x != b.x) return true; return a.y != b.y;}

// ******************************************************************
// Funktionen deklarieren
inline float		tbVector2Length(const tbVector2& v)													{return sqrtf(v.x * v.x + v.y * v.y);}
inline float		tbVector2LengthSq(const tbVector2& v)												{return v.x * v.x + v.y * v.y;}
inline tbVector2	tbVector2Normalize(const tbVector2& v)												{return v / sqrtf(v.x * v.x + v.y * v.y);}
inline tbVector2	tbVector2NormalizeEx(const tbVector2& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y) + 0.0001f);}
inline float		tbVector2Dot(const tbVector2& v1, const tbVector2& v2)								{return v1.x * v2.x + v1.y * v2.y;}
inline float		tbVector2Angle(const tbVector2& v1, const tbVector2& v2)							{return acosf((v1.x * v2.x + v1.y * v2.y) / sqrtf((v1.x * v1.x + v1.y * v1.y) * (v2.x * v2.x + v2.y * v2.y)));}
inline tbVector2	tbVector2InterpolateCoords(const tbVector2& v1, const tbVector2& v2, const float p)	{return v1 + p * (v2 - v1);}
inline tbVector2	tbVector2InterpolateNormal(const tbVector2& v1, const tbVector2& v2, const float p)	{return tbVector2NormalizeEx(v1 + p * (v2 - v1));}
inline tbVector2	tbVector2Min(const tbVector2& v1, const tbVector2& v2)								{return tbVector2(TB_MIN(v1.x, v2.x), TB_MIN(v1.y, v2.y));}
inline tbVector2	tbVector2Max(const tbVector2& v1, const tbVector2& v2)								{return tbVector2(TB_MAX(v1.x, v2.x), TB_MAX(v1.y, v2.y));}
inline tbVector2	tbVector2Random()																	{return tbVector2NormalizeEx(tbVector2(g_pTimer->Random(-1.0f, 1.0f), g_pTimer->Random(-1.0f, 1.0f)));}
//selbstgemacht
inline ostream & operator<<(ostream & os, const tbVector2 & v) {os << "x: " << v.x << ", y: " << v.y; return os;}

// ******************************************************************

#endif /*TBVECTOR2_H_*/
