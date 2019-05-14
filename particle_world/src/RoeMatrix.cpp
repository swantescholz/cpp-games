#include "RoeMatrix.h"
#include "RoeUtil.h"

namespace Roe {
	
	const Matrix Matrix::ZERO    (0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	const Matrix Matrix::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	
	// ******************************************************************
	std::string Matrix::toString() const
	{
		char c[350];
		sprintf(c,"Matrix( %f, %f, %f, %f\n        %f, %f, %f, %f\n        %f, %f, %f, %f\n        %f, %f, %f, %f)",
		        m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44);
		return std::string(c);
	}
	// ******************************************************************
	// returns the determinant
	float Matrix::determinant(const Matrix &m)
	{
		//determinant of the left 3*3 matrix
		return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
		       m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
		       m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
	}
	
	// ******************************************************************
	Matrix Matrix::invert(const Matrix &m)
	{
		//calculate inversed value of the determinant
		float fInvDet = determinant(m);
		if(fInvDet == 0.0f) return Matrix::IDENTITY;
		fInvDet = 1.0f / fInvDet;
		
		Matrix mResult;
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
	Matrix Matrix::transpose(const Matrix& m)
	{
		// Matrix transponieren
		return Matrix(m.m11, m.m21, m.m31, m.m41,
		              m.m12, m.m22, m.m32, m.m42,
		              m.m13, m.m23, m.m33, m.m43,
		              m.m14, m.m24, m.m34, m.m44);
	}
	// ******************************************************************
	// STATIC...
	// ******************************************************************
	Matrix Matrix::translation(const Vector3& v)
	{
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
		              0.0f, 1.0f, 0.0f, 0.0f,
		              0.0f, 0.0f, 1.0f, 0.0f,
		              v.x,  v.y,  v.z,  1.0f);
	}
	// ******************************************************************
	Matrix Matrix::rotationX(const float f)
	{
		Matrix mResult;
		
		mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
		mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
		mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
		mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;
		
		mResult.m22 = mResult.m33 = cosf(f);
		mResult.m23 = sinf(f);
		mResult.m32 = -mResult.m23;
		
		return mResult;
	}
	Matrix Matrix::rotationY(const float f)
	{
		Matrix mResult;
	
		                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
		mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
		                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
		mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;
		
		mResult.m11 = mResult.m33 = cosf(f);
		mResult.m31 = sinf(f);
		mResult.m13 = -mResult.m31;
		
		return mResult;
	}
	Matrix Matrix::rotationZ(const float f)
	{
		Matrix mResult;
		
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
	// rotation around all 3 axes
	Matrix Matrix::rotation(const float x, const float y, const float z)
	{
		return Matrix::rotationZ(z) * Matrix::rotationX(x) * Matrix::rotationY(y);
	}
	
	// rotation around all 3 axes (angles in the vector)
	Matrix Matrix::rotation(const Vector3& v)
	{
		return Matrix::rotationZ(v.z) * Matrix::rotationX(v.x) * Matrix::rotationY(v.y);
	}
	// ******************************************************************
	// rotation around a special axis
	Matrix Matrix::rotationAxis(const Vector3& v, const float f)
	{
		//calculate sine und cosine
		const float fSin = sinf(-f);
		const float fCos = cosf(-f);
		const float fOneMinusCos = 1.0f - fCos;
		
		//normalise axis vector
		const Vector3 vAxis(Vector3::normalize(v));
		
		return Matrix((vAxis.x * vAxis.x) * fOneMinusCos + fCos,
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
		              0.0f, 0.0f, 0.0f, 1.0f);
	}
	// ******************************************************************
	// transformation matrix to rotate the dir vector from to the dir vector to
	// from and to must be normalized
	Matrix Matrix::rotationFromTo(const Vector3& from, const Vector3& to) {
		if (Util::isNearlyZero(from.x-to.x) && Util::isNearlyZero(from.y-to.y) && Util::isNearlyZero(from.z-to.z))
			return Matrix::IDENTITY; //nearly equal
		if ((Util::isNearlyZero(to.x)   && Util::isNearlyZero(to.z)) ||
		    (Util::isNearlyZero(from.x) && Util::isNearlyZero(from.z))) {
			return Matrix::rotationAxis(Vector3::crossProduct(from, to),
		           Vector3::angleBetween(from, to) );
		}
		Matrix rot(Matrix::rotationY(Util::algebraicSign(Vector3::crossProduct(Vector3(from.x,0.0f,from.z), Vector3(to.x,0.0f,to.z)).y) *
		           Vector3::angleBetween(Vector3(from.x,0.0f,from.z), Vector3(to.x,0.0f,to.z))));
		
		const Vector3 tmp = Matrix::transformNormal(from, rot);
		if (Vector3::isNearlyZero(to-tmp)) return rot; //rotation around y-axis is sufficient
		return rot * Matrix::rotationAxis(Vector3::crossProduct(tmp, to), Vector3::angleBetween(tmp, to) );
	}
	
	// ******************************************************************
	// scaling matrix
	Matrix Matrix::scaling(const Vector3& v)
	{
		return Matrix(v.x,  0.0f, 0.0f, 0.0f,
		              0.0f, v.y,  0.0f, 0.0f,
		              0.0f, 0.0f, v.z,  0.0f,
		              0.0f, 0.0f, 0.0f, 1.0f);
	}
	// ******************************************************************
	// returns a axes matrix
	Matrix Matrix::axes(const Vector3& vXAxis, const Vector3& vYAxis, const Vector3& vZAxis)
	{
		return Matrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		              vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
		              vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
		              0.0f,     0.0f,     0.0f,     1.0f);
	}
	// ******************************************************************
	// calculate projection matrix
	Matrix Matrix::projection(const float fFOV,
	                          const float fAspect,
	                          const float fNearPlane,
	                          const float fFarPlane)
	{
		const float s = 1.0f / tan(fFOV * 0.5f);
		const float Q = fFarPlane / (fFarPlane - fNearPlane);
	
		return Matrix(s / fAspect, 0.0f, 0.0f, 0.0f,
		              0.0f, s, 0.0f, 0.0f,
		              0.0f, 0.0f, Q, 1.0f,
		              0.0f, 0.0f, -Q * fNearPlane, 0.0f);
	}
	// ******************************************************************
	// calculate camera matrix
	Matrix Matrix::camera(const Vector3& vPos,
	                      const Vector3& vLookAt,
	                      const Vector3& vUp) // = Vector3(0.0f, 1.0f, 0.0f)
	{
		//calculate z-axis
		Vector3 vZAxis(-Vector3::normalize(vLookAt - vPos)); //the minus was originally not here, but it works for OpenGL!
		//calculate x-axis
		Vector3 vXAxis(Vector3::normalize(vUp.crossProduct(vZAxis)));
		//calculate y-axis
		Vector3 vYAxis(Vector3::normalize(vZAxis.crossProduct(vXAxis)));
	
		// multiply translation matrix with rotation matrix
		return Matrix::translation(-vPos) * 
		       Matrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		              vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
		              vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
		              0.0f,     0.0f,     0.0f,     1.0f);
	}
	
