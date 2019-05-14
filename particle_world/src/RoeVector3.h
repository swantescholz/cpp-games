#ifndef ROEVECTOR3_H_
#define ROEVECTOR3_H_

#include <cmath>
#include <cstdio>
#include <string>
#include "RoeRandom.h"

namespace Roe {
	
	//forward declaration
	class  Vector3;
	inline Vector3 operator * (const float f, const Vector3& v);
	inline Vector3 operator * (const Vector3& v, const float f);
	inline Vector3 operator / (const Vector3& v, const float f);
	inline bool    operator ==(const Vector3& a, const Vector3& b);
	inline bool    operator !=(const Vector3& a, const Vector3& b);
	
	class Vector3
	{
	public:
		//variables
		union
		{
			struct
			{
				float x;  // cooridinates
				float y;
				float z;
			};
			
			struct
			{
				float u;  // texture coordinates
				float v;
				float w;
			};
			
			float rg[3]; // array
		};
		
		// constructors
		Vector3()                                                                                     {}
		Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z)                                            {}
		Vector3(const float f) : x(f), y(f), z(f)                                                     {}
		Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)                 {}
		Vector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])   {}
		
		//destructors
		~Vector3() {}
		
		// casting operators
		operator float* () {return (float*)(rg);}
		
		// assignment operators
		Vector3& operator  = (const Vector3& v) {x  = v.x; y  = v.y; z  = v.z; return *this;}
		Vector3& operator += (const Vector3& v) {x += v.x; y += v.y; z += v.z; return *this;}
		Vector3& operator -= (const Vector3& v) {x -= v.x; y -= v.y; z -= v.z; return *this;}
		Vector3& operator *= (const Vector3& v) {x *= v.x; y *= v.y; z *= v.z; return *this;}
		Vector3& operator *= (const float f)    {x *= f;   y *= f;   z *= f;   return *this;}
		Vector3& operator /= (const Vector3& v) {x /= v.x; y /= v.y; z /= v.z; return *this;}
		Vector3& operator /= (const float f)    {x /= f;   y /= f;   z /= f;   return *this;}
		
		//other methods
		std::string toString   () const {char c[80]; sprintf(c,"Vector3(%f, %f, %f)",x,y,z); return std::string(c);}
		Vector3 crossProduct   (const Vector3& v) const {return crossProduct(*this, v);}
		float   dotProduct     (const Vector3& v) const {return dotProduct  (*this, v);}
		float   angleBetween   (const Vector3& v) const {return angleBetween(*this, v);}
		float   distance       (const Vector3& v) const {return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z));}
		float   squaredDistance(const Vector3& v) const {return      (x-v.x)*(x-v.x)+(y-v.y)*(y-v.y)+(z-v.z)*(z-v.z) ;}
		Vector3 midPoint       (const Vector3& v) const {return Vector3(x*v.x*0.5+y*v.y*0.5+z*v.z*0.5);}
		float   length         ()                 const {return sqrt(x*x+y*y+z*z);}
		float   squaredLength  ()                 const {return x*x+y*y+z*z;}
		void    normalize      ()                       {(*this) /= sqrt(x*x+y*y+z*z);}
		void    makeFloor      (const Vector3& v)       {if(v.x<x)x=v.x;if(v.y<y)y=v.y;if(v.z<z)z=v.z;}
		void    makeCeil       (const Vector3& v)       {if(v.x>x)x=v.x;if(v.y>y)y=v.y;if(v.z>z)z=v.z;}
		void    makeRandom     ()                       {(*this) = createRandomUnit();}
		
		static bool    isNaN        (const Vector3& v)                   {return v.x!=v.x || v.y!=v.y || v.z!=v.z;}
		static bool   isNearlyZero  (const Vector3& v, float eps = 0.0001f) {return std::abs(v.x)<=eps && std::abs(v.y)<=eps && std::abs(v.z)<=eps;}
		static Vector3 length       (const Vector3& v)                   {return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
		static Vector3 squaredLength(const Vector3& v)                   {return v.x*v.x+v.y*v.y+v.z*v.z;}
		static Vector3 normalize    (const Vector3& v)                   {return v / sqrt(v.x*v.x+v.y*v.y+v.z*v.z);}
		static Vector3 floor        (const Vector3& a, const Vector3& b) {return Vector3((a.x<b.x)?a.x:b.x, (a.y<b.y)?a.y:b.y, (a.z<b.z)?a.z:b.z);}
		static Vector3 ceil         (const Vector3& a, const Vector3& b) {return Vector3((a.x>b.x)?a.x:b.x, (a.y>b.y)?a.y:b.y, (a.z>b.z)?a.z:b.z);}
		static Vector3 crossProduct (const Vector3& a, const Vector3& b) {return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);}
		static float   dotProduct   (const Vector3& a, const Vector3& b) {return a.x*b.x + a.y*b.y + a.z*b.z;}
		static float   angleBetween (const Vector3& a, const Vector3& b) {
			return acos((a.x*b.x + a.y*b.y + a.z*b.z) /
				sqrt((a.x*a.x+a.y*a.y+a.z*a.z) * (b.x*b.x+b.y*b.y+b.z*b.z)));} //returns angle as radian, works CCW
		static Vector3 makePerpendicularTo(const Vector3& v1, const Vector3& v2) { //makes v1 perpendicular to v2 (return value)
			return v1.length()*Vector3::normalize(Vector3::crossProduct(Vector3::crossProduct(v2,v1), v2));}
		static Vector3 createRandomBox(const Vector3& min, const Vector3& max) {
			const float x = Random::getSingletonPtr()->uniformReal(min.x,max.x);
			const float y = Random::getSingletonPtr()->uniformReal(min.y,max.y);
			const float z = Random::getSingletonPtr()->uniformReal(min.z,max.z);
			return Vector3(x,y,z);}
		static Vector3 createRandomUnit() {
			const float z = Random::getSingletonPtr()->uniformReal(-1.0f,1.0f);
			const float t = Random::getSingletonPtr()->uniformReal(0.0f, 2*3.14159265359f);
			const float r = std::sqrt(1.0f - z*z);
			return Vector3(r*std::cos(t), r*std::sin(t), z);}
		
		//special points
		static const Vector3 ZERO;
		static const Vector3 X;
		static const Vector3 Y;
		static const Vector3 Z;
		static const Vector3 NX;
		static const Vector3 NY;
		static const Vector3 NZ;
		static const Vector3 ONE;
		static const Vector3 GRAVITY; //normal earth gravity(9.81m/s^2)
	};
	
	// comparison operators
	inline bool operator == (const Vector3& a, const Vector3& b) {return a.x==b.x && a.y==b.y && a.z==b.z;}
	inline bool operator != (const Vector3& a, const Vector3& b) {return a.x!=b.x || a.y!=b.y || a.z!=b.z;}
	inline bool operator <= (const Vector3& a, const Vector3& b) {return a.x*a.x+a.y*a.y+a.z*a.z <= b.x*b.x+b.y*b.y+b.z*b.z;}
	inline bool operator <  (const Vector3& a, const Vector3& b) {return a.x*a.x+a.y*a.y+a.z*a.z <  b.x*b.x+b.y*b.y+b.z*b.z;}
	inline bool operator >= (const Vector3& a, const Vector3& b) {return a.x*a.x+a.y*a.y+a.z*a.z >= b.x*b.x+b.y*b.y+b.z*b.z;}
	inline bool operator >  (const Vector3& a, const Vector3& b) {return a.x*a.x+a.y*a.y+a.z*a.z >  b.x*b.x+b.y*b.y+b.z*b.z;}
	
	// arithmetic operators
	inline Vector3 operator + (const Vector3& a, const Vector3& b)  {return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);}
	inline Vector3 operator - (const Vector3& a, const Vector3& b)  {return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);}
	inline Vector3 operator - (const Vector3& v)                    {return Vector3(-v.x, -v.y, -v.z);}
	inline Vector3 operator * (const Vector3& a, const Vector3& b)  {return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);}
	inline Vector3 operator * (const Vector3& v, const float f)     {return Vector3(v.x * f, v.y * f, v.z * f);}
	inline Vector3 operator * (const float f,    const Vector3& v)  {return Vector3(v.x * f, v.y * f, v.z * f);}
	inline Vector3 operator / (const Vector3& a, const Vector3& b)  {return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);}
	inline Vector3 operator / (const Vector3& v, const float f)     {return Vector3(v.x / f, v.y / f, v.z / f);}
	
} // namespace Roe

#endif /*ROEVECTOR3_H_*/
