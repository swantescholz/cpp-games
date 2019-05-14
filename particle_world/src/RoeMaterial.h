#ifndef ROEMATERIAL_H_
#define ROEMATERIAL_H_

#include <GL/gl.h>
#include "RoeColor.h"

namespace Roe {

	class Material
	{
	public:
		Material(Color pAmb  = Color(1,1,1,1),Color pDiff = Color(1,1,1,1),
		         Color pSpec = Color(1,1,1,1),float pfShininess = 0.0f)
		: a(pAmb), b(pDiff), c(pSpec), s(pfShininess) {}
		Material(const Material& m) : a(m.a),b(m.b),c(m.c),s(m.s) {}
		~Material() {}
		
		Color a; //ambient
		Color b; //diffuse
		Color c; //specular
		float s; //shininess
		
		static const Material BLACK;
		static const Material WHITE, WHITE_AMB;
		static const Material RED, RED_AMB;
		static const Material GREEN, GREEN_AMB;
		static const Material BLUE, BLUE_AMB;
		static const Material PURPLE, PURPLE_AMB;
		static const Material YELLOW, YELLOW_AMB;
		static const Material CYAN, CYAN_AMB;
		static const Material ORANGE, ORANGE_AMB;
		static const Material PINK, PINK_AMB;
		static const Material SKY, SKY_AMB;
		static const Material MELLOW, MELLOW_AMB;
		static const Material FOREST, FOREST_AMB;
		static const Material SILVER;
		static const Material GOLD;
		
		void render() const {
			glMaterialfv(GL_FRONT, GL_AMBIENT , a.rg);
			glMaterialfv(GL_FRONT, GL_DIFFUSE , b.rg);
			glMaterialfv(GL_FRONT, GL_SPECULAR, c.rg);
			glMaterialf (GL_FRONT, GL_SHININESS, s);
		}
		
		std::string toString() const;
		
		// assignment operators
		Material& operator  = (const Material& m) {a  = m.a; b  = m.b; c  = m.c; s  = m.s; return *this;}
		Material& operator += (const Material& m) {a += m.a; b += m.b; c += m.c; s += m.s; return *this;}
		Material& operator -= (const Material& m) {a -= m.a; b -= m.b; c -= m.c; s -= m.s; return *this;}
		Material& operator *= (const Material& m) {a *= m.a; b *= m.b; c *= m.c; s *= m.s; return *this;}
		Material& operator *= (const float f)     {a *= f  ; b *= f  ; c *= f  ; s *= f  ; return *this;}
		Material& operator /= (const Material& m) {a /= m.a; b /= m.b; c /= m.c; s /= m.s; return *this;}
		Material& operator /= (const float f)     {a /= f  ; b /= f  ; c /= f  ; s /= f  ; return *this;}
	};
	
	// arithmetic operators
	inline Material operator + (const Material& ma, const Material& mb) {return Material( ma.a + mb.a, ma.b + mb.b, ma.c + mb.c, ma.s + mb.s);}
	inline Material operator - (const Material& ma, const Material& mb) {return Material( ma.a - mb.a, ma.b - mb.b, ma.c - mb.c, ma.s - mb.s);}
	inline Material operator - (const Material& mc                    ) {return Material(-mc.a       ,-mc.b       ,-mc.c       , mc.s       );}
	inline Material operator * (const Material& ma, const Material& mb) {return Material( ma.a * mb.a, ma.b * mb.b, ma.c * mb.c, ma.s * mb.s);}
	inline Material operator * (const Material& mc, const float f     ) {return Material( mc.a * f   , mc.b * f   , mc.c * f   , mc.s * f   );}
	inline Material operator * (const float f     , const Material& mc) {return Material( mc.a * f   , mc.b * f   , mc.c * f   , mc.s * f   );}
	inline Material operator / (const Material& ma, const Material& mb) {return Material( ma.a / mb.a, ma.b / mb.b, ma.c / mb.c, ma.s / mb.s);}
	inline Material operator / (const Material& mc, const float f     ) {return Material( mc.a / f   , mc.b / f   , mc.c / f   , mc.s / f   );}
	
} // namespace Roe

#endif /*ROEMATERIAL_H_*/
