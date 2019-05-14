#ifndef ROECOLOR_
#define ROECOLOR_

#include <cmath>
#include <cstdio>
#include <string>
#include "RoeRandom.h"

namespace Roe {
	
	class Color
	{
	public:
		// variables
		union
		{
			struct
			{
				float r; // red, green, blue and alpha components
				float g;
				float b;
				float a;
			};
		
			struct
			{
				float fRed;   // red, green, blue and alpha components
				float fGreen;
				float fBlue;
				float fAlpha;
			};
			float rg[4]; //array (RanGe)
		};
		
		// constructors
		Color()                                                                                                      {}
		Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a)                                                       {}
		Color(const float f) : r(f), g(f), b(f), a(1.0f)                                                             {}
		Color(const float _r, const float _g, const float _b) : r(_r), g(_g), b(_b), a(1.0f)                         {}
		Color(const float _r, const float _g, const float _b, const float _a) : r(_r), g(_g), b(_b), a(_a)           {}
		Color(const float* pfComponent) : r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3]) {}
		
		// destructors
		~Color() {}
		
		// casting operators
		operator float* () {return (float*)(rg);}
		
		// assignment operators
		Color& operator  = (const Color& c) {r  = c.r; g  = c.g; b  = c.b; a  = c.a; return *this;}
		Color& operator += (const Color& c) {r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
		Color& operator -= (const Color& c) {r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
		Color& operator *= (const Color& c) {r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
		Color& operator *= (const float f)  {r *= f;   g *= f;   b *= f;   a *= f;   return *this;}
		Color& operator /= (const Color& c) {r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
		Color& operator /= (const float f)  {r /= f;   g /= f;   b /= f;   a /= f;   return *this;}
		
		
		// comparison operators
		bool operator == (const Color& c) {return r==c.r && g==c.g && b==c.b && a==c.a;}
		bool operator != (const Color& c) {return r!=c.r || g!=c.g || b!=c.b || a!=c.a;} 
		
		// ******************************************************************
		// other methods
		std::string toString() const  {char c[80]; sprintf(c,"Color4(%f, %f, %f, %f)",r,g,b,a); return std::string(c);}
		void   clamp        ()        {if(r>1)r=1;if(g>1)g=1;if(b>1)b=1;if(a>1)a=1;if(r<0)r=0;if(g<0)g=0;if(b<0)b=0;if(a<0)a=0;}
		Color  clampedCopy  ()        {return Color((r<0)?0:((r>1)?1:r),(g<0)?0:((g>1)?1:g),(b<0)?0:((b>1)?1:b),(a<0)?0:((a>1)?1:a));}
		void   negate       ()        {r=1.0f-r; g=1.0f-g; b=1.0f-b; a=1.0f-a;}
		Color  negatedCopy  () const  {return Color(1.0f-r, 1.0f-g, 1.0f-b, 1.0f-a);}
		float  brightness   () const  {return r * 0.299f + g * 0.587f + b * 0.114f;}
		void   makeFloor(const Color& c)        {if(c.r<r)r=c.r;if(c.g<g)g=c.g;if(c.b<b)b=c.b;if(c.a<a)a=c.a;}
		void   makeCeil (const Color& c)        {if(c.r>r)r=c.r;if(c.g>g)g=c.g;if(c.b>b)b=c.b;if(c.a>a)a=c.a;}
		Color  floorCopy(const Color& c) const  {return Color((r < c.r)?r:c.r, (r < c.r)?g:c.g, (b < c.b)?b:c.b, (a < c.a)?a:c.a);}
		Color  ceilCopy (const Color& c) const  {return Color((r > c.r)?r:c.r, (r > c.r)?g:c.g, (b > c.b)?b:c.b, (a > c.a)?a:c.a);}
		void   makeRandom(float fAlpha = 1.0f)  {(*this) = createRandom(fAlpha);}
		void   makeRandomPlusAlpha()            {(*this) = createRandom(Random::getSingletonPtr()->uniformReal(0.0f,1.0f));}
		static Color createRandom(const float fAlpha = 1.0f) {
			const float r = Random::getSingletonPtr()->uniformReal(0.0f,1.0f);
			const float g = Random::getSingletonPtr()->uniformReal(0.0f,1.0f);
			const float b = Random::getSingletonPtr()->uniformReal(0.0f,1.0f);
			return Color(r,g,b,fAlpha);}
		static void fromColorArrayToByteArray(const Color *colors, unsigned char *bytes, unsigned long num, bool getsAlpha = false);
		static void fromByteArrayToColorArray(const unsigned char *bytes, Color *colors, unsigned long num, bool hasAlpha = false);
		
		static const Color ZERO;
		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color YELLOW;
		static const Color PURPLE;
		static const Color CYAN;
		static const Color ORANGE;
		static const Color PINK;
		static const Color SKY;
		static const Color MELLOW;
		static const Color FOREST;
		static const Color SILVER;
		static const Color GOLD;
	};
	
	// arithmetic operators
	inline Color operator + (const Color& a, const Color& b) {return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);}
	inline Color operator - (const Color& a, const Color& b) {return Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);}
	inline Color operator - (const Color& c)                 {return Color(-c.r, -c.g, -c.b, -c.a);}
	inline Color operator * (const Color& a, const Color& b) {return Color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);}
	inline Color operator * (const Color& c, const float f)  {return Color(c.r * f,   c.g * f,   c.b * f,   c.a * f);}
	inline Color operator * (const float f,  const Color& c) {return Color(c.r * f,   c.g * f,   c.b * f,   c.a * f);}
	inline Color operator / (const Color& a, const Color& b) {return Color(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);}
	inline Color operator / (const Color& c, const float f)  {return Color(c.r / f,   c.g / f,   c.b / f,   c.a / f);}
	
} // namespace Roe

#endif /*ROECOLOR_*/