	// ******************************************************************
	// texture matrix
	Matrix Matrix::toTex2DMatrix(const Matrix& m)
	{
		return Matrix(m.m11, m.m12, m.m14, 0.0f,
		              m.m21, m.m22, m.m24, 0.0f,
		              m.m41, m.m42, m.m44, 0.0f,
		              0.0f,  0.0f,  0.0f,  1.0f);
	}
	
	// ******************************************************************
	
	Vector2 Matrix::transformCoords(const Vector2& v, const Matrix& m, float* pfOutW) { //tranform a position vector
		Vector2 vResult(v.x * m.m11 + v.y * m.m21 + m.m41,
		                v.x * m.m12 + v.y * m.m22 + m.m42);
		const float w = v.x * m.m14 + v.y * m.m24 + m.m44;
		if(w != 1.0f) vResult /= w;
		if(pfOutW) *pfOutW = w;
		return Vector2::normalize(vResult) * v.length();
	}
	Vector2 Matrix::transformNormal(const Vector2& v, const Matrix& m) { //transofrm a direction vector
		const float fLength = v.length();
		if(fLength == 0.0f) return v;
		const Matrix mTransform(Matrix::transpose(Matrix::invert(m)));
		return Vector2::normalize(Vector2(v.x * mTransform.m11 + v.y * mTransform.m21,
		                                  v.x * mTransform.m12 + v.y * mTransform.m22)) * fLength;
	}
	Vector3 Matrix::transformCoords  (const Vector3& v, const Matrix& m, float* pfOutW) { //transform position vector
		Vector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		                v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
		                v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);
		const float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
		if(w != 1.0f) vResult /= w;
		if(pfOutW) *pfOutW = w;
		return vResult;
	}
	Vector3 Matrix::transformNormal  (const Vector3& v, const Matrix& m) { //transform direction vecotr
		const float fLength = v.length();
		if(fLength == 0.0f) return v;
		const Matrix mTransform(Matrix::transpose(Matrix::invert(m)));
		return Vector3::normalize(Vector3(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                                  v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
		                                  v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33)) * fLength;
	}


} // namespace Roe
