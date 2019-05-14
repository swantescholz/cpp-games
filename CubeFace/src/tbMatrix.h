#ifndef TBMATRIX_H_
#define TBMATRIX_H_


#include <iostream>
#include <math.h>
using namespace std;
#include "tbVector3.h"

// Verwendete Symbole vordefinieren

class tbVector3;
class tbMatrix;
tbMatrix tbMatrixInvert(const tbMatrix& m);

// ******************************************************************
// Die Matrixklasse
class tbMatrix
{
public:
	// Variablen
	union
	{
		struct
		{
			float m11, m12, m13, m14,	// Elemente der Matrix
				  m21, m22, m23, m24,
				  m31, m32, m33, m34,
				  m41, m42, m43, m44;
		};

		float		m[4][4];			// Zweidimensionales Array der Elemente
		float		n[16];				// Eindimensionales Array der Elemente
		//D3DMATRIX	mD3DMatrix;			// Matrix als D3DMATRIX
	};

	// Konstruktoren
	tbMatrix() {}

	tbMatrix(const tbMatrix& m) : m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
                                  m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
								  m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
								  m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44) {}

	tbMatrix(float _m11, float _m12, float _m13, float _m14,
			 float _m21, float _m22, float _m23, float _m24,
			 float _m31, float _m32, float _m33, float _m34,
			 float _m41, float _m42, float _m43, float _m44) : m11(_m11), m12(_m12), m13(_m13), m14(_m14),
			                                                   m21(_m21), m22(_m22), m23(_m23), m24(_m24),
															   m31(_m31), m32(_m32), m33(_m33), m34(_m34),
															   m41(_m41), m42(_m42), m43(_m43), m44(_m44) {}

	//tbMatrix(const float* pfValue) : mD3DMatrix(*((D3DMATRIX*)(pfValue)))	{}
	//tbMatrix(const D3DMATRIX& m) : mD3DMatrix(m)							{}

	// Casting-Opeatoren
	operator float* ()					{return (float*)(n);}
	//operator D3DMATRIX& ()				{return mD3DMatrix;}
	//operator const D3DMATRIX& () const	{return mD3DMatrix;}

	// Zugriffsoperatoren
	float& operator () (int iRow, int iColumn) {return m[iRow - 1][iColumn - 1];}
	float operator () (int iRow, int iColumn) const {return m[iRow - 1][iColumn - 1];}

	// Zuweisungsoperatoren
	//tbMatrix& operator = (const tbMatrix& m) {mD3DMatrix = m.mD3DMatrix; return *this;}
	
	//** selbst gemacht()()
	tbMatrix& operator = (const tbMatrix& m)
	{
		m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
		m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
		m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
		m41 = m.m41; m42 = m.m42; m43 = m.m43; m44 = m.m44;
		return *this;
	}
	
	tbMatrix& operator += (const tbMatrix& m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
		m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
		m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
		m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
		return *this;
	}

	tbMatrix& operator -= (const tbMatrix& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
		m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
		m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
		return *this;
	}

	tbMatrix& operator *= (const tbMatrix& m)
	{
		return *this = tbMatrix(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
								m.m12 * m11 + m.m22 * m12 + m.m32 * m13 + m.m42 * m14,
								m.m13 * m11 + m.m23 * m12 + m.m33 * m13 + m.m43 * m14,
								m.m14 * m11 + m.m24 * m12 + m.m34 * m13 + m.m44 * m14,
								m.m11 * m21 + m.m21 * m22 + m.m31 * m23 + m.m41 * m24,
								m.m12 * m21 + m.m22 * m22 + m.m32 * m23 + m.m42 * m24,
								m.m13 * m21 + m.m23 * m22 + m.m33 * m23 + m.m43 * m24,
								m.m14 * m21 + m.m24 * m22 + m.m34 * m23 + m.m44 * m24,
								m.m11 * m31 + m.m21 * m32 + m.m31 * m33 + m.m41 * m34,
								m.m12 * m31 + m.m22 * m32 + m.m32 * m33 + m.m42 * m34,
								m.m13 * m31 + m.m23 * m32 + m.m33 * m33 + m.m43 * m34,
								m.m14 * m31 + m.m24 * m32 + m.m34 * m33 + m.m44 * m34,
								m.m11 * m41 + m.m21 * m42 + m.m31 * m43 + m.m41 * m44,
								m.m12 * m41 + m.m22 * m42 + m.m32 * m43 + m.m42 * m44,
								m.m13 * m41 + m.m23 * m42 + m.m33 * m43 + m.m43 * m44,
								m.m14 * m41 + m.m24 * m42 + m.m34 * m43 + m.m44 * m44);
	}

	tbMatrix& operator *= (const float f)
	{
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}
	
	tbMatrix& operator /= (const tbMatrix& m)
	{
		return *this *= tbMatrixInvert(m);
	}

	tbMatrix& operator /= (const float f)
	{
		m11 /= f; m12 /= f; m13 /= f; m14 /= f;
		m21 /= f; m22 /= f; m23 /= f; m24 /= f;
		m31 /= f; m32 /= f; m33 /= f; m34 /= f;
		m41 /= f; m42 /= f; m43 /= f; m44 /= f;
		return *this;
	}
};

