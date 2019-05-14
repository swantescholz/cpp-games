#include "RoeVector3.h"

namespace Roe {
	
	//special points
	const Vector3 Vector3::ZERO(0,0,0);
	const Vector3 Vector3::X(1,0,0);
	const Vector3 Vector3::Y(0,1,0);
	const Vector3 Vector3::Z(0,0,1);
	const Vector3 Vector3::NX(-1,0,0);
	const Vector3 Vector3::NY(0,-1,0);
	const Vector3 Vector3::NZ(0,0,-1);
	const Vector3 Vector3::ONE(1,1,1);
	const Vector3 Vector3::GRAVITY(0,-9.81,0);
	
} // namespace Roe
