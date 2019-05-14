#ifndef ROEMATRIX_H_
#define ROEMATRIX_H_

#include <cmath>
#include <cstdio>
#include <string>
#include "RoeVector2.h"
#include "RoeVector3.h"

namespace Roe {

	class Matrix;
	Matrix MatrixInvert(const Matrix& m);
	
	// ******************************************************************
	//Matrix class for 4*4 matrices (x-Direction = row, y-Direction = column)
	//all rotations work CW if you look into positive vector direction, and CCW if you look into negative vector direction
	class Matrix
	{
	public:
		// Variablen
		union
		{
			struct
			{
				float m11, m12, m13, m14,  // elements of the matrix
					  m21, m22, m23, m24,
					  m31, m32, m33, m34,
					  m41, m42, m43, m44;
			};
			
			float m[4][4]; // two-dimensional array
			float n[16];   // one-dimensional array
		};
		// ******************************************************************
		static const Matrix ZERO;     //matrix with just 0s
		static const Matrix IDENTITY; //identity matrix
		// ******************************************************************
		
		// constructors
		Matrix() {}
		Matrix(const Matrix& m) : m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14),
		                          m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24),
		                          m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34),
		                          m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44) {}
		Matrix(float _m11, float _m12, float _m13, float _m14,
		       float _m21, float _m22, float _m23, float _m24,
		       float _m31, float _m32, float _m33, float _m34,
		       float _m41, float _m42, float _m43, float _m44)
		         : m11(_m11), m12(_m12), m13(_m13), m14(_m14),
		           m21(_m21), m22(_m22), m23(_m23), m24(_m24),
		           m31(_m31), m32(_m32), m33(_m33), m34(_m34),
		           m41(_m41), m42(_m42), m43(_m43), m44(_m44) {}
		// destructor
		~Matrix() {};
		
		// casting operators
		operator float* () {return (float*)(n);}
		
		// access operators
		float& operator () (int iRow, int iColumn)       {return m[iRow - 1][iColumn - 1];}
		float  operator () (int iRow, int iColumn) const {return m[iRow - 1][iColumn - 1];}
		
		// assignment operators
		Matrix& operator = (const Matrix& m)
		{
			m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
			m21 = m.m21; m22 = m.m22; m23 = m.m23; m24 = m.m24;
			m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
			m41 = m.m41; m42 = m.m42; m43 = m.m43; m44 = m.m44;
			return *this;
		}
		
		Matrix& operator += (const Matrix& m)
		{
			m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
			m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
			m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
			m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
			return *this;
		}
	
		Matrix& operator -= (const Matrix& m)
		{
			m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
			m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
			m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
			m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
			return *this;
		}
	
		Matrix& operator *= (const Matrix& m)
		{
			return *this = Matrix(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
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
		Matrix& operator *= (const float f)
		{
			m11 *= f; m12 *= f; m13 *= f; m14 *= f;
			m21 *= f; m22 *= f; m23 *= f; m24 *= f;
			m31 *= f; m32 *= f; m33 *= f; m34 *= f;
			m41 *= f; m42 *= f; m43 *= f; m44 *= f;
			return *this;
		}
		
		Matrix& operator /= (const Matrix& m) {return *this *= Matrix::invert(m);}
		Matrix& operator /= (const float f)
		{
			m11 /= f; m12 /= f; m13 /= f; m14 /= f;
			m21 /= f; m22 /= f; m23 /= f; m24 /= f;
			m31 /= f; m32 /= f; m33 /= f; m34 /= f;
			m41 /= f; m42 /= f; m43 /= f; m44 /= f;
			return *this;
		}
		
		// comparison operators
		bool operator == (const Matrix& m)
		{
			if(m11 != m.m11) return false;
			if(m12 != m.m12) return false;
			if(m13 != m.m13) return false;
			if(m14 != m.m14) return false;
			if(m21 != m.m21) return false;
			if(m22 != m.m22) return false;
			if(m23 != m.m23) return false;
			if(m24 != m.m24) return false;
			if(m31 != m.m31) return false;
			if(m32 != m.m32) return false;
			if(m33 != m.m33) return false;
			if(m34 != m.m34) return false;
			if(m41 != m.m41) return false;
			if(m42 != m.m42) return false;
			if(m43 != m.m43) return false;
			return m44 == m.m44;
		}
		bool operator != (const Matrix& m)
		{
			if(m11 != m.m11) return true;
			if(m12 != m.m12) return true;
			if(m13 != m.m13) return true;
			if(m14 != m.m14) return true;
			if(m21 != m.m21) return true;
			if(m22 != m.m22) return true;
			if(m23 != m.m23) return true;
			if(m24 != m.m24) return true;
			if(m31 != m.m31) return true;
			if(m32 != m.m32) return true;
			if(m33 != m.m33) return true;
			if(m34 != m.m34) return true;
			if(m41 != m.m41) return true;
			if(m42 != m.m42) return true;
			if(m43 != m.m43) return true;
			return m44 != m.m44;
		}
		
		// ******************************************************************
		// other methods
		std::string toString  () const;
		void   setIdentity    ()       {*this = Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}
		float  determinant    () const {return determinant(*this);}   // calculate determinant
		void   invert         ()       {*this = invert(*this);}       // calculate inverted matrix
		void   transpose      ()       {*this = transpose(*this);}    // calculate transposed matrix
		Vector2   tranformCoords (const Vector2& v) const {return Matrix::transformCoords(v,*this);} //returns a with this matrix transformed vector2
		Vector2   tranformNormal (const Vector2& v) const {return Matrix::transformNormal(v,*this);} //returns a with this matrix transformed vector2
		Vector3   tranformCoords (const Vector3& v) const {return Matrix::transformCoords(v,*this);} //returns a with this matrix transformed vector3
		Vector3   tranformNormal (const Vector3& v) const {return Matrix::transformNormal(v,*this);} //returns a with this matrix transformed vector3
		
		//various static methods to create special matrices
		static float  determinant   (const Matrix &m);
		static Matrix invert        (const Matrix &m);
		static Matrix transpose     (const Matrix &m);
		static Matrix translation   (const Vector3& v);
		//all rotations work CW if you look into positive vector direction and CCW if you look into negative vector direction
		static Matrix rotationX     (const float f);
		static Matrix rotationY     (const float f);
		static Matrix rotationZ     (const float f);
		static Matrix rotation      (const float x, const float y, const float z);
		static Matrix rotation      (const Vector3& v);
		static Matrix rotationAxis  (const Vector3& v, const float f);
		static Matrix rotationFromTo(const Vector3& from, const Vector3& to); // from and to must be normalized
		static Matrix scaling       (const Vector3& v);
		static Matrix scaling       (const float s) {return Matrix::scaling(Vector3(s,s,s));}
		static Matrix scaling       (const float x, const float y, const float z) {return Matrix::scaling(Vector3(x,y,z));}
		static Matrix axes          (const Vector3& vXAxis, const Vector3& vYAxis, const Vector3& vZAxis);
		static Matrix projection    (const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);
		static Matrix camera        (const Vector3& vPos, const Vector3& vLookAt, const Vector3& vUp  = Vector3(0.0f, 1.0f, 0.0f));
		static Matrix toTex2DMatrix (const Matrix& m);
		
		// ******************************************************************
		//vector transformations
		static Vector2 transformCoords(const Vector2& v, const Matrix& m, float* pfOutW = NULL);   //tranform a position vector
		static Vector2 transformNormal(const Vector2& v, const Matrix& m);                         //transofrm a direction vector
		static Vector3 transformCoords(const Vector3& v, const Matrix& m, float* pfOutW = NULL); //transform position vector
		static Vector3 transformNormal(const Vector3& v, const Matrix& m);                       //transform direction vecotr
	};
	
	// arithmetic operators
	inline Matrix operator + (const Matrix& a, const Matrix& b) {return Matrix(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14, a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24, a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34, a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);}
	inline Matrix operator - (const Matrix& a, const Matrix& b) {return Matrix(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14, a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24, a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34, a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44);}
	inline Matrix operator - (const Matrix& m)                  {return Matrix(-m.m11, -m.m12, -m.m13, -m.m14, -m.m21, -m.m22, -m.m23, -m.m24, -m.m31, -m.m32, -m.m33, -m.m34, -m.m41, -m.m42, -m.m43, -m.m44);}
	
	inline Matrix operator * (const Matrix& a, const Matrix& b)
	{
		return Matrix(b.m11 * a.m11 + b.m21 * a.m12 + b.m31 * a.m13 + b.m41 * a.m14,
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
	inline Matrix operator * (const Matrix& m, const float f)
	{
		return Matrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
		              m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
		              m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
		              m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
	}
	inline Matrix operator * (const float f, const Matrix& m)
	{
		return Matrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
		              m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
		              m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
		              m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
	}
	
	inline Matrix operator / (const Matrix& a, const Matrix& b) {return a * Matrix::invert(b);}
	inline Matrix operator / (const Matrix& m, const float f)
	{
		return Matrix(m.m11 / f, m.m12 / f, m.m13 / f, m.m14 / f,
		              m.m21 / f, m.m22 / f, m.m23 / f, m.m24 / f,
		              m.m31 / f, m.m32 / f, m.m33 / f, m.m34 / f,
		              m.m41 / f, m.m42 / f, m.m43 / f, m.m44 / f);
	}
	
	
} // namespace Roe

#endif /*ROEMATRIX_H_*/