// Arithmetische Operatoren
inline tbMatrix operator + (const tbMatrix& a, const tbMatrix& b)	{return tbMatrix(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34, a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);}
inline tbMatrix operator - (const tbMatrix& a, const tbMatrix& b)	{return tbMatrix(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24, a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34, a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44);}
inline tbMatrix operator - (const tbMatrix& m)						{return tbMatrix(-m.m11, -m.m12, -m.m13, -m.m14, -m.m21, -m.m22, -m.m23, -m.m24, -m.m31, -m.m32, -m.m33, -m.m34, -m.m41, -m.m42, -m.m43, -m.m44);}

inline tbMatrix operator * (const tbMatrix& a,
							const tbMatrix& b)
{
	return tbMatrix(b.m11 * a.m11 + b.m21 * a.m12 + b.m31 * a.m13 + b.m41 * a.m14,
					b.m12 * a.m11 + b.m22 * a.m12 + b.m32 * a.m13 + b.m42 * a.m14,
					b.m13 * a.m11 + b.m23 * a.m12 + b.m33 * a.m13 + b.m43 * a.m14,
					b.m14 * a.m11 + b.m24 * a.m12 + b.m34 * a.m13 + b.m44 * a.m14,
					b.m11 * a.m21 + b.m21 * a.m22 + b.m31 * a.m23 + b.m41 * a.m24,
					b.m12 * a.m21 + b.m22 * a.m22 + b.m32 * a.m23 + b.m42 * a.m24,
					b.m13 * a.m21 + b.m23 * a.m22 + b.m33 * a.m23 + b.m43 * a.m24,
					b.m14 * a.m21 + b.m24 * a.m22 + b.m34 * a.m23 + b.m44 * a.m24,
					b.m11 * a.m31 + b.m21 * a.m32 + b.m31 * a.m33 + b.m41 * a.m34,
					b.m12 * a.m31 + b.m22 * a.m32 + b.m32 * a.m33 + b.m42 * a.m34,
					b.m13 * a.m31 + b.m23 * a.m32 + b.m33 * a.m33 + b.m43 * a.m34,
					b.m14 * a.m31 + b.m24 * a.m32 + b.m34 * a.m33 + b.m44 * a.m34,
					b.m11 * a.m41 + b.m21 * a.m42 + b.m31 * a.m43 + b.m41 * a.m44,
					b.m12 * a.m41 + b.m22 * a.m42 + b.m32 * a.m43 + b.m42 * a.m44,
					b.m13 * a.m41 + b.m23 * a.m42 + b.m33 * a.m43 + b.m43 * a.m44,
					b.m14 * a.m41 + b.m24 * a.m42 + b.m34 * a.m43 + b.m44 * a.m44);
}

