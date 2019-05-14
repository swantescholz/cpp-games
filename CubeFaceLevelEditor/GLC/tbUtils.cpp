#include "tbUtils.h"

/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`__\ /\ \ \ \  __\ /\ __\_ /\  __\   /__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbUtils.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Verschiedene Hilfsfunktionen

	Autor:
	David Scherfgen

********************************************************************/

//verwendete symbole vordefinieren
//class CModel;


// ******************************************************************
char* tbRemoveDir(char* pcFilename)
{
	int iLastBackSlash;
	int iChar;


	// Letzten Back-Slash ("\") suchen
	iLastBackSlash = -1;
	iChar = 0;
	while(pcFilename[iChar] != 0)
	{
		if(pcFilename[iChar] == '\\') iLastBackSlash = iChar;
		iChar++;
	}

	// String ab dem letzten Back-Slash plus 1 zurckliefern
	return pcFilename + iLastBackSlash + 1;
}

// ******************************************************************
// Liefert die Erweiterung einer Datei (z.B. "cpp")
char* tbGetFilenameExtension(char* pcFilename)
{
	int iLastDot;
	int iChar;


	// Letzten Punkt (".") im Dateinamen suchen
	iLastDot = -1;
	iChar = 0;
	while(pcFilename[iChar] != 0)
	{
		if(pcFilename[iChar] == '.') iLastDot = iChar;
		iChar++;
	}

	// String ab dem letzten Punkt plus 1 zurckliefern
	return pcFilename + iLastDot + 1;
}

// ******************************************************************
// Prft, ob eine Datei existiert
bool tbFileExists(char* pcFilename)
{
	// Parameter prfen
	if(!pcFilename) throw glcException("NULL-pointer in tbFileExists", 0);


	// Versuchen, die Datei zu ffnen
	FILE* pFile = fopen(pcFilename, "rb");
	if(pFile) fclose(pFile);

	// pFile = NULL: Datei existiert nicht (oder Fehler), ansonsten existiert sie.
	return pFile != NULL;
}

// ******************************************************************


// ******************************************************************
// 2D-Positionsvektor transformieren
tbVector2 tbVector2TransformCoords(const tbVector2& v,
											   const tbMatrix& m,
											   float* const pfOutW) // = NULL
{
	// Vektor mit Matrix transformieren
	tbVector2 vResult(v.x * m.m11 + v.y * m.m21 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + m.m42);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, mssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const float w = v.x * m.m14 + v.y * m.m24 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwnscht, w kopieren
	if(pfOutW) *pfOutW = w;

	return tbVector2Normalize(vResult) * tbVector2Length(v);
}

// ******************************************************************
// 2D-Richtungsvektor transformieren
tbVector2 tbVector2TransformNormal(const tbVector2& v,
											   const tbMatrix& m)
{
	// Vektorlnge berechnen
	const float fLength = tbVector2Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte inverse Matrix berechnen
	const tbMatrix mTransform(tbMatrixTranspose(tbMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprngliche Lnge wiederherstellen
	return tbVector2Normalize(tbVector2(v.x * mTransform.m11 + v.y * mTransform.m21,
		                                v.x * mTransform.m12 + v.y * mTransform.m22))
	       * fLength;
}

// ******************************************************************
// 2D-Richtungsvektor transformieren (transponierte invertierte Matrix)
tbVector2 tbVector2TransformNormal_TranspInv(const tbVector2& v,
														 const tbMatrix& m)
{
	// Vektorlnge berechnen
	const float fLength = tbVector2Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprngliche Lnge wiederherstellen
	return tbVector2Normalize(tbVector2(v.x * m.m11 + v.y * m.m21,
		                                v.x * m.m12 + v.y * m.m22))
	       * fLength;
}

// ******************************************************************
// 3D-Positionsvektor transformieren
tbVector3 tbVector3TransformCoords(const tbVector3& v,
											   const tbMatrix& m,
											   float* const pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	tbVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, mssen die anderen Vektorelemente durch sie geteilt
	// werden.
	const float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwnscht, w kopieren
	if(pfOutW) *pfOutW = w;

	return vResult;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren
tbVector3 tbVector3TransformNormal(const tbVector3& v,
											   const tbMatrix& m)
{
	// Vektorlnge berechnen
	const float fLength = tbVector3Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte invertierte Matrix berechnen
	const tbMatrix mTransform(tbMatrixTranspose(tbMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprngliche Lnge wiederherstellen
	return tbVector3Normalize(tbVector3(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                                v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
										v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33))
		   * fLength;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren (transponierte invertierte Matrix)
tbVector3 tbVector3TransformNormal_TranspInv(const tbVector3& v,
														 const tbMatrix& m)
{
	// Vektorlnge berechnen
	const float fLength = tbVector3Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprngliche Lnge wiederherstellen
	return tbVector3Normalize(tbVector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
		                                v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
										v.x * m.m13 + v.y * m.m23 + v.z * m.m33))
		   * fLength;
}

// ******************************************************************
// Ebene mit einer Matrix transformieren
tbPlane tbPlaneTransform(const tbPlane& p,
									 const tbMatrix& m)
{
	const float a = p.n.x * m.m11 + p.n.y * m.m21 + p.n.z * m.m31;
	const float b = p.n.x * m.m12 + p.n.y * m.m22 + p.n.z * m.m32;
	const float c = p.n.x * m.m13 + p.n.y * m.m23 + p.n.z * m.m33;

	return tbPlane(a, b, c,
				   p.d - (a * m.m41 + b * m.m42 + c * m.m43));
}

// ******************************************************************
// Normalenvektor eines Dreiecks berechnen
tbVector3 tbComputeTriangleNormal(const tbVector3& v1,
											  const tbVector3& v2,
											  const tbVector3& v3)
{
	// Normalisiertes Kreuzprodukt der Verbindungsvektoren berechnen
	return tbVector3Normalize(tbVector3Cross(v2 - v1, v3 - v1));
}

// ******************************************************************
// Berechnen der sechs Ebenen eines achsenausgerichteten Quaders
tbResult tbComputeAABoxPlanes(const tbVector3& vBoxMin,
										  const tbVector3& vBoxMax,
										  tbPlane* pOut)
{
	// Die sechs Ebenen berechnen, ohne die Drehung zu bercksichtigen
	pOut[0] = tbPlaneFromPointNormal(vBoxMin, tbVector3(-1.0f,  0.0f,  0.0f));	// Linke Ebene
	pOut[1] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 1.0f,  0.0f,  0.0f));	// Rechte Ebene
	pOut[2] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 0.0f,  1.0f,  0.0f));	// Obere Ebene
	pOut[3] = tbPlaneFromPointNormal(vBoxMin, tbVector3( 0.0f, -1.0f,  0.0f));	// Untere Ebene
	pOut[4] = tbPlaneFromPointNormal(vBoxMin, tbVector3( 0.0f,  0.0f, -1.0f));	// Vordere Ebene
	pOut[5] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 0.0f,  0.0f,  1.0f));	// Hintere Ebene

	return TB_OK;
}

// ******************************************************************
// Berechnen der sechs Ebenen eines frei drehbaren Quaders
tbResult tbComputeBoxPlanes(const tbVector3& vBoxMin,
										const tbVector3& vBoxMax,
										const tbMatrix& mBoxTransformation,
										tbPlane* pOut)
{
	// Die sechs Ebenen berechnen, ohne die Drehung zu bercksichtigen
	pOut[0] = tbPlaneFromPointNormal(vBoxMin, tbVector3(-1.0f,  0.0f,  0.0f));	// Linke Ebene
	pOut[1] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 1.0f,  0.0f,  0.0f));	// Rechte Ebene
	pOut[2] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 0.0f,  1.0f,  0.0f));	// Obere Ebene
	pOut[3] = tbPlaneFromPointNormal(vBoxMin, tbVector3( 0.0f, -1.0f,  0.0f));	// Untere Ebene
	pOut[4] = tbPlaneFromPointNormal(vBoxMin, tbVector3( 0.0f,  0.0f, -1.0f));	// Vordere Ebene
	pOut[5] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 0.0f,  0.0f,  1.0f));	// Hintere Ebene

	// Alle Ebenen transformieren
	pOut[0] = tbPlaneTransform(pOut[0], mBoxTransformation);
	pOut[1] = tbPlaneTransform(pOut[1], mBoxTransformation);
	pOut[2] = tbPlaneTransform(pOut[2], mBoxTransformation);
	pOut[3] = tbPlaneTransform(pOut[3], mBoxTransformation);
	pOut[4] = tbPlaneTransform(pOut[4], mBoxTransformation);
	pOut[5] = tbPlaneTransform(pOut[5], mBoxTransformation);

	return TB_OK;
}

// ******************************************************************
// Berechnen des View-Frustums anhand der Projektionsmatrix und der Sichtmatrix
tbResult tbComputeClipPlanes(const tbMatrix& mView,
										 const tbMatrix& mProjection,
										 tbPlane* pOut)
{
	// Sichtmatrix mit der Projektionsmatrix multiplizieren
	tbMatrix mTemp(mView * mProjection);

	// Linke Clippingebene
	pOut[0].n.x = -(mTemp.m14 + mTemp.m11);
	pOut[0].n.y = -(mTemp.m24 + mTemp.m21);
	pOut[0].n.z = -(mTemp.m34 + mTemp.m31);
	pOut[0].d = -(mTemp.m44 + mTemp.m41);
	pOut[0] = tbPlaneNormalize(pOut[0]);

	// Rechte Clippingebene
	pOut[1].n.x = -(mTemp.m14 - mTemp.m11);
	pOut[1].n.y = -(mTemp.m24 - mTemp.m21);
	pOut[1].n.z = -(mTemp.m34 - mTemp.m31);
	pOut[1].d = -(mTemp.m44 - mTemp.m41);
	pOut[1] = tbPlaneNormalize(pOut[1]);

	// Obere Clippingebene
	pOut[2].n.x = -(mTemp.m14 - mTemp.m12);
	pOut[2].n.y = -(mTemp.m24 - mTemp.m22);
	pOut[2].n.z = -(mTemp.m34 - mTemp.m32);
	pOut[2].d = -(mTemp.m44 - mTemp.m42);
	pOut[2] = tbPlaneNormalize(pOut[2]);

	// Untere Clippingebene
	pOut[3].n.x = -(mTemp.m14 + mTemp.m12);
	pOut[3].n.y = -(mTemp.m24 + mTemp.m22);
	pOut[3].n.z = -(mTemp.m34 + mTemp.m32);
	pOut[3].d = -(mTemp.m44 + mTemp.m42);
	pOut[3] = tbPlaneNormalize(pOut[3]);

	// Nahe Clippingebene
	pOut[4].n.x = -(mTemp.m14 + mTemp.m13);
	pOut[4].n.y = -(mTemp.m24 + mTemp.m23);
	pOut[4].n.z = -(mTemp.m34 + mTemp.m33);
	pOut[4].d = -(mTemp.m44 + mTemp.m43);
	pOut[4] = tbPlaneNormalize(pOut[4]);

	// Ferne Clippingebene
	pOut[5].n.x = -(mTemp.m14 - mTemp.m13);
	pOut[5].n.y = -(mTemp.m24 - mTemp.m23);
	pOut[5].n.z = -(mTemp.m34 - mTemp.m33);
	pOut[5].d = -(mTemp.m44 - mTemp.m43);
	pOut[5] = tbPlaneNormalize(pOut[5]);

	return TB_OK;
}

// ******************************************************************
// Minimale Entfernung zwischen Punkt und Ebene berechnen
float tbPointPlaneDistance(const tbVector3& vPoint,
									   const tbPlane& Plane)
{
	// Die Ebene normalisieren und dann den Punkt einsetzen
	return tbPlaneDotCoords(tbPlaneNormalize(Plane), vPoint);
}

// ******************************************************************
// Minimale Entfernung zwischen Punkt und Ebene berechnen (schnell)
float tbPointPlaneDistanceFast(const tbVector3& vPoint,
										   const tbPlane& Plane)
{
	// Den Punkt in die Gleichung einsetzen
	return Plane.n.x * vPoint.x + Plane.n.y * vPoint.y + Plane.n.z * vPoint.z + Plane.d;
}

// ******************************************************************
// Minimale Distanz zwischen Punkt und Linie berechnen
float tbPointLineDistance(const tbVector3& vPoint,
									  const tbVector3& vLineA,
									  const tbVector3& vLineB)
{
	tbVector3 v(vLineB - vLineA);
	tbVector3 w(vPoint - vLineA);

	return tbVector3Length(vLineA + v * (tbVector3Dot(w, v) / tbVector3LengthSq(v)) - vPoint);
}

// ******************************************************************
// Minimale Distanz zwischen zwei Linien berechnen
float tbLineLineDistance(const tbVector3& vLine1A,
									 const tbVector3& vLine1B,
									 const tbVector3& vLine2A,
									 const tbVector3& vLine2B)
{
	tbVector3	u(vLine1B - vLine1A);
    tbVector3	v(vLine2B - vLine1A);
	tbVector3	w(vLine1A - vLine2A);
	float		a = tbVector3LengthSq(u);
	float		c = tbVector3LengthSq(v);
	float		b = tbVector3Dot(u, v);
	float		d = tbVector3Dot(u, w);
	float		e = tbVector3Dot(v, w);
	float		D = a * c - b * b;


	if(D < 0.0001f)
	{
		float sc = 0.0f;
		float tc = b > c ? d / b : e / c;

		// Verbindungsvektor zwischen den zwei am nchsten liegenden
		// Punkten auf den Linien berechnen (L1(sc) - L2(tc))
		// Die Lnge dieses Vektors ist die minimale Distanz.
		return tbVector3Length(w + (u * sc) - (v * tc));
	}
	else
	{
		float sc = (b * e - c * d) / D;
		float tc = (a * e - b * d) / D;

		// Verbindungsvektor zwischen den zwei am nchsten liegenden
		// Punkten auf den Linien berechnen (L1(sc) - L2(tc))
		// Die Lnge dieses Vektors ist die minimale Distanz.
		return tbVector3Length(w + (u * sc) - (v * tc));
	}
}

// ******************************************************************
// Liegt ein Punkt innerhalb einer Kugel?
bool tbPointHitsSphere(const tbVector3& vPoint,
								   const tbVector3& vSphere,
								   const float fRadius)
{
	// Wenn die Distanz zwischen Punkt und dem Mittelpunkt der
	// Kugel kleiner gleich dem Radius ist, liegt der Punkt in
	// der Kugel.
	return tbVector3LengthSq(vPoint - vSphere) <= fRadius * fRadius;
}

// ******************************************************************
// Liegt ein Punkt in einem achsenausgerichteten Quader?
bool tbPointHitsAABox(const tbVector3& vPoint,
								  const tbVector3& vBoxMin,
								  const tbVector3& vBoxMax)
{
	return vPoint.x >= vBoxMin.x && vPoint.x <= vBoxMax.x &&
		   vPoint.y >= vBoxMin.y && vPoint.y <= vBoxMax.y &&
		   vPoint.z >= vBoxMin.z && vPoint.z <= vBoxMax.z;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
bool tbPointHitsBox(const tbVector3& vPoint,
								const tbPlane* pBoxPlanes)
{
	// Sobald der Punkt auf der Vorderseite auch nur einer einzigen der sechs
	// Ebenen des Quaders liegt, liegt er nicht mehr innerhalb des Quaders.
	// Es gibt eine kleine Toleranzgrenze.
	if(tbPlaneDotCoords(pBoxPlanes[0], vPoint) > 0.0001f) return false;
	if(tbPlaneDotCoords(pBoxPlanes[1], vPoint) > 0.0001f) return false;
	if(tbPlaneDotCoords(pBoxPlanes[2], vPoint) > 0.0001f) return false;
	if(tbPlaneDotCoords(pBoxPlanes[3], vPoint) > 0.0001f) return false;
	if(tbPlaneDotCoords(pBoxPlanes[4], vPoint) > 0.0001f) return false;
	return tbPlaneDotCoords(pBoxPlanes[5], vPoint) <= 0.0001f;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
bool tbPointHitsBox(const tbVector3& vPoint,
								const tbVector3& vBoxMin,
								const tbVector3& vBoxMax,
								const tbMatrix& mBoxTransformation)
{
	tbPlane aBoxPlane[6];

	// Die sechs Ebenen berechnen
	tbComputeBoxPlanes(vBoxMin, vBoxMax, mBoxTransformation, aBoxPlane);

	// Die andere Funktion aufrufen
	return tbPointHitsBox(vPoint, aBoxPlane);
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
// Diese Funktion verwendet die invertierte Matrix, um das herauszufinden.
bool tbPointHitsBox2(const tbVector3& vPoint,
								 const tbVector3& vBoxMin,
								 const tbVector3& vBoxMax,
								 const tbMatrix& mBoxTransformation)
{
	// Punkt mit der invertierten Matrix transformieren
	const tbVector3 vNewPoint(tbVector3TransformCoords(vPoint, tbMatrixInvert(mBoxTransformation)));

	// Liegt der Punkt im Quader?
	return vNewPoint.x >= vBoxMin.x && vNewPoint.x <= vBoxMax.x &&
		   vNewPoint.y >= vBoxMin.y && vNewPoint.y <= vBoxMax.y &&
		   vNewPoint.z >= vBoxMin.z && vNewPoint.z <= vBoxMax.z;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
// Diese Funktion verwendet die invertierte Matrix, um das herauszufinden.
bool tbPointHitsBox2_Inv(const tbVector3& vPoint,
									 const tbVector3& vBoxMin,
									 const tbVector3& vBoxMax,
									 const tbMatrix& mInvBoxTransformation)
{
	// Punkt mit der invertierten Matrix transformieren
	tbVector3 vNewPoint(tbVector3TransformCoords(vPoint, mInvBoxTransformation));

	// Liegt der Punkt im Quader?
	return vNewPoint.x >= vBoxMin.x && vNewPoint.x <= vBoxMax.x &&
		   vNewPoint.y >= vBoxMin.y && vNewPoint.y <= vBoxMax.y &&
		   vNewPoint.z >= vBoxMin.z && vNewPoint.z <= vBoxMax.z;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen
bool tbLineHitsPlane(const tbVector3& vLineA,
								 const tbVector3& vLineB,
								 const tbPlane& Plane,
								 tbVector3* pvOut) // = NULL
{
	// Richtungsvektor der Linie berechnen
	const tbVector3 vLineDir(vLineB - vLineA);

	// Nenner des Bruchs fr s berechnen. Falls dieser gleich null ist, sind Linie
	// und Ebene parallel und es gibt entweder keinen oder unendlich viele Schnittpunkte.
	// Der Nenner ist das Punktprodukt aus Linienrichtung und Normalenvektor der Ebenen.
	// Kommt hier ein sehr kleiner Wert heraus, dann sind Linie und Ebene parallel.
	const float fDenominator = tbVector3Dot(Plane.n, vLineDir);

	if(fabsf(fDenominator) < 0.0001f)
	{
		// Linie und Ebene sind parallel!
		// Liegt der Startpunkt der Strecke in der Ebene? Wenn ja, dann liegt die
		// gesamte Strecke in ihr. Als Kollisionsort nehmen wir dann den Startpunkt.
		if(fabsf(tbVector3Dot(Plane.n, vLineA) + Plane.d) < 0.0001f)
		{
			// Strecke liegt komplett auf der Ebene!
			if(pvOut) *pvOut = vLineA;
			return true;
		}
		else
		{
			// Linie und Ebene sind parallel - keine Kollision!
			return false;
		}
	}

	// Streckenabschnitt des Schnittpunkts des Strahls berechnen
	const float s = -(tbVector3Dot(Plane.n, vLineA) + Plane.d) / fDenominator;

	// Liegt der Streckenabschnitt auerhalb von [0; 1], dann gibt es keinen Schnittpunkt,
	// da der Punkt auerhalb der Linie liegt.
	if(s < 0.0f || s > 1.0f) return false;

	// Es gibt einen Schnittpunkt!
	if(pvOut) *pvOut = vLineA + (s * vLineDir);
	return true;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (Linienabschnitt)
float tbLineHitsPlaneS(const tbVector3& vLineA,
								   const tbVector3& vLineB,
								   const tbPlane& Plane)
{
	// Richtungsvektor der Linie berechnen
	const tbVector3 vLineDir(vLineB - vLineA);

	// Nenner des Bruchs fr s berechnen. Falls dieser gleich null ist, sind Linie
	// und Ebene parallel und es gibt entweder keinen oder unendlich viele Schnittpunkte.
	const float fDenominator = tbVector3Dot(Plane.n, vLineDir);
	if(fabsf(fDenominator) < 0.0001f) return -1.0f;

	// Streckenabschnitt des Schnittpunkts des Strahls berechnen
	const float s = -(tbVector3Dot(Plane.n, vLineA) + Plane.d) / fDenominator;

	// Liegt der Streckenabschnitt auerhalb von [0; 1], dann gibt es keinen Schnittpunkt,
	// da der Punkt auerhalb der Linie liegt.
	if(s < 0.0f || s > 1.0f) return -1.0f;

	// Es gibt einen Schnittpunkt!
	return s;
}

// ******************************************************************
// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
bool tbLineHitsPlaneFast(const tbVector3& vLineA,
									 const tbVector3& vLineB,
									 const tbPlane& Plane)
{
	// Es wird kein exakter Schnittpunkt verlangt - daher kann eine schnellere
	// Methode angewandt werden.

	// Punktprodukte zwischen den Linienpunkten und der Ebene bilden
	const float d1 = tbPlaneDotCoords(Plane, vLineA);
	const float d2 = tbPlaneDotCoords(Plane, vLineB);

	// Liegen die beiden Linienpunkte auf verschiedenen Seiten oder beide
	// direkt auf der Ebene? Falls ja, gibt es einen Schnittpunkt.
	if(d1 <= 0.0f && d2 >= 0.0f) return true;
	if(d1 >= 0.0f && d2 <= 0.0f) return true;

	// Kein Schnittpunkt!
	return false;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (schnell)
tbVector3 tbLineHitsPlaneFastEx(const tbVector3& vLineA,
											const tbVector3& vLineB,
											const tbPlane& Plane)
{
	// Richtungsvektor der Linie berechnen
	const tbVector3 vLineDir(vLineB - vLineA);

	// Schnittpunkt liefern
	return   vLineA
	       + (  (-Plane.n.x * vLineA.x - Plane.n.y * vLineA.y - Plane.n.z * vLineA.z - Plane.d)
		      / (Plane.n.x * vLineDir.x + Plane.n.y * vLineDir.y + Plane.n.z * vLineDir.z))
		   * vLineDir;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
bool tbLineHitsTriangle(const tbVector3& vLineA,
									const tbVector3& vLineB,
									const tbVector3& vTriangleA,
									const tbVector3& vTriangleB,
									const tbVector3& vTriangleC,
									tbVector3* pvOut) // = NULL
{
	// Die Ebene dieses Dreiecks berechnen
	tbPlane Plane(tbPlaneFromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schnittpunkt der Linie mit der Ebene bestimmen
	tbVector3 vIntersection;
	if(!tbLineHitsPlane(vLineA, vLineB, Plane, &vIntersection))
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return false;
	}

	// Erstellen der ersten Ebene entlang den Punkten A und B und
	// Einsetzen des Punkts in die Gleichung. Falls das Ergebnis kleiner
	// als null ist, liegt der Punkt hinter der Ebene und damit nicht im
	// Dreieck.
	tbVector3 vNormal(Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleA, tbVector3Cross(vTriangleA - vTriangleB, vNormal));
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return false;

	// Test mit der zweiten Ebene entlang den Punkten B und C
	Plane = tbPlaneFromPointNormal(vTriangleB, tbVector3Cross(vTriangleB - vTriangleC, vNormal));
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return false;

	// Test mit der dritten Ebene entlang den Punkten C und A
	Plane = tbPlaneFromPointNormal(vTriangleC, tbVector3Cross(vTriangleC - vTriangleA, vNormal));
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return false;

	// Der Punkt liegt im Dreieck, da er vor allen drei Ebenen liegt!
	// Schnittpunkt einsetzen.
	if(pvOut) *pvOut = vIntersection;

	return true;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
bool tbLineHitsTriangle2(const tbVector3& vLineA,
									 const tbVector3& vLineB,
									 const tbVector3& vTriangleA,
									 const tbVector3& vTriangleB,
									 const tbVector3& vTriangleC,
									 tbVector3* pvOut,				// = NULL
									 tbVector2* pvOutBaryCentric)	// = NULL
{
	// Die Ebene dieses Dreiecks berechnen
	const tbPlane Plane(tbPlaneFromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schnittpunkt der Linie mit der Ebene bestimmen
	tbVector3 vIntersection;
	if(!tbLineHitsPlane(vLineA, vLineB, Plane, &vIntersection))
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return false;
	}

	// q, u und v berechnen
	const tbVector3 q(vIntersection - vTriangleA);
	const tbVector3 u(vTriangleB - vTriangleA);
	const tbVector3 v(vTriangleC - vTriangleA);

	// v' berechnen
	tbVector3 vTemp(tbVector3Cross(Plane.n, v));

	// Den s-Wert berechnen
	const float s = tbVector3Dot(q, vTemp) / tbVector3Dot(u, vTemp);

	// Wenn s kleiner als null ist, dann kann der Punkt nicht im Dreieck liegen.
	if(s < 0.0f) return false;

	// u' und t berechnen
	vTemp = tbVector3Cross(Plane.n, u);
	const float t = tbVector3Dot(q, vTemp) / tbVector3Dot(v, vTemp);
	if(t < 0.0f) return false;

	// Testen, ob die Summe kleiner grer als 1 ist
	if(s + t > 1.0f) return false;

	// Der Punkt liegt im Dreieck! Auf Wunsch tragen wir die Koordinaten ein.
	if(pvOut) *pvOut = vIntersection;
	if(pvOutBaryCentric) *pvOutBaryCentric = tbVector2(s, t);

	return true;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen (Linienabschnitt)
float tbLineHitsTriangleS(const tbVector3& vLineA,
									  const tbVector3& vLineB,
									  const tbVector3& vTriangleA,
									  const tbVector3& vTriangleB,
									  const tbVector3& vTriangleC)
{
	// Die Ebene dieses Dreiecks berechnen
	tbPlane Plane(tbPlaneFromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schneidet die Linie die Ebene nicht?
	float fIntersection = tbLineHitsPlaneS(vLineA, vLineB, Plane);
	if(fIntersection < 0.0f)
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return -1.0f;
	}

	// Schnittpunkt berechnen
	tbVector3 vIntersection(vLineA + (vLineB - vLineA) * fIntersection);

	// Erstellen der ersten Ebene entlang den Punkten A und B und
	// Einsetzen des Punkts in die Gleichung. Falls das Ergebnis kleiner
	// als null ist, liegt der Punkt hinter der Ebene und damit nicht im
	// Dreieck.
	tbVector3 vTemp(tbVector3Cross(vTriangleA - vTriangleB, Plane.n));
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Test mit der zweiten Ebene entlang den Punkten B und C
	vTemp = tbVector3Cross(vTriangleB - vTriangleC, Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Test mit der dritten Ebene entlang den Punkten C und A
	vTemp = tbVector3Cross(vTriangleC - vTriangleA, Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Der Punkt liegt im Dreieck, da er vor allen drei Ebenen liegt!
	// Schnittstelle liefern.
	return fIntersection;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Kugel berechnen
bool tbLineHitsSphere(const tbVector3& vLineA,
								  const tbVector3& vLineB,
								  const tbVector3& vSphereCenter,
								  const float fRadius,
								  tbVector3* pvOut) // = NULL
{
	// r߲ vorberechnen
	const float fRadiusSq = fRadius * fRadius;

	// (p - m) vorberechnen
	const tbVector3 vPMinusM(vLineA - vSphereCenter);

	// Wenn der Startpunkt der Linie schon in der Kugel liegt,
	// dann brechen wir sofort ab.
	if(tbVector3LengthSq(vPMinusM) <= fRadiusSq)
	{
		// Als Schnittpunkt nehmen wir einfach den Startpunkt der Linie.
		if(pvOut) *pvOut = vLineA;
		return true;
	}

	// Richtungsvektor der Linie berechnen (u)
	const tbVector3 vLineDir(vLineB - vLineA);

	// u * (p - m) vorberechnen
	const float fUDotPMinusM = tbVector3Dot(vLineDir, vPMinusM);

	// u vorberechnen
	const float fLineDirSq = tbVector3LengthSq(vLineDir);

	// Die Diskriminante berechnen:
	//   (u * (p - m))
	// - (u * ((p - m) - r)) 
	const float d =   fUDotPMinusM * fUDotPMinusM
					- fLineDirSq * (tbVector3LengthSq(vPMinusM) - fRadiusSq);

	// Ist die Diskriminante negativ, null oder positiv?
	if(d < 0.0f) return false;
	else if(d < 0.0001f)
	{
		// Die Diskriminante ist (ungefhr) null.
		// Die gesamte Wurzel entfllt und die Lsung ist (-u * (p - m)) / u.
		// Wir mssen nur noch prfen, ob der Wert im korrekten Bereich [0; 1] liegt.
		const float s = -fUDotPMinusM / fLineDirSq;
		if(s < 0.0f || s > 1.0f) return false;
		else
		{
			// Berhrpunkt!
			if(pvOut) *pvOut = vLineA + s * vLineDir;
			return true;
		}
	}
	else
	{
		// Die Gerade schneidet den Kreis zweimal.
		// Uns interessiert nur die kleinere Lsung fr s, denn das ist die Stelle,
		// wo die Linie den Kreis "zum ersten Mal" schneidet.
		// Diese Lsung berechnen wir nun.
		const float s = (-fUDotPMinusM - sqrtf(d)) / fLineDirSq;
		if(s < 0.0f || s > 1.0f) return false;
		else
		{
			if(pvOut) *pvOut = vLineA + s * vLineDir;
			return true;
		}
	}
}

// ******************************************************************
// Schnittpunkt zwischen Linie und achsenausgerichtetem Quader berechnen
bool tbLineHitsAABox(const tbVector3& vLineA,
								 const tbVector3& vLineB,
								 const tbVector3& vBoxMin,
								 const tbVector3& vBoxMax,
								 tbVector3* pvOut) // = NULL
{
	tbPlane aBoxPlane[6];

	// Die sechs Ebenen des Quaders berechnen
	tbComputeAABoxPlanes(vBoxMin, vBoxMax, aBoxPlane);

	// Die andere Funktion aufrufen
	return tbLineHitsBox(vLineA, vLineB, aBoxPlane, pvOut);
}

// ******************************************************************
// Schnittpunkt zwischen Linie und frei drehbarem Quader berechnen
bool tbLineHitsBox(const tbVector3& vLineA,
							   const tbVector3& vLineB,
							   const tbPlane* pBoxPlanes,
							   tbVector3* pvOut) // = NULL
{
	// Wenn der Linienstartpunkt im Quader liegt, setzen wir
	// den Schnittpunkt einfach auf den Linienstartpunkt.
	if(tbPointHitsBox(vLineA, pBoxPlanes))
	{
		if(pvOut) *pvOut = vLineA;
		return true;
	}

	// Linienrichtung berechnen
	const tbVector3 vLineDir(vLineB - vLineA);

	float		fNearestIntersection = 100000.0f;
	float		fIntersection;
	tbVector3	vIntersection;			// Schnittpunktkoordinaten
	tbVector3	vNearestIntersection;	// Koordinaten des nchsten Schnittpunkts

	// Jede Ebene durchgehen und prfen, ob sie von der Linie geschnitten wird.
	// Es wird der nchste Schnittpunkt (niedrigster Linienabschnitt) gesucht.
	for(int iPlane = 0; iPlane < 6; iPlane++)
	{
		fIntersection = tbLineHitsPlaneS(vLineA, vLineB, pBoxPlanes[iPlane]);
		if(fIntersection >= 0.0f)
		{
			// Die Ebene wurde von der Linie geschnitten!
			// Wenn der Linienabschnitt des Schnittpunkts kleiner ist als
			// der aktuelle Minimumwert, dann rechnen wir weiter. Ansonsten
			// lassen wir es sein, da dieser Schnittpunkt so wie so nicht
			// der jenige mit der kleinsten Entfernung zum Linienstartpunkt
			// sein kann.
			if(fIntersection < fNearestIntersection)
			{
				// Dieser Schnittpunkt ist ein guter Kandidat!
				// Nun prfen wir, ob der Schnittpunkt im Quader liegt.
				vIntersection = vLineA + vLineDir * fIntersection;
				if(tbPointHitsBox(vIntersection, pBoxPlanes))
				{
					// Aha! Der Schnittpunkt liegt im Quader.
					// Damit haben wir einen neuen nchsten Schnittpunkt gefunden.
					// Wenn der Schnittpunkt unwichtig ist, knnen wir hier abbrechen.
					if(!pvOut) return true;

					// Der Schnittpunkt ist erwnscht!
					// Wir speichern seine Koordinaten und seinen Linienabschnitt.
					vNearestIntersection = vIntersection;
					fNearestIntersection = fIntersection;
				}
			}
		}
	}

	// Wenn der nchste Schnittpunkt immer noch 100000 ist (so wurde er am Anfang
	// gesetzt), dann gab es keine Kollision.
	if(fNearestIntersection == 100000.0f) return false;

	// Ansonsten liefern wir nun die Koordinaten des am nchsten liegenden
	// Schnittpunkts zurck.
	if(pvOut) *pvOut = vNearestIntersection;

	return true;
}

// ******************************************************************
// Schnittpunkt zwischen Linie und frei drehbarem Quader berechnen
bool tbLineHitsBox(const tbVector3& vLineA,
							   const tbVector3& vLineB,
							   const tbVector3& vBoxMin,
							   const tbVector3& vBoxMax,
							   const tbMatrix& mBoxTransformation,
							   tbVector3* pvOut) // = NULL
{
	tbPlane aBoxPlane[6];

	// Die sechs Ebenen des Quaders berechnen
	tbComputeBoxPlanes(vBoxMin, vBoxMax, mBoxTransformation, aBoxPlane);

	// Die andere Funktion aufrufen
	return tbLineHitsBox(vLineA, vLineB, aBoxPlane, pvOut);
}

// ******************************************************************
//todo: modelle

// ******************************************************************
// Schnittlinie zwischen einem Dreieck und einer Ebene berechnen
bool tbTriangleHitsPlane(const tbVector3& vTriangleA,
									 const tbVector3& vTriangleB,
									 const tbVector3& vTriangleC,
									 tbPlane& Plane,
									 tbVector3* pvHitStart,
									 tbVector3* pvHitEnd)
{
	// Punkte des Dreiecks in die Ebenengleichung einsetzen
	float fDotA = tbPlaneDotCoords(Plane, vTriangleA);
	float fDotB = tbPlaneDotCoords(Plane, vTriangleB);
	float fDotC = tbPlaneDotCoords(Plane, vTriangleC);

	// Wenn alle Punkte auf der gleichen Seite liegen, gibt es keine Schnittlinie.
	if(fDotA > 0.0f) if(fDotB > 0.0f) if(fDotC > 0.0f) return false;
	if(fDotA < 0.0f) if(fDotB < 0.0f) if(fDotC < 0.0f) return false;

	// Den Punkt finden, der auf seiner Ebenenseite alleine ist
	int iTop = 0;
	if(fDotB <= 0.0f && fDotC <= 0.0f) iTop = 1;
	else if(fDotA >= 0.0f && fDotB >= 0.0f) iTop = 3;
	else if(fDotA >= 0.0f && fDotC >= 0.0f) iTop = 2;
	else if(fDotB >= 0.0f && fDotC >= 0.0f) iTop = 1;
	else if(fDotA <= 0.0f && fDotB <= 0.0f) iTop = 3;
	else iTop = 2;

	// Schnittpunkte zwischen der Linie vom einzelnen Punkt zu den beiden anderen
	// und der Ebene berechnen
	switch(iTop)
	{
	case 1: *pvHitStart = tbLineHitsPlaneFastEx(vTriangleA, vTriangleB, Plane);
			*pvHitEnd = tbLineHitsPlaneFastEx(vTriangleA, vTriangleC, Plane);
			return true;

	case 2: *pvHitStart = tbLineHitsPlaneFastEx(vTriangleB, vTriangleA, Plane);
			*pvHitEnd = tbLineHitsPlaneFastEx(vTriangleB, vTriangleC, Plane);
			return true;
	
	case 3: *pvHitStart = tbLineHitsPlaneFastEx(vTriangleC, vTriangleA, Plane);
			*pvHitEnd = tbLineHitsPlaneFastEx(vTriangleC, vTriangleB, Plane);
			return true;
	}

	// Das Dreieck schneidet die Ebene nicht!
	return false;
}

// ******************************************************************
// Schnittlinie zweier Dreiecke berechnen
bool tbTriangleHitsTriangle(const tbVector3& v1A,
										const tbVector3& v1B,
										const tbVector3& v1C,
										const tbVector3& v2A,
										const tbVector3& v2B,
										const tbVector3& v2C,
										tbVector3* pvHitStart,
										tbVector3* pvHitEnd)
{
	// Ebene des zweiten Dreiecks berechnen
	tbPlane Plane2(tbPlaneFromPoints(v2A, v2B, v2C));

	// Alle Punkte des ersten Dreiecks in die Ebenengleichung einsetzen
	float fDot1A = v1A.x * Plane2.n.x + v1A.y * Plane2.n.y + v1A.z * Plane2.n.z + Plane2.d;
	float fDot1B = v1B.x * Plane2.n.x + v1B.y * Plane2.n.y + v1B.z * Plane2.n.z + Plane2.d;
	float fDot1C = v1C.x * Plane2.n.x + v1C.y * Plane2.n.y + v1C.z * Plane2.n.z + Plane2.d;

	// Liegen alle (ungefhr) auf der gleichen Seite?
	// Wenn ja, dann knnen wir abbrechen!
	if(fDot1A >  0.0001f && fDot1B >  0.0001f && fDot1C >  0.0001f) return false;
	if(fDot1A < -0.0001f && fDot1B < -0.0001f && fDot1C < -0.0001f) return false;

	// Nun tun wir das gleiche mit dem anderen Dreieck.
	tbPlane Plane1(tbPlaneFromPoints(v1A, v1B, v1C));
	float fDot2A = v2A.x * Plane1.n.x + v2A.y * Plane1.n.y + v2A.z * Plane1.n.z + Plane1.d;
	float fDot2B = v2B.x * Plane1.n.x + v2B.y * Plane1.n.y + v2B.z * Plane1.n.z + Plane1.d;
	float fDot2C = v2C.x * Plane1.n.x + v2C.y * Plane1.n.y + v2C.z * Plane1.n.z + Plane1.d;
	if(fDot2A >  0.0001f && fDot2B >  0.0001f && fDot2C >  0.0001f) return false;
	if(fDot2A < -0.0001f && fDot2B < -0.0001f && fDot2C < -0.0001f) return false;

	// Den alleine liegenden Punkt beider Dreiecke bestimmen
	int iTop1 = 0;
	int iTop2 = 0;
	if(fDot2B <= -0.0001f && fDot2C <= -0.0001f) iTop1 = 1;
	else if(fDot2A >= 0.0001f && fDot2B >= 0.0001f) iTop1 = 3;
	else if(fDot2A >= 0.0001f && fDot2C >= 0.0001f) iTop1 = 2;
	else if(fDot2B >= 0.0001f && fDot2C >= 0.0001f) iTop1 = 1;
	else if(fDot2A <= -0.0001f && fDot2B <= -0.0001f) iTop1 = 3;
	else iTop1 = 2;
	if(fDot1B <= -0.0001f && fDot1C <= -0.0001f) iTop2 = 1;
	else if(fDot1A >= 0.0001f && fDot1B >= 0.0001f) iTop2 = 3;
	else if(fDot1A >= 0.0001f && fDot1C >= 0.0001f) iTop2 = 2;
	else if(fDot1B >= 0.0001f && fDot1C >= 0.0001f) iTop2 = 1;
	else if(fDot1A <= -0.0001f && fDot1B <= -0.0001f) iTop2 = 3;
	else iTop2 = 2;

	// L1 berechnen
	tbVector3 vL1A; // Startpunkt von L1
	tbVector3 vL1B; // Endpunkt von L1

	// Nun kommt es darauf an, welcher Punkt alleine liegt!
	switch(iTop1)
	{
	case 1: vL1A = tbLineHitsPlaneFastEx(v1A, v1B, Plane2);
			vL1B = tbLineHitsPlaneFastEx(v1A, v1C, Plane2);
			break;

	case 2: vL1A = tbLineHitsPlaneFastEx(v1B, v1A, Plane2);
			vL1B = tbLineHitsPlaneFastEx(v1B, v1C, Plane2);
			break;

	default: vL1A = tbLineHitsPlaneFastEx(v1C, v1A, Plane2);
			 vL1B = tbLineHitsPlaneFastEx(v1C, v1B, Plane2);
			 break;
	}

	// L2 berechnen
	tbVector3 vL2A; // Startpunkt von L2
	tbVector3 vL2B; // Endpunkt von L2

	switch(iTop2)
	{
	case 1: vL2A = tbLineHitsPlaneFastEx(v2A, v2B, Plane1);
			vL2B = tbLineHitsPlaneFastEx(v2A, v2C, Plane1);
			break;

	case 2: vL2A = tbLineHitsPlaneFastEx(v2B, v2A, Plane1);
			vL2B = tbLineHitsPlaneFastEx(v2B, v2C, Plane1);
			break;

	default: vL2A = tbLineHitsPlaneFastEx(v2C, v2A, Plane1);
			 vL2B = tbLineHitsPlaneFastEx(v2C, v2B, Plane1);
			 break;
	}

	// Richtungsvektor der Geraden berechnen
	const tbVector3 vDir(vL1B - vL1A);

	// Quadrat der Lnge des Richtungsvektors vorberechnen
	const float fDirSq = tbVector3LengthSq(vDir);

	// Jetzt berechnen wir die s-Werte von L2A und L2B.
	// Der s-Wert von L1A ist in jedem Fall null,
	// und der von L1B ist in jedem Fall eins.
	float fL2A = tbVector3Dot(vDir, vL2A - vL1A) / fDirSq;
	float fL2B = tbVector3Dot(vDir, vL2B - vL1A) / fDirSq;

	// fL2B soll der grere der beiden Werte sein.
	// Wenn das nicht so ist: tauschen!
	if(fL2B < fL2A) {const float fTemp = fL2A; fL2A = fL2B; fL2B = fTemp;}

	// berlappen sich die beiden Schnittlinien?
	if(fL2A > 1.0f)
	{
		// fL2B muss jetzt ebenfalls grer als 1 sein, da fL2B > fL2A.
		// Fall 1 - keine Kollision!
		return false;
	}
	else if(fL2B < 0.0f)
	{
		// fL2A muss jetzt ebenfalls kleiner als null sein, da fL2B > fL2A.
		// Es liegt Fall 2 vor - keine Kollision!
		return false;
	}
	else if(fL2A <= 1.0f && fL2B >= 1.0f && fL2A >= 0.0f)
	{
		// Fall 3!
		if(pvHitStart) *pvHitStart = vL2A;
		if(pvHitEnd) *pvHitEnd = vL1B;
		return true;
	}
	else if(fL2A <= 0.0f && fL2B >= 0.0f && fL2B <= 1.0f)
	{
		// Fall 4!
		if(pvHitStart) *pvHitStart = vL1A;
		if(pvHitEnd) *pvHitEnd = vL2B;
		return true;
	}
	else if(fL2A <= 0.0f && fL2B >= 1.0f)
	{
		// Fall 5!
		if(pvHitStart) *pvHitStart = vL1A;
		if(pvHitEnd) *pvHitEnd = vL1B;
		return true;
	}
	else if(fL2A >= 0.0f && fL2B <= 1.0f)
	{
		// Fall 6!
		if(pvHitStart) *pvHitStart = vL2A;
		if(pvHitEnd) *pvHitEnd = vL2B;
	}
	else
	{
		// Hierhin sollte das Programm eigentlich nie gelangen!
		throw glcException("Unerwartete Situation!", 0);
	}

	// Keine Kollision!
	return false;
}

// ******************************************************************
// Kollision zwischen einer Kugel und einer Ebene berechnen
bool tbSphereHitsPlane(const tbVector3& vSphere,
								   const float fRadius,
								   const tbPlane& Plane)
{
	// Wenn der Abstand des Kugelmittelpunkts zur Ebene kleiner oder gleich
	// dem Radius der Kugel ist, dann gibt es eine Kollision.
	return fabsf(tbPointPlaneDistance(vSphere, Plane)) <= fRadius;
}

// ******************************************************************
// Kollision zweier Kugeln berechnen
bool tbSphereHitsSphere(const tbVector3& vSphereA,
									const float fRadiusA,
									const tbVector3& vSphereB,
									const float fRadiusB)
{
	// Quadrat der Entfernung berechnen und entscheiden, ob eine Kollision vorliegt
	return tbVector3LengthSq(vSphereA - vSphereB) <= (fRadiusA + fRadiusB) * (fRadiusA + fRadiusB);
}

// ******************************************************************
// Schneiden sich Kugel und achsenausgerichteter Quader?
bool tbSphereHitsAABox(const tbVector3& vSphere,
								   const float fRadius,
								   const tbVector3& vBoxMin,
								   const tbVector3& vBoxMax)
{
	float s, d = 0.0f;
	for(int i = 0; i < 3; i++)
	{
		if(vSphere.c[i] < vBoxMin.c[i])
		{
			s = vSphere.c[i] - vBoxMin.c[i];
			d += s * s;
		}
		else if(vSphere.c[i] > vBoxMax.c[i])
		{
			s = vSphere.c[i] - vBoxMax.c[i];
			d += s * s;
		}
	}

	return d <= fRadius * fRadius;
}

// ******************************************************************
// Schneiden sich Kugel und frei drehbarer Quader?
bool tbSphereHitsBox(const tbVector3& vSphere,
								 const float fRadius,
								 const tbVector3& vBoxMin,
								 const tbVector3& vBoxMax,
								 const tbMatrix& mBoxTransformation)
{
	return tbSphereHitsAABox(tbVector3TransformCoords(vSphere, tbMatrixInvert(mBoxTransformation)),
							 fRadius, vBoxMin, vBoxMax);
}

// ******************************************************************
// Kollision zwischen zwei Quadern berechnen
bool tbBoxHitsBox(const tbVector3& vBox1Min,
							  const tbVector3& vBox1Max,
							  const tbMatrix& mBox1Transformation,
							  const tbVector3& vBox2Min,
							  const tbVector3& vBox2Max,
							  const tbMatrix& mBox2Transformation,
							  const int iNumSamples)
{
	tbPlane aBoxPlanes[6];	// Die sechs Ebenen

	// Die Ebenen des zweiten Quaders berechnen
	tbComputeBoxPlanes(vBox2Min, vBox2Max, mBox2Transformation, aBoxPlanes);

	// Schrittweiten berechnen
	float fStepX = (vBox1Max.x - vBox1Min.x) / (float)(iNumSamples);
	float fStepY = (vBox1Max.y - vBox1Min.y) / (float)(iNumSamples);
	float fStepZ = (vBox1Max.z - vBox1Min.z) / (float)(iNumSamples);

	// Am Anfang ist der zu testende Punkt beim Minimum des ersten Quaders.
	tbVector3 vPoint(vBox1Min);

	// Den ersten Quader durchlaufen
	for(int x = 0; x < iNumSamples; x++)
	{
		// y- und z-Koordinate zurcksetzen
		vPoint.y = vBox1Min.y;
		vPoint.z = vBox1Min.z;
		for(int y = 0; y < iNumSamples; y++)
		{
			// z-Koordinate zurcksetzen
			vPoint.z = vBox1Min.z;
			for(int z = 0; z < iNumSamples; z++)
			{
				// Wenn sich der aktuelle Punkt transformiert innerhalb von
				// Quader 2 befindet, dann liegt eine Kollision vor.
				if(tbPointHitsBox(tbVector3TransformCoords(vPoint,
					                                       mBox1Transformation),
					              aBoxPlanes))
				{
					// Kollision!
					return true;
				}

				// Nchster Punkt
				vPoint.z += fStepZ;
			}

			// Nchster Punkt
			vPoint.y += fStepY;
		}

		// Nchster Punkt
		vPoint.x += fStepX;
	}


	// Nun machen wir das ganze noch einmal, aber diesmal durchwandern wir
	// den zweiten Quader und nicht den ersten.

	tbComputeBoxPlanes(vBox1Min, vBox1Max, mBox1Transformation, aBoxPlanes);
	fStepX = (vBox2Max.x - vBox2Min.x) / (float)(iNumSamples);
	fStepY = (vBox2Max.y - vBox2Min.y) / (float)(iNumSamples);
	fStepZ = (vBox2Max.z - vBox2Min.z) / (float)(iNumSamples);
	vPoint = vBox2Min;

	for(int x = 0; x < iNumSamples; x++)
	{
		vPoint.y = vBox2Min.y;
		vPoint.z = vBox2Min.z;
		for(int y = 0; y < iNumSamples; y++)
		{
			vPoint.z = vBox2Min.z;
			for(int z = 0; z < iNumSamples; z++)
			{
				if(tbPointHitsBox(tbVector3TransformCoords(vPoint,
					                                       mBox2Transformation),
					              aBoxPlanes))
				{
					return true;
				}

				vPoint.z += fStepZ;
			}

			vPoint.y += fStepY;
		}

		vPoint.x += fStepX;
	}

	// Keine Kollision!
	return false;
}

// ******************************************************************
// Kollision zwischen zwei Quadern berechnen (schnell)
bool tbBoxHitsBox2(const tbVector3& vBox1Min,
							   const tbVector3& vBox1Max,
							   const tbMatrix& mBox1Transformation,
							   const tbVector3& vBox2Min,
							   const tbVector3& vBox2Max,
							   const tbMatrix& mBox2Transformation)
{
	const tbVector3 vBox1Extents((vBox1Max - vBox1Min) * 0.5f);
	const tbVector3 vBox1Position(tbVector3TransformCoords(vBox1Max - vBox1Extents, mBox1Transformation));
	const tbVector3 vBox2Extents((vBox2Max - vBox2Min) * 0.5f);
	const tbVector3 vBox2Position(tbVector3TransformCoords(vBox2Max - vBox2Extents, mBox2Transformation));
	const tbVector3 v(vBox2Position - vBox1Position);
	const tbVector3 T(tbVector3Dot(v, tbVector3(&mBox1Transformation.m11)),
					  tbVector3Dot(v, tbVector3(&mBox1Transformation.m21)),
					  tbVector3Dot(v, tbVector3(&mBox1Transformation.m31)));

	float R[3][3];
	float ra, rb, t;
	int i, k;

	for(i = 0; i < 3; i++)
	{
		for(k = 0; k < 3; k++)
		{
			R[i][k] = tbVector3Dot(tbVector3(mBox1Transformation.n + i * 4),
								   tbVector3(mBox1Transformation.n + k * 4));
		}
	}

	for(i = 0; i < 3; i++)
	{
		ra = vBox1Extents.c[i];
		rb = vBox2Extents.c[0] * fabsf(R[i][0]) +
			 vBox2Extents.c[1] * fabsf(R[i][1]) +
			 vBox2Extents.c[2] * fabsf(R[i][2]);
		t = fabsf(T.c[i]);
		if(t > ra + rb) return false;
	}

	for(k = 0; k < 3; k++)
	{
		ra = vBox1Extents.c[0] * fabsf(R[0][k]) +
			 vBox1Extents.c[1] * fabsf(R[1][k]) +
			 vBox1Extents.c[2] * fabsf(R[2][k]);
		rb = vBox2Extents.c[k];
		t = fabsf(T.c[0] * R[0][k] + T.c[1] * R[1][k] + T.c[2] * R[2][k]);
		if(t > ra + rb) return false;
	}

	ra = vBox1Extents.c[1] * fabsf(R[2][0]) + vBox1Extents.c[2] * fabsf(R[1][0]);
	ra = vBox2Extents.c[1] * fabsf(R[0][2]) + vBox2Extents.c[2] * fabsf(R[0][1]);
	t = fabsf(T.c[2] * R[1][0] - T.c[1] * R[2][0]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[1] * fabsf(R[2][1]) + vBox1Extents.c[2] * fabsf(R[1][1]);
	rb = vBox2Extents.c[0] * fabsf(R[0][2]) + vBox2Extents.c[2] * fabsf(R[0][0]);
	t = fabsf(T.c[2] * R[1][1] - T.c[1] * R[2][1]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[1] * fabsf(R[2][2]) + vBox1Extents.c[2] * fabsf(R[1][2]);
	rb = vBox2Extents.c[0] * fabsf(R[0][1]) + vBox2Extents.c[1] * fabsf(R[0][0]);
	t = fabsf(T.c[2] * R[1][2] - T.c[1] * R[2][2]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[0] * fabsf(R[2][0]) + vBox1Extents.c[2] * fabsf(R[0][0]);
	rb = vBox2Extents.c[1] * fabsf(R[1][2]) + vBox2Extents.c[2] * fabsf(R[1][1]);
	t = fabsf(T.c[0] * R[2][0] - T.c[2] * R[0][0]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[0] * fabsf(R[2][1]) + vBox1Extents.c[2] * fabsf(R[0][1]);
	rb = vBox2Extents.c[0] * fabsf(R[1][2]) + vBox2Extents.c[2] * fabsf(R[1][0]);
	t = fabsf(T.c[0] * R[2][1] - T.c[2] * R[0][1]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[0] * fabsf(R[2][2]) + vBox1Extents.c[2] * fabsf(R[0][2]);
	rb = vBox2Extents.c[0] * fabsf(R[1][1]) + vBox2Extents.c[1] * fabsf(R[1][0]);
	t = fabsf(T.c[0] * R[2][2] - T.c[2] * R[0][2]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[0] * fabsf(R[1][0]) + vBox1Extents.c[1] * fabsf(R[0][0]);
	rb = vBox2Extents.c[1] * fabsf(R[2][2]) + vBox2Extents.c[2] * fabsf(R[2][1]);
	t = fabsf(T.c[1] * R[0][0] - T.c[0] * R[1][0]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[0] * fabsf(R[1][1]) + vBox1Extents.c[1] * fabsf(R[0][1]);
	rb = vBox2Extents.c[0] * fabsf(R[2][2]) + vBox2Extents.c[2] * fabsf(R[2][0]);
	t = fabsf(T.c[1] * R[0][1] - T.c[0] * R[1][1]);
	if(t > ra + rb) return false;

	ra = vBox1Extents.c[0] * fabsf(R[1][2]) + vBox1Extents.c[1] * fabsf(R[0][2]);
	rb = vBox2Extents.c[0] * fabsf(R[2][1]) + vBox2Extents.c[1] * fabsf(R[2][0]);
	t = fabsf(T.c[1] * R[0][2] - T.c[0] * R[1][2]);
	if(t > ra + rb) return false;

	return true;
}
//*******************************************************************
//selbst

//ende

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn die Kugel vollstndig vor der Ebene liegt,
// -1, wenn sie vollstndig hinter der Ebene liegt und
//  0, wenn die Kugel die Ebene schneidet.
int tbClassifySphere(const tbVector3& vSphere,
								 const float fRadius,
								 const tbPlane& Plane)
{
	// Die Entfernung des Kugelmittelpunkts zur Ebene ist entscheidend.
	float fDistance = tbPointPlaneDistance(vSphere, Plane);
	if(fabsf(fDistance) < fRadius) return 0;

	return fDistance > 0.0f ? 1 : -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollstndig vor der Ebene liegt,
// -1, wenn er vollstndig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
int tbClassifyBox(const tbVector3& vBoxMin,
							  const tbVector3& vBoxMax,
							  const tbMatrix& mBoxTransformation,
							  const tbPlane& Plane)
{
	// Es gibt ein recht schnelles Verfahren, dies herauszufinden, aber
	// es funktioniert nur mit achsenausgerichteten Quadern.
	// Darum wenden wir einen Trick an und tun so, als sei der Quader
	// achsenausgerichtet und transformieren statt dessen einfach die Ebene
	// mit der invertierten Matrix des Quaders.

	// Die Ebene transformieren
	tbPlane NewPlane(tbPlaneTransform(Plane, tbMatrixInvert(mBoxTransformation)));

	// Nchsten und fernsten Punkt berechnen
	tbVector3 vNearPoint(vBoxMax);
	tbVector3 vFarPoint(vBoxMin);
	if(NewPlane.n.x > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(NewPlane.n.y > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(NewPlane.n.z > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prfen, ob der nchste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(tbPlaneDotCoords(NewPlane, vNearPoint) > 0.0f) return 1;

	// Prfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(tbPlaneDotCoords(NewPlane, vFarPoint) > 0.0f) return 0;

	// Ansonsten liegt der Quader auf der Rckseite.
	return -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollstndig vor der Ebene liegt,
// -1, wenn er vollstndig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
int tbClassifyBox_Inv(const tbVector3& vBoxMin,
								  const tbVector3& vBoxMax,
								  const tbMatrix& mInvBoxTransformation,
								  const tbPlane& Plane)
{
	// Es gibt ein recht schnelles Verfahren, dies herauszufinden, aber
	// es funktioniert nur mit achsenausgerichteten Quadern.
	// Darum wenden wir einen Trick an und tun so, als sei der Quader
	// achsenausgerichtet und transformieren statt dessen einfach die Ebene
	// mit der invertierten Matrix des Quaders.

	// Die Ebene transformieren
	tbPlane NewPlane(tbPlaneTransform(Plane, mInvBoxTransformation));

	// Nchsten und fernsten Punkt berechnen
	tbVector3 vNearPoint(vBoxMax);
	tbVector3 vFarPoint(vBoxMin);
	if(NewPlane.n.x > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(NewPlane.n.y > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(NewPlane.n.z > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prfen, ob der nchste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(tbPlaneDotCoords(NewPlane, vNearPoint) > 0.0f) return 1;

	// Prfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(tbPlaneDotCoords(NewPlane, vFarPoint) > 0.0f) return 0;

	// Ansonsten liegt der Quader auf der Rckseite.
	return -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollstndig vor der Ebene liegt,
// -1, wenn er vollstndig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
//
// ACHTUNG: Diese Funktion arbeitet mit achsenausgerichteten Quadern!
//          Diese Quader haben keine Transformationsmatrix!
int tbClassifyAABox(const tbVector3& vBoxMin,
								const tbVector3& vBoxMax,
								const tbPlane& Plane)
{
	// Nchsten und fernsten Punkt berechnen
	tbVector3 vNearPoint(vBoxMax);
	tbVector3 vFarPoint(vBoxMin);
	if(Plane.n.x > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(Plane.n.y > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(Plane.n.z > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prfen, ob der nchste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(tbPlaneDotCoords(Plane, vNearPoint) > 0.0f) return 1;

	// Prfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(tbPlaneDotCoords(Plane, vFarPoint) > 0.0f) return 0;

	// Der Quader befindet sich hinter der Ebene.
	return -1;
}

// ******************************************************************
// Sichtbarkeit eines Punkts prfen
bool tbPointVisible(const tbVector3& vPoint,
								const tbPlane* pClipPlanes)
{
	// Den Punkt in jede Ebenengleichung einsetzen
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[0]) > 0.0f) return false;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[1]) > 0.0f) return false;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[2]) > 0.0f) return false;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[3]) > 0.0f) return false;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[4]) > 0.0f) return false;
	return tbPointPlaneDistanceFast(vPoint, pClipPlanes[5]) <= 0.0f;
}

// ******************************************************************
// Sichtbarkeit einer Kugel prfen
bool tbSphereVisible(const tbVector3& vSphere,
								 const float fRadius,
								 const tbPlane* pClipPlanes)
{
	// Den Kugelmittelpunkt in jede Ebenengleichung einsetzen
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[0]) > fRadius) return false;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[1]) > fRadius) return false;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[2]) > fRadius) return false;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[3]) > fRadius) return false;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[4]) > fRadius) return false;
	return tbPointPlaneDistanceFast(vSphere, pClipPlanes[5]) <= fRadius;
}

// ******************************************************************
// Sichtbarkeit eines achsenausgerichteten Quaders prfen
bool tbAABoxVisible(const tbVector3& vBoxMin,
								const tbVector3& vBoxMax,
								const tbPlane* pClipPlanes)
{
	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichtbar.
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[0]) == 1) return false;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[1]) == 1) return false;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[2]) == 1) return false;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[3]) == 1) return false;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[4]) == 1) return false;
	return tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Sichtbarkeit eines Quaders prfen
bool tbBoxVisible(const tbVector3& vBoxMin,
							  const tbVector3& vBoxMax,
							  const tbMatrix& mBoxTransformation,
							  const tbPlane* pClipPlanes)
{
	// Matrix invertieren
	tbMatrix mInvBoxTransformation(tbMatrixInvert(mBoxTransformation));

	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichtbar.
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[0]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[1]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[2]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[3]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[4]) == 1) return false;
	return tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Sichtbarkeit eines Quaders prfen
bool tbBoxVisible_Inv(const tbVector3& vBoxMin,
								  const tbVector3& vBoxMax,
								  const tbMatrix& mInvBoxTransformation,
								  const tbPlane* pClipPlanes)
{
	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichtbar.
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[0]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[1]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[2]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[3]) == 1) return false;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[4]) == 1) return false;
	return tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Berechnet den Zielvektor, um ein Objekt zu treffen
tbVector3 tbComputeAimingVector(const tbVector3& vOrigin,
											const tbVector3& vTargetPos,
											const tbVector3& vTargetVel,
											const float fProjectileSpeed)
{
	// Richtung vom Abschusspunkt zum Ziel bestimmen
	tbVector3 vDirection(vTargetPos - vOrigin);

	// Diskriminante des Terms fr die Zeit berechnen.
	// Wenn sie kleiner als null ist, dann ist es nicht mglich, das Ziel zu treffen.
	// Das kann zum Beispiel passieren, wenn das Ziel schneller als das Geschoss ist.
	// Wir liefern dann einfach die Position des Ziels als Zielvektor zurck.
	float fDiscriminant = fProjectileSpeed * fProjectileSpeed * tbVector3LengthSq(vDirection) - tbVector3LengthSq(tbVector3Cross(vDirection, vTargetVel));
	if(fDiscriminant < 0.0f) return vTargetPos;

	// Zeit des Treffers berechnen
	float fTime = (sqrtf(fDiscriminant) + tbVector3Dot(vDirection, vTargetVel))
				/ (fProjectileSpeed * fProjectileSpeed - tbVector3LengthSq(vTargetVel));

	// Zielvektor liefern
	return vTargetPos + fTime * vTargetVel;
}

// ******************************************************************

// ******************************************************************