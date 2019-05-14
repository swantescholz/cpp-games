#include "RoePlane.h"

namespace Roe {
	
	const Plane Plane::X_PLANE  (1,0,0,0);
	const Plane Plane::Y_PLANE  (0,1,0,0);
	const Plane Plane::Z_PLANE  (0,0,1,0);
	const Plane Plane::XY_PLANE (1,1,0,0);
	const Plane Plane::XZ_PLANE (1,0,1,0);
	const Plane Plane::YZ_PLANE (0,1,1,0);
	const Plane Plane::XYZ_PLANE(1,1,1,0);
	
	Plane Plane::createByPoints(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
		return Plane(v1, (v3 - v2).crossProduct(v1 - v2));
	}
	
} // namespace Roe