inline tbMatrix operator * (const tbMatrix& m,
							const float f)
{
	return tbMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline tbMatrix operator * (const float f,
							const tbMatrix& m)
{
	return tbMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			        m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
					m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
					m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
}

inline tbMatrix operator / (const tbMatrix& a, const tbMatrix& b) {return a * tbMatrixInvert(b);}

inline tbMatrix operator / (const tbMatrix& m,
							const float f)
{
	return tbMatrix(m.m11 / f, m.m12 / f, m.m13 / f, m.m14 / f,
			        m.m21 / f, m.m22 / f, m.m23 / f, m.m24 / f,
					m.m31 / f, m.m32 / f, m.m33 / f, m.m34 / f,
					m.m41 / f, m.m42 / f, m.m43 / f, m.m44 / f);
}

// Vergleichsoperatoren
inline bool operator == (const tbMatrix& a,
						 const tbMatrix& b)
{
	if(a.m11 != b.m11) return false;
	if(a.m12 != b.m12) return false;
	if(a.m13 != b.m13) return false;
	if(a.m14 != b.m14) return false;
	if(a.m21 != b.m21) return false;
	if(a.m22 != b.m22) return false;
	if(a.m23 != b.m23) return false;
	if(a.m24 != b.m24) return false;
	if(a.m31 != b.m31) return false;
	if(a.m32 != b.m32) return false;
	if(a.m33 != b.m33) return false;
	if(a.m34 != b.m34) return false;
	if(a.m41 != b.m41) return false;
	if(a.m42 != b.m42) return false;
	if(a.m43 != b.m43) return false;
	return a.m44 == b.m44;
}

// Vergleichsoperatoren
inline bool operator != (const tbMatrix& a,
						 const tbMatrix& b)
{
	if(a.m11 != b.m11) return true;
	if(a.m12 != b.m12) return true;
	if(a.m13 != b.m13) return true;
	if(a.m14 != b.m14) return true;
	if(a.m21 != b.m21) return true;
	if(a.m22 != b.m22) return true;
	if(a.m23 != b.m23) return true;
	if(a.m24 != b.m24) return true;
	if(a.m31 != b.m31) return true;
	if(a.m32 != b.m32) return true;
	if(a.m33 != b.m33) return true;
	if(a.m34 != b.m34) return true;
	if(a.m41 != b.m41) return true;
	if(a.m42 != b.m42) return true;
	if(a.m43 != b.m43) return true;
	return a.m44 != b.m44;
}

// ******************************************************************
// Funktionen deklarieren
inline		tbMatrix	tbMatrixIdentity() {return tbMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}	// Identitätsmatrix liefern
tbMatrix	tbMatrixTranslation(const tbVector3& v);																								// Translationsmatrix (Verschiebungsmatrix) berechnen
tbMatrix	tbMatrixRotationX(const float f);																										// Rotationsmatrix um die X-Achse berechnen
tbMatrix	tbMatrixRotationY(const float f);																										// Rotationsmatrix um die Y-Achse berechnen
tbMatrix	tbMatrixRotationZ(const float f);																										// Rotationsmatrix um die Z-Achse berechnen
tbMatrix	tbMatrixRotation(const float x, const float y, const float z);																			// Rotiert um alle drei Achsen
tbMatrix	tbMatrixRotation(const tbVector3& v);																									// Rotiert um alle drei Achsen
tbMatrix	tbMatrixRotationAxis(const tbVector3& v, const float f);																				// Rotationsmatrix um eine beliebige Achse berechnen
tbMatrix	tbMatrixScaling(const tbVector3& v);																									// Skalierungsmatrix berechnen
tbMatrix	tbMatrixAxes(const tbVector3& vXAxis, const tbVector3& vYAxis, const tbVector3& vZAxis);												// Liefert eine Achsenmatrix
float		tbMatrixDet(const tbMatrix& m, float* pfOut);																							// Determinante berechnen
tbMatrix	tbMatrixInvert(const tbMatrix& m);																										// Invertierte (umgekehrte) Matrix berechnen
tbMatrix	tbMatrixTranspose(const tbMatrix& m);																									// Transponierte Matrix berechnen
tbMatrix	tbMatrixProjection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
tbMatrix	tbMatrixCamera(const tbVector3& vPos, const tbVector3& vLookAt, const tbVector3& vUp);// = tbVector3(0.0f, 1.0f, 0.0f));					// Kameramatrix erzeugen
tbMatrix	tbMatrixToTex2DMatrix(const tbMatrix& m);																								// In Texturmatrix umwandeln

//selbstgemacht
inline ostream & operator<<(ostream & os, const tbMatrix & m) {for(int i= 0; i < 4; i++) {for (int j = 0; j < 4; j++) os << m.n[4*i+j] << ", "; if(i!=3) os << endl;} return os;}
// ******************************************************************

#endif /*TBMATRIX_H_*/
