#ifndef TBVECTOR3_H_
#define TBVECTOR3_H_


#include <iostream>
#include <math.h>
using namespace std;
#include "glcTimer.h"
#include "tbVector2.h"

#define TB_MIN(X,Y) ((X<Y)?X:Y)
#define TB_MAX(X,Y) ((X>Y)?X:Y)


// ******************************************************************
// Die 3D-Vektor-Klasse
class tbVector3
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
			float z;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
			float w;
		};

		float		c[3];		// Array der Koordinaten
		//D3DVECTOR	vD3DVector;	// Vektor in Form eines Direct3D-Vektors
	};

	// Konstruktoren
	tbVector3()																						{}
	tbVector3(const tbVector3& v) : x(v.x), y(v.y), z(v.z)											{}
	tbVector3(const float f) : x(f), y(f), z(f)														{}
	tbVector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	tbVector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}
	tbVector3(const tbVector2& v, const float f = 0.0f) : x(v.x), y(v.y), z(f)						{}
	tbVector3(const float f, const tbVector2& v       ) : x(v.x), y(v.y), z(f)						{}
	
	//Destruktoren (self-made)
	~tbVector3() {}
	
	// Casting-Operatoren
	operator float* ()					{return (float*)(c);}
	//operator D3DVECTOR& ()				{return vD3DVector;}
	//operator const D3DVECTOR& () const	{return vD3DVector;}

	// Zuweisungsoperatoren
	tbVector3& operator = (const tbVector3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	tbVector3& operator += (const tbVector3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	tbVector3& operator -= (const tbVector3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	tbVector3& operator *= (const tbVector3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	tbVector3& operator *= (const float f)		{x *= f; y *= f; z *= f; return *this;}
	tbVector3& operator /= (const tbVector3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	tbVector3& operator /= (const float f)		{x /= f; y /= f; z /= f; return *this;}
};

// Arithmetische Operatoren
inline tbVector3 operator + (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x + b.x, a.y + b.y, a.z + b.z);}
inline tbVector3 operator - (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x - b.x, a.y - b.y, a.z - b.z);}
inline tbVector3 operator - (const tbVector3& v)						{return tbVector3(-v.x, -v.y, -v.z);}
inline tbVector3 operator * (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x * b.x, a.y * b.y, a.z * b.z);}
inline tbVector3 operator * (const tbVector3& v, const float f)			{return tbVector3(v.x * f, v.y * f, v.z * f);}
inline tbVector3 operator * (const float f, const tbVector3& v)			{return tbVector3(v.x * f, v.y * f, v.z * f);}
inline tbVector3 operator / (const tbVector3& a, const tbVector3& b)	{return tbVector3(a.x / b.x, a.y / b.y, a.z / b.z);}
inline tbVector3 operator / (const tbVector3& v, const float f)			{return tbVector3(v.x / f, v.y / f, v.z / f);}

// Vergleichsoperatoren
inline bool operator == (const tbVector3& a, const tbVector3& b) {if(a.x != b.x) return false; if(a.y != b.y) return false; return a.z == b.z;}
inline bool operator != (const tbVector3& a, const tbVector3& b) {if(a.x != b.x) return true; if(a.y != b.y) return true; return a.z != b.z;}

// ******************************************************************
// Funktionen deklarieren
inline float		tbVector3Length(const tbVector3& v)													{return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline float		tbVector3LengthSq(const tbVector3& v)												{return v.x * v.x + v.y * v.y + v.z * v.z;}
inline tbVector3	tbVector3Normalize(const tbVector3& v)												{return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline tbVector3	tbVector3NormalizeEx(const tbVector3& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);}
inline tbVector3	tbVector3Cross(const tbVector3& v1, const tbVector3& v2)							{return tbVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
inline float		tbVector3Dot(const tbVector3& v1, const tbVector3& v2)								{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
inline float		tbVector3Angle(const tbVector3& v1, const tbVector3& v2)							{return acosf((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / sqrtf((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)));}
inline tbVector3	tbVector3InterpolateCoords(const tbVector3& v1, const tbVector3& v2, const float p)	{return v1 + p * (v2 - v1);}
inline tbVector3	tbVector3InterpolateNormal(const tbVector3& v1, const tbVector3& v2, const float p)	{return tbVector3NormalizeEx(v1 + p * (v2 - v1));}
inline tbVector3	tbVector3Min(const tbVector3& v1, const tbVector3& v2)								{return tbVector3(TB_MIN(v1.x, v2.x), TB_MIN(v1.y, v2.y), TB_MIN(v1.z, v2.z));}
inline tbVector3	tbVector3Max(const tbVector3& v1, const tbVector3& v2)								{return tbVector3(TB_MAX(v1.x, v2.x), TB_MAX(v1.y, v2.y), TB_MAX(v1.z, v2.z));}
inline tbVector3	tbVector3Random()																	{return tbVector3NormalizeEx(tbVector3(g_pTimer->Random(-1.0f, 1.0f), g_pTimer->Random(-1.0f, 1.0f), g_pTimer->Random(-1.0f, 1.0f)));}

//selbst
inline ostream & operator<<(ostream & os, const tbVector3 & v) {os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z; return os;} 
inline bool tbVector3Compare(const tbVector3 &v1, const tbVector3 &v2, float f = 0.001f) {if (tbVector3LengthSq(v2 - v1) < f * f) return true; return false;}

// ******************************************************************

#endif /*TBVECTOR3_H_*/
