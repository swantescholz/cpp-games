#ifndef TBCOLOR_H_
#define TBCOLOR_H_

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`__\ /\ \ \ \  __\ /\ __\_ /\  __\   /__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbColor.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration einer Farbklasse und von Farbfunktionen

	Autor:
	David Scherfgen

********************************************************************/


// Makros und Definitionen
#define TB_COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255

#include <iostream>
#include "glcTimer.h"
using namespace std;

// ******************************************************************
// Die Farbklasse
class tbColor
{
public:
	// Variablen
	union
	{
		struct
		{
			float r;	// Rot-, Gruen-, Blau- und Alphakomponente
			float g;
			float b;
			float a;
		};

		struct
		{
			float fRed;		// Rot-, Gruen-, Blau- und Alphakomponente
			float fGreen;
			float fBlue;
			float fAlpha;
		};
		
		float			c[4];			// Array der Komponenten
	};

	// Konstruktoren
	tbColor()																																{}
	tbColor(const tbColor& c) : r(c.r), g(c.g), b(c.b), a(c.a)																				{}
	tbColor(const float f) : r(f), g(f), b(f), a(1.0f)																						{}
	tbColor(const float _r, const float _g, const float _b) : r(_r), g(_g), b(_b), a(1.0f)													{}
	tbColor(const float _r, const float _g, const float _b, const float _a) : r(_r), g(_g), b(_b), a(_a)									{}
	tbColor(const float* pfComponent) : r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3])							{}

	// Casting-Operatoren
	operator float* ()			{return (float*)(c);}
	//operator SDL_Color ()		{SDL_Color col = {r,g,b,a}; return col;}

	// Zuweisungsoperatoren
	tbColor& operator =  (const tbColor& c)	{r=c.r; g=c.g; b=c.b; a=c.a; return *this;}
	tbColor& operator += (const tbColor& c)	{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
	tbColor& operator -= (const tbColor& c)	{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
	tbColor& operator *= (const tbColor& c)	{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
	tbColor& operator *= (const float f)	{r *= f; g *= f; b *= f; a *= f; return *this;}
	tbColor& operator /= (const tbColor& c)	{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
	tbColor& operator /= (const float f)	{r /= f; g /= f; b /= f; a /= f; return *this;}
	
	//selbst
	SDL_Color GetSDLCol() const {const SDL_Color col = {r*255,g*255,b*255,a*255}; return col;}
};

// Arithmetische Operatoren
inline tbColor operator + (const tbColor& a, const tbColor& b)	{return tbColor(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);}
inline tbColor operator - (const tbColor& a, const tbColor& b)	{return tbColor(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);}
inline tbColor operator - (const tbColor& c)					{return tbColor(-c.r, -c.g, -c.b, c.a);}
inline tbColor operator * (const tbColor& a, const tbColor& b)	{return tbColor(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);}
inline tbColor operator * (const tbColor& c, const float f)		{return tbColor(c.r * f, c.g * f, c.b * f, c.a * f);}
inline tbColor operator * (const float f, const tbColor& c)		{return tbColor(c.r * f, c.g * f, c.b * f, c.a * f);}
inline tbColor operator / (const tbColor& a, const tbColor& b)	{return tbColor(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);}
inline tbColor operator / (const tbColor& c, const float f)		{return tbColor(c.r / f, c.g / f, c.b / f, c.a / f);}

// Vergleichsoperatoren
inline bool operator == (const tbColor& a, const tbColor& b) {if(a.r != b.r) return false; if(a.g != b.g) return false; if(a.b != b.b) return false; return a.a == b.a;}
inline bool operator != (const tbColor& a, const tbColor& b) {if(a.r != b.r) return true; if(a.g != b.g) return true; if(a.b != b.b) return true; return a.a != b.a;}

// ******************************************************************
// Funktionen deklarieren
inline tbColor	tbColorNegate(const tbColor& c)											{return tbColor(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);}
inline float	tbColorBrightness(const tbColor& c)										{return c.r * 0.299f + c.g * 0.587f + c.b * 0.114f;}
inline tbColor	tbColorInterpolate(const tbColor& c1, const tbColor& c2, const float p)	{return c1 + p * (c2 - c1);}
inline tbColor	tbColorMin(const tbColor& c1, const tbColor& c2)						{return tbColor(TB_MIN(c1.r, c2.r), TB_MIN(c1.g, c2.g), TB_MIN(c1.b, c2.b), TB_MIN(c1.a, c2.a));}
inline tbColor	tbColorMax(const tbColor& c1, const tbColor& c2)						{return tbColor(TB_MAX(c1.r, c2.r), TB_MAX(c1.g, c2.g), TB_MAX(c1.b, c2.b), TB_MAX(c1.a, c2.a));}
inline tbColor	tbColorRandom(float fAlpha = 1.0f)										{return tbColor(g_pTimer->Random(0.0f, 1.0f), g_pTimer->Random(0.0f, 1.0f), g_pTimer->Random(0.0f, 1.0f), fAlpha < 0.0f ? g_pTimer->Random(0.0f, 1.0f) : fAlpha);}
//selbstgemacht
inline ostream & operator<<(ostream & os, const tbColor & col) {os << "r: " << col.r << ", g: " << col.g << ", b: " << col.b << ", a: " << col.a; return os;}

// ******************************************************************

#endif /*TBCOLOR_H_*/
