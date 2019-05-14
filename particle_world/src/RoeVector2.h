#ifndef ROEVECTOR2_H_
#define ROEVECTOR2_H_

#include <cmath>
#include <string>
#include <cstdio>
#include "RoeRandom.h"

namespace Roe {
	
	//forward declaration
	class  Vector2;
	inline Vector2 operator * (const Vector2& v, const float f);
	inline Vector2 operator / (const Vector2& v, const float f);
	inline bool    operator ==(const Vector2& a, const Vector2& b);
	inline bool    operator !=(const Vector2& a, const Vector2& b);
	
	class Vector2
	{
	public:
		//variables
		union
		{
			struct
			{
				float x;  // cooridinates
				float y;
			};
			
			struct
			{
				float u;  // texture coordinates
				float v;
			};
			
			float rg[2]; // array
		};
		
		// constructors
		Vector2()                                                                 {}
		Vector2(const Vector2& v) : x(v.x), y(v.y)                                {}
		Vector2(const float f) : x(f), y(f)                                       {}
		Vector2(const float _x, const float _y) : x(_x), y(_y)                    {}
		Vector2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])  {}
		
		//destructors
		~Vector2() {}
		
		// casting operators
		operator float* () {return (float*)(rg);}
		
		// assignment operators
		Vector2& operator  = (const Vector2& v) {x  = v.x; y  = v.y; return *this;}
		Vector2& operator += (const Vector2& v) {x += v.x; y += v.y; return *this;}
		Vector2& operator -= (const Vector2& v) {x -= v.x; y -= v.y; return *this;}
		Vector2& operator *= (const Vector2& v) {x *= v.x; y *= v.y; return *this;}
		Vector2& operator *= (const float f)    {x *= f;   y *= f;   return *this;}
		Vector2& operator /= (const Vector2& v) {x /= v.x; y /= v.y; return *this;}
		Vector2& operator /= (const float f)    {x /= f;   y /= f;   return *this;}
		
		//other methods
		std::string toString   () const {char c[60]; sprintf(c,"Vector3(%f, %f)",x,y); return std::string(c);}
		float   dotProduct     (const Vector2& v) const {return x * v.x + y * v.y;}
		float   angleBetween   (const Vector2& v) const {return acos((x * v.x + y * v.y) / sqrt((x*x+y*y) * (v.x*v.x+v.y*v.y)));}
		float   distance       (const Vector2& v) const {return sqrt((x-v.x)*(x-v.x)+(y-v.y)*(y-v.y));}
		float   squaredDistance(const Vector2& v) const {return      (x-v.x)*(x-v.x)+(y-v.y)*(y-v.y) ;}
		Vector2 midPoint       (const Vector2& v) const {return Vector2(x*v.x*0.5+y*v.y*0.5);}
		float   length         () const {return sqrt(x*x+y*y);}
		float   squaredLength  () const {return      x*x+y*y;}
		void    normalize      ()       {(*this) /= sqrt(x*x+y*y);}
		void    makeFloor      (const Vector2& v)       {if(v.x<x)x=v.x;if(v.y<y)y=v.y;}
		void    makeCeil       (const Vector2& v)       {if(v.x>x)x=v.x;if(v.y>y)y=v.y;}
		void    makeRandom     ()                       {(*this) = createRandomUnit();}
		
		static bool    isNaN       (const Vector2& v)                   {return v.x!=v.x || v.y!=v.y;}
		static bool   isNearlyZero (const Vector2& v, float eps = 0.0001f) {return std::abs(v.x)<=eps && std::abs(v.y)<=eps;}
		static Vector2 normalize   (const Vector2& v)                   {return v / sqrt(v.x*v.x+v.y*v.y);}
		static Vector2 floor       (const Vector2& a, const Vector2& b) {return Vector2((a.x<b.x)?a.x:b.x, (a.y<b.y)?a.y:b.y);}
		static Vector2 ceil        (const Vector2& a, const Vector2& b) {return Vector2((a.x>b.x)?a.x:b.x, (a.y>b.y)?a.y:b.y);}
		static Vector2 dotProduct  (const Vector2& a, const Vector2& b) {return a.x*b.x + a.y*b.y;}
		static Vector2 angleBetween(const Vector2& a, const Vector2& b) {return acos((a.x * b.x + a.y * b.y) / sqrt((a.x*a.x+a.y*a.y) * (b.x*b.x+b.y*b.y)));}
		
		static Vector2 createRandomBox(const Vector2& min, const Vector2& max) {
			const float x = Random::getSingletonPtr()->uniformReal(min.x, max.x);
			const float y = Random::getSingletonPtr()->uniformReal(min.y, max.y);
			return Vector2(x,y);}
		static Vector2 createRandomUnit() {
			float x,y;
			do {
				x = Random::getSingletonPtr()->uniformReal(-1.0f,1.0f);
				y = Random::getSingletonPtr()->uniformReal(-1.0f,1.0f);
			} while(x*x+y*y > 1.0f);
			return Vector2::normalize(Vector2(x,y));}
		
		// special points
		static const Vector2 ZERO;
		static const Vector2 X;
		static const Vector2 Y;
		static const Vector2 NX;
		static const Vector2 NY;
		static const Vector2 ONE;
	};
	
	// comparison operators	
	inline bool operator == (const Vector2& a, const Vector2& b) {return a.x==b.x && a.y==b.y;}
	inline bool operator != (const Vector2& a, const Vector2& b) {return a.x!=b.x || a.y!=b.y;}
	inline bool operator <= (const Vector2& a, const Vector2& b) {return a.x*a.x+a.y*a.y <= b.x*b.x+b.y*b.y;}
	inline bool operator <  (const Vector2& a, const Vector2& b) {return a.x*a.x+a.y*a.y <  b.x*b.x+b.y*b.y;}
	inline bool operator >= (const Vector2& a, const Vector2& b) {return a.x*a.x+a.y*a.y >= b.x*b.x+b.y*b.y;}
	inline bool operator >  (const Vector2& a, const Vector2& b) {return a.x*a.x+a.y*a.y >  b.x*b.x+b.y*b.y;}
	
	// arithmetic operators
	inline Vector2 operator + (const Vector2& a, const Vector2& b)  {return Vector2(a.x + b.x, a.y + b.y);}
	inline Vector2 operator - (const Vector2& a, const Vector2& b)  {return Vector2(a.x - b.x, a.y - b.y);}
	inline Vector2 operator - (const Vector2& v)                    {return Vector2(-v.x, -v.y);}
	inline Vector2 operator * (const Vector2& a, const Vector2& b)  {return Vector2(a.x * b.x, a.y * b.y);}
	inline Vector2 operator * (const Vector2& v, const float f)     {return Vector2(v.x * f, v.y * f);}
	inline Vector2 operator * (const float f,    const Vector2& v)  {return Vector2(v.x * f, v.y * f);}
	inline Vector2 operator / (const Vector2& a, const Vector2& b)  {return Vector2(a.x / b.x, a.y / b.y);}
	inline Vector2 operator / (const Vector2& v, const float f)     {return Vector2(v.x / f, v.y / f);}
	
} // namespace Roe

#endif /*ROEVECTOR2_H_*/
