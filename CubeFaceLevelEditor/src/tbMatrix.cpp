/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMatrix.h
	==========
	Diese Datei ist Teil der TriBase-Engine

	Zweck:
	Matrizenrechnung

	Autor:
	David Scherfgen

********************************************************************/

#include "tbMatrix.h"

// ******************************************************************
// Translationsmatrix berechnen
tbMatrix tbMatrixTranslation(const tbVector3& v)
{
	return tbMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					v.x,  v.y,  v.z,  1.0f);
}

// ******************************************************************
// Rotationsmatrix für Rotation um die x-Achse berechnen
tbMatrix tbMatrixRotationX(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = cosf(f);
	mResult.m23 = sinf(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die y-Achse berechnen
tbMatrix tbMatrixRotationY(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = cosf(f);
	mResult.m31 = sinf(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die z-Achse berechnen
tbMatrix tbMatrixRotationZ(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = cosf(f);
	mResult.m12 = sinf(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}

// ******************************************************************
// Rotiert um alle drei Achsen
tbMatrix tbMatrixRotation(const float x,
									  const float y,
									  const float z)
{
	return tbMatrixRotationZ(z) *
		   tbMatrixRotationX(x) *
		   tbMatrixRotationY(y);
}

// ******************************************************************
// Rotiert um alle drei Achsen (Winkel in Vektor)
tbMatrix tbMatrixRotation(const tbVector3& v)
{
	return tbMatrixRotationZ(v.z) *
		   tbMatrixRotationX(v.x) *
		   tbMatrixRotationY(v.y);
}

// ******************************************************************
// Rotationsmatrix für Rotation um eine beliebige Achse berechnen
tbMatrix tbMatrixRotationAxis(const tbVector3& v,
										  const float f)
{
	// Sinus und Kosinus berechnen
	const float fSin = sinf(-f);
	const float fCos = cosf(-f);
	const float fOneMinusCos = 1.0f - fCos;

	// Achsenvektor normalisieren
	const tbVector3 vAxis(tbVector3Normalize(v));

	// Matrix erstellen
	return tbMatrix((vAxis.x * vAxis.x) * fOneMinusCos + fCos,
		            (vAxis.x * vAxis.y) * fOneMinusCos - (vAxis.z * fSin),
				    (vAxis.x * vAxis.z) * fOneMinusCos + (vAxis.y * fSin),
					0.0f,
					(vAxis.y * vAxis.x) * fOneMinusCos + (vAxis.z * fSin),
					(vAxis.y * vAxis.y) * fOneMinusCos + fCos,
					(vAxis.y * vAxis.z) * fOneMinusCos - (vAxis.x * fSin),
					0.0f,
					(vAxis.z * vAxis.x) * fOneMinusCos - (vAxis.y * fSin),
					(vAxis.z * vAxis.y) * fOneMinusCos + (vAxis.x * fSin),
					(vAxis.z * vAxis.z) * fOneMinusCos + fCos,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f);
}

// ******************************************************************
// Skalierungsmatrix berechnen
tbMatrix tbMatrixScaling(const tbVector3& v)
{
	// Skalierungsmatrix berechnen
	return tbMatrix(v.x,  0.0f, 0.0f, 0.0f,
		            0.0f, v.y,  0.0f, 0.0f,
					0.0f, 0.0f, v.z,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************
// Liefert eine Achsenmatrix
tbMatrix tbMatrixAxes(const tbVector3& vXAxis,
								  const tbVector3& vYAxis,
								  const tbVector3& vZAxis)
{
	return tbMatrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		            vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
					vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Determinante einer Matrix berechnen
float tbMatrixDet(const tbMatrix& m)
{
	// Determinante der linken oberen 3x3-Teilmatrix berechnen
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// ******************************************************************
// Invertierte Matrix berechnen
tbMatrix tbMatrixInvert(const tbMatrix& m)
{
	// Kehrwert der Determinante vorberechnen
	float fInvDet = tbMatrixDet(m);
	if(fInvDet == 0.0f) return tbMatrixIdentity();
	fInvDet = 1.0f / fInvDet;

	// Invertierte Matrix berechnen
	tbMatrix mResult;
	mResult.m11 =  fInvDet * (m.m22 * m.m33 - m.m23 * m.m32);
	mResult.m12 = -fInvDet * (m.m12 * m.m33 - m.m13 * m.m32);
	mResult.m13 =  fInvDet * (m.m12 * m.m23 - m.m13 * m.m22);
	mResult.m14 =  0.0f;
	mResult.m21 = -fInvDet * (m.m21 * m.m33 - m.m23 * m.m31);
	mResult.m22 =  fInvDet * (m.m11 * m.m33 - m.m13 * m.m31);
	mResult.m23 = -fInvDet * (m.m11 * m.m23 - m.m13 * m.m21);
	mResult.m24 =  0.0f;
	mResult.m31 =  fInvDet * (m.m21 * m.m32 - m.m22 * m.m31);
	mResult.m32 = -fInvDet * (m.m11 * m.m32 - m.m12 * m.m31);
	mResult.m33 =  fInvDet * (m.m11 * m.m22 - m.m12 * m.m21);
	mResult.m34 =  0.0f;
	mResult.m41 = -(m.m41 * mResult.m11 + m.m42 * mResult.m21 + m.m43 * mResult.m31);
	mResult.m42 = -(m.m41 * mResult.m12 + m.m42 * mResult.m22 + m.m43 * mResult.m32);
	mResult.m43 = -(m.m41 * mResult.m13 + m.m42 * mResult.m23 + m.m43 * mResult.m33);
	mResult.m44 =  1.0f;

	return mResult;
}

// ******************************************************************
// Transponierte Matrix berechnen
tbMatrix tbMatrixTranspose(const tbMatrix& m)
{
	// Matrix transponieren
	return tbMatrix(m.m11, m.m21, m.m31, m.m41,
		            m.m12, m.m22, m.m32, m.m42,
					m.m13, m.m23, m.m33, m.m43,
					m.m14, m.m24, m.m34, m.m44);
}

// ******************************************************************
// Projektionsmatrix berechnen
tbMatrix tbMatrixProjection(const float fFOV,
										const float fAspect,
										const float fNearPlane,
										const float fFarPlane)
{
	const float s = 1.0f / tanf(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	return tbMatrix(s / fAspect, 0.0f, 0.0f, 0.0f,
		            0.0f, s, 0.0f, 0.0f,
					0.0f, 0.0f, Q, 1.0f,
					0.0f, 0.0f, -Q * fNearPlane, 0.0f);
}

// ******************************************************************
// Kameramatrix berechnen
tbMatrix tbMatrixCamera(const tbVector3& vPos,
									const tbVector3& vLookAt,
									const tbVector3& vUp) // = tbVector3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse des Kamerakoordinatensystems berechnen
	tbVector3 vZAxis(tbVector3Normalize(vLookAt - vPos));

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	tbVector3 vXAxis(tbVector3Normalize(tbVector3Cross(vUp, vZAxis)));

	// y-Achse berechnen
	tbVector3 vYAxis(tbVector3Normalize(tbVector3Cross(vZAxis, vXAxis)));

	// Rotationsmatrix erzeugen und die Translationsmatrix mit ihr multiplizieren
	return tbMatrixTranslation(-vPos) *
	       tbMatrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		            vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				    vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Texturmatrix berechnen
tbMatrix tbMatrixToTex2DMatrix(const tbMatrix& m)
{
	// Texturmatrix berechnen
	return tbMatrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}

// ******************************************************************
