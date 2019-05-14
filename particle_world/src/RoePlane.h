#ifndef ROEPLANE_H_
#define ROEPLANE_H_

#include <cmath>
#include <string>
#include "RoeVector3.h"

namespace Roe {

	class Plane
	{
	public:
		// variables
		Vector3 n; // normal vector
		float   d; // distance
		
		// constructors
		Plane()                                                                                      {}
		Plane(const Plane& p) : n(p.n), d(p.d)                                                       {}
		Plane(const float _a, const float _b, const float _c, const float _d) : n(_a, _b, _c), d(_d) {}
		Plane(const float* pfValue) : n(pfValue[0], pfValue[1], pfValue[2]), d(pfValue[3])           {}
		Plane(const Vector3& _n, float _d) : n(_n), d(_d)                                            {}
		Plane(const Vector3& _p, const Vector3& _n) : n(_n), d(-_n.x*_p.x-_n.y*_p.y-_n.z*_p.z)       {} //plane from point and normal
		
		// assignment operators
		Plane& operator = (const Plane& p) {n = p.n; d = p.d; return *this;}
		
		//turn plane
		inline Plane operator -() {return Plane(-n.x,-n.y,-n.z,d);}
		
		// comparison operations
		bool operator == (const Plane& p) {return n == p.n && d == p.d;}
		bool operator != (const Plane& p) {return n != p.n || d != p.d;}
		
		// ******************************************************************
		// other methods
		std::string toString()  const      {char c[80]; sprintf(c,"Plane(%f, %f, %f, %f)",n.x,n.y,n.z,d); return std::string(c);}
		void  normalize     ()             {const float fLength = n.length(); n /= fLength; d /= fLength;}
		Plane normalizedCopy()  const      {const float fLength = n.length(); return Plane(n / fLength, d / fLength);}
		float dotNormal(const Vector3& v) const {return n.x * v.x + n.y * v.y + n.z * v.z;}     //dot product of v and n of plane
		float dotCoords(const Vector3& v) const {return n.x * v.x + n.y * v.y + n.z * v.z + d;} //plug point in plane formula
		float distance (const Vector3& v) const {return normalizedCopy().dotCoords(v);}         //distance to plane, +->in front of plane,- -> behind
		Vector3 nearestPoint(const Vector3& v) const {return v-dotCoords(v)*n;} //returns the point on plane that is nearest to v
		
		static Plane   createByPoints(const Vector3& v1, const Vector3& v2, const Vector3& v3); //plane from 3 points (works CCW)
		static Vector3 projectPointOntoPlane(const Vector3& v, const Plane& p) { //returns point on plane which is nearest to v
			return v + (-Vector3::normalize(p.n) * p.distance(v));
		}
		static const Plane X_PLANE;
		static const Plane Y_PLANE;
		static const Plane Z_PLANE;
		static const Plane XY_PLANE;
		static const Plane XZ_PLANE;
		static const Plane YZ_PLANE;
		static const Plane XYZ_PLANE;
	};
	
} // namespace Roe

#endif /*ROEPLANE_H_*/
