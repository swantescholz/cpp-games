#ifndef TBUTILS_H_
#define TBUTILS_H_

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`__\ /\ \ \ \  __\ /\ __\_ /\  __\   /__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbUtils.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Verschiedene Hilfsfunktionen

	Autor:
	David Scherfgen

********************************************************************/

#include "glcException.h"
#include "glcLogfile.h"
#include "tbVector3.h"
#include "tbVector2.h"
#include "tbMatrix.h"
#include "tbPlane.h"
#include <iostream>
#include <math.h>
using namespace std;

// Statuscodes
enum tbResult
{
	TB_OK			= 0,	// Erfolg
	TB_ERROR		= 1,	// Allgemeiner Fehler
	TB_CANCELED		= 2,	// Aktion abgebrochen
	TB_NOT_FOUND	= 3,	// Nicht gefunden
	TB_STOP			= 4,	// Stopp (Vorgang abbrechen)
};


// ******************************************************************
// Funktionen deklarieren

//von S. Scholz:
std::string	operator+= (      std::string &s, bool   b);
std::string	operator+  (const std::string &s, bool   b);
std::string	operator+= (      std::string &s, int    i);
std::string	operator+  (const std::string &s, int    i);
std::string	operator+= (      std::string &s, float  f);
std::string	operator+  (const std::string &s, float  f);
std::string	operator+= (      std::string &s, double d);
std::string	operator+  (const std::string &s, double d);

std::string	operator+  (bool   b, const std::string &s);
std::string	operator+  (int    i, const std::string &s);
std::string	operator+  (float  f, const std::string &s);
std::string	operator+  (double d, const std::string &s);
//ende von S. Scholz

char*		tbRemoveDir(char* pcFilename);						// Pfadangabe aus einem Dateinamen entfernen
char*		tbGetFilenameExtension(char* pcFilename);			// Liefert die Erweiterung einer Datei
bool		tbFileExists(char* pcFilename);						// Fragt ab, ob die Datei existiert

tbVector2	tbVector2TransformCoords(const tbVector2& v, const tbMatrix& m, float* const pfOutW = NULL);																																																					// 2D-Vektor mit Matrix multiplizieren
tbVector2	tbVector2TransformNormal(const tbVector2& v, const tbMatrix& m);																																																												// 2D-Normalenvektor mit Matrix multiplizieren
tbVector2	tbVector2TransformNormal_TranspInv(const tbVector2& v, const tbMatrix& m);																																																										// 2D-Normalenvektor mit Matrix multiplizieren
tbVector3	tbVector3TransformCoords(const tbVector3& v, const tbMatrix& m, float* const pfOutW = NULL);																																																					// 3D-Vektor mit Matrix multiplizieren
tbVector3	tbVector3TransformNormal(const tbVector3& v, const tbMatrix& m);																																																												// 3D-Normalenvektor mit Matrix multiplizieren
tbVector3	tbVector3TransformNormal_TranspInv(const tbVector3& v, const tbMatrix& m);																																																										// 3D-Normalenvektor mit Matrix multiplizieren
tbPlane		tbPlaneTransform(const tbPlane& p, const tbMatrix& m);																																																															// Ebene mit Matrix transformieren
tbVector3	tbComputeTriangleNormal(const tbVector3& v1, const tbVector3& v2, const tbVector3& v3);																																																							// Normalenvektor eines Dreiecks berechnen
tbResult	tbComputeAABoxPlanes(const tbVector3& vBoxMin, const tbVector3& vBoxMax, tbPlane* pOut);																																																						// Die sechs Ebenen eines achsenausgerichteten Quaders berechnen
tbResult	tbComputeBoxPlanes(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, tbPlane* pOut);																																														// Die sechs Ebenen eines frei drehbaren Quaders berechnen
tbResult	tbComputeClipPlanes(const tbMatrix& mView, const tbMatrix& mProjection, tbPlane* pOut);																																																							// View-Frustum anhand zweier Matrizen berechnen
float		tbPointPlaneDistance(const tbVector3& vPoint, const tbPlane& Plane);																																																											// Minimale Entfernung zwischen Punkt und Ebene berechnen
float		tbPointPlaneDistanceFast(const tbVector3& vPoint, const tbPlane& Plane);																																																										// Minimale Entfernung zwischen Punkt und Ebene berechnen (schnell)
float		tbPointLineDistance(const tbVector3& vPoint, const tbVector3& vLineA, const tbVector3& vLineB);																																																					// Minimale Entfernung zwischen Punkt und Linie berechnen
float		tbLineLineDistance(const tbVector3& vLine1A, const tbVector3& vLine1B, const tbVector3& vLine2A, const tbVector3& vLine2B);																																														// Minimale Entfernung zwischen zwei Linien berechnen
bool		tbPointHitsSphere(const tbVector3& vPoint, const tbVector3& vSphere, const float fRadius);																																																						// Liegt ein Punkt in einer Kugel?
bool		tbPointHitsAABox(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax);																																																					// Liegt ein Punkt in einem achsenausgerichteten Quader?
bool		tbPointHitsBox(const tbVector3& vPoint, const tbPlane* pBoxPlanes);																																																												// Liegt ein Punkt in einem frei drehbaren Quader?
bool		tbPointHitsBox(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation);																																												// Liegt ein Punkt in einem frei drehbaren Quader?
bool		tbPointHitsBox2(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation);																																												// Liegt ein Punkt in einem frei drehbaren Quader? Diese Funktion rechnet mit der invertierten Matrix.
bool		tbPointHitsBox2_Inv(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mInvBoxTransformation);																																										// Liegt ein Punkt in einem frei drehbaren Quader? Diese Funktion rechnet mit der invertierten Matrix.
bool		tbLineHitsPlane(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane, tbVector3* pvOut = NULL);																																																// Schnittpunkt zwischen einer Linie und einer Ebene berechnen
float		tbLineHitsPlaneS(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane);																																																						// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (Linienabschnitt)
bool		tbLineHitsPlaneFast(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane);																																																					// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
tbVector3	tbLineHitsPlaneFastEx(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane);																																																					// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
bool		tbLineHitsTriangle(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC, tbVector3* pvOut = NULL);																															// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
bool		tbLineHitsTriangle2(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC, tbVector3* pvOut = NULL, tbVector2* pvOutBaryCentric = NULL);																						// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
float		tbLineHitsTriangleS(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC);																																					// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen (Linienabschnitt)
bool		tbLineHitsSphere(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vSphere, const float fRadius, tbVector3* pvOut = NULL);																																										// Schnittpunkt zwischen einer Linie und einer Kugel berechnen
bool		tbLineHitsAABox(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vBoxMin, const tbVector3& vBoxMax, tbVector3* pvOut = NULL);																																									// Schnittpunkt zwischen einer Linie und einem achsenausgerichteten Quader berechnen
bool		tbLineHitsBox(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane* pBoxPlanes, tbVector3* pvOut = NULL);																																															// Schnittpunkt zwischen einer Linie und einem frei drehbaren Quader berechnen
bool		tbLineHitsBox(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, tbVector3* pvOut = NULL);																																// Schnittpunkt zwischen einer Linie und einem frei drehbaren Quader berechnen
bool		tbTriangleHitsPlane(const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC, tbPlane& Plane, tbVector3* pvHitStart, tbVector3* pvHitEnd);																																			// Schnittlinie zwischen einem Dreieck und einer Ebene berechnen
bool		tbTriangleHitsTriangle(const tbVector3& v1A, const tbVector3& v1B, const tbVector3& v1C, const tbVector3& v2A, const tbVector3& v2B, const tbVector3& v2C, tbVector3* pvHitStart, tbVector3* pvHitEnd);																											// Schnittlinie zweier Dreiecke berechnen
bool		tbSphereHitsPlane(const tbVector3& vSphere, const float fRadius, const tbPlane& Plane);																																																							// Kollision zwischen einer Kugel und einer Ebene berechnen
bool		tbSphereHitsSphere(const tbVector3& vSphereA, const float fRadiusA, const tbVector3& vSphereB, const float fRadiusB);																																															// Kollision zweier Kugeln berechnen
bool		tbSphereHitsAABox(const tbVector3& vSphere, const float fRadius, const tbVector3& vBoxMin, const tbVector3& vBoxMax);																																															// Schneiden sich Kugel und achsenausgerichteter Quader?
bool		tbSphereHitsBox(const tbVector3& vSphere, const float fRadius, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation);																																							// Schneiden sich Kugel und frei drehbarer Quader?
bool		tbBoxHitsBox(const tbVector3& vBox1Min, const tbVector3& vBox1Max, const tbMatrix& mBox1Transformation, const tbVector3& vBox2Min, const tbVector3& vBox2Max, const tbMatrix& mBox2Transformation, const int iNumSamples);																						// Kollision zwischen zwei Quadern berechnen
bool		tbBoxHitsBox2(const tbVector3& vBox1Min, const tbVector3& vBox1Max, const tbMatrix& mBox1Transformation, const tbVector3& vBox2Min, const tbVector3& vBox2Max, const tbMatrix& mBox2Transformation);																											// Kollision zwischen zwei Quadern berechnen (schnell)

//selbst

//ende

int			tbClassifySphere(const tbVector3& vSphere, const float fRadius, const tbPlane& Plane);																																																							// Kugel klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
int			tbClassifyBox(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, const tbPlane& Plane);																																													// Quader klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
int			tbClassifyBox_Inv(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mInvBoxTransformation, const tbPlane& Plane);																																												// Quader klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
int			tbClassifyAABox(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbPlane& Plane);																																																						// Achsenausgerichteten Quader klassifizieren
bool		tbPointVisible(const tbVector3& vPoint, const tbPlane* pClipPlanes);																																																											// Sichtbarkeit eines Punkts prfen
bool		tbSphereVisible(const tbVector3& vSphere, const float fRadius, const tbPlane* pClipPlanes);																																																						// Sichtbarkeit einer Kugel prfen
bool		tbAABoxVisible(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbPlane* pClipPlanes);																																																					// Sichtbarkeit eines achsenausgerichteten Quaders prfen
bool		tbBoxVisible(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, const tbPlane* pClipPlanes);																																												// Sichtbarkeit eines Quaders prfen
bool		tbBoxVisible_Inv(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mInvBoxTransformation, const tbPlane* pClipPlanes);																																										// Sichtbarkeit eines Quaders prfen

tbVector3	tbComputeAimingVector(const tbVector3& vOrigin, const tbVector3& vTargetPos, const tbVector3& vTargetVel, const float fProjectileSpeed);	// Berechnet den Zielvektor, um ein Objekt zu treffen


// ******************************************************************
// Template-Funktion fr lineare Interpolation
template <typename Type> Type tbInterpolateLinear(const Type& Value1,
												  const Type& Value2,
												  const float x)
{
	// Einfache lineare Interpolation
	return Value1 + x * (Value2 - Value1);
}

// ******************************************************************
// Template-Funktion fr Hermite-Interpolation
template <typename Type> Type tbInterpolateHermite(const Type& Value1,
												   const Type& Tangent1,
												   const Type& Value2,
												   const Type& Tangent2,
												   const float x)
{
	// Die Koeffizienten A und B berechnen
	const Type A(2.0f * Value1 - 2.0f * Value2 + Tangent1 + Tangent2);
	const Type B(3.0f * Value2 - 3.0f * Value1 - 2.0f * Tangent1 - Tangent2);

	// Interpolation durchfhren
	return A * (x * x * x) + B * (x * x) + Tangent1 * (x) + Value1;
}

// ******************************************************************
// Template-Funktion fr bilineare Interpolation
template <typename Type> Type tbInterpolateBilinear(const Type& A,
													const Type& B,
													const Type& C,
													const Type& D,
													const float x,
													const float y)
{
	// Die Hilfswerte P und Q berechnen
	const Type P(A + x * (B - A));
	const Type Q(C + x * (D - C));

	// Zwischen P und Q interpolieren
	return P + y * (Q - P);
}

// ******************************************************************

#endif /*TBUTILS_H_*/
