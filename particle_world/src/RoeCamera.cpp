#include "RoeCamera.h"
#include "RoeUtil.h"

namespace Roe {
	
	const Camera Camera::STD_CAMERA(0, Vector3(0,0,10), Vector3(0,0,-1), Vector3(0,1,0));
	
	Camera::Camera(unsigned int pflags, Vector3 ppos, Vector3 pdir)
		: flags(pflags)
		, pos  (ppos)
		, dir  (pdir)
	{
		if (flags & CF_CONST_UP_VECTOR) {
			up = Vector3(0,1,0);
		} else {
			if (flags & CF_DIR_IS_ABS) up = Vector3::crossProduct(Vector3::crossProduct(dir-pos, Vector3(0,1,0)), dir-pos);
			else                       up = Vector3::crossProduct(Vector3::crossProduct(dir    , Vector3(0,1,0)), dir    );
		}
	}
	
	void Camera::lookAt (const Vector3& v) {
		if (flags & CF_DIR_IS_ABS) dir = v;
		else                       dir = v-pos;
	}
	void Camera::pitch (float radian) { //x
		radian *= -1; //for CCW
		Vector3 cross;
		if (flags & CF_CONST_UP_VECTOR) {
			static const float variance = 0.01f;
			if (flags & CF_DIR_IS_ABS) {
				const float angle = Vector3::angleBetween(up, dir-pos); //angle for maximal pitching
				if (ROE_RAD_TO_DEG(angle) <= 90.0) {
					dir = pos+Matrix::transformNormal(dir-pos, Matrix::rotationAxis(Vector3::crossProduct(up, dir-pos),
					                                                       (-radian < angle-variance) ? (radian) : (-angle+variance)));
				} else {
					dir = pos+Matrix::transformNormal(dir-pos, Matrix::rotationAxis(Vector3::crossProduct(up, dir-pos),
					                                                       (radian < ROE_PI-angle-variance) ? (radian) : (ROE_PI-angle-variance)));
				}
			} else {
				const float angle = Vector3::angleBetween(up, dir    ); //angle for maximal pitching
				if (ROE_RAD_TO_DEG(angle) <= 90.0f) {
					dir = Matrix::transformNormal(dir, Matrix::rotationAxis(Vector3::crossProduct(up, dir    ),
					                                                        (-radian < angle-variance) ? (radian) : (-angle+variance)));
				} else {
					dir = Matrix::transformNormal(dir, Matrix::rotationAxis(Vector3::crossProduct(up, dir    ),
					                                                       (radian < ROE_PI-angle-variance) ? (radian) : (ROE_PI-angle-variance)));
				}
			}
		} else {
			if (flags & CF_DIR_IS_ABS) {
				cross = Vector3::crossProduct(up, dir-pos);
				dir = pos+Matrix::transformNormal(dir-pos, Matrix::rotationAxis(cross, radian)); //360 degree pitching
				up  =     Matrix::transformNormal(up     , Matrix::rotationAxis(cross, radian)); //the up vector is transformed too
			} else {
				cross = Vector3::crossProduct(up, dir);
				dir = Matrix::transformNormal(dir, Matrix::rotationAxis(cross, radian)); //360 degree pitching
				up  = Matrix::transformNormal(up , Matrix::rotationAxis(cross, radian)); //the up vector is transformed too
			}
		}
	}
	void Camera::yaw   (float radian) { //y
		radian *= -1; //for CCW
		if (flags & CF_DIR_IS_ABS) dir = pos+Matrix::transformNormal(dir-pos, Matrix::rotationAxis( up, radian));
		else                       dir =     Matrix::transformNormal(dir    , Matrix::rotationAxis( up, radian));
	}
	void Camera::roll  (float radian) { //z
		radian *= -1; //for CCW
		if (flags & CF_CONST_UP_VECTOR) return; //up vector shall not be changed
		if (flags & CF_DIR_IS_ABS) up = Matrix::transformNormal(up, Matrix::rotationAxis(dir-pos, radian));
		else                       up = Matrix::transformNormal(up, Matrix::rotationAxis(dir    , radian));
	}
	void Camera::transformPos(const Matrix& m) {pos = Matrix::transformCoords(pos, m);}
	void Camera::transformDir(const Matrix& m) {
		if (flags & CF_DIR_IS_ABS) dir = pos+Matrix::transformNormal(dir-pos, m);
		else                       dir =     Matrix::transformNormal(dir    , m);
		if (!(flags & CF_CONST_UP_VECTOR)) up = Matrix::transformNormal(up, m);
	}
	void Camera::transformCam(const Matrix& m) {
		if (flags & CF_DIR_IS_ABS) dir = pos+Matrix::transformNormal(dir-pos, m);
		else                       dir =     Matrix::transformNormal(dir    , m);
		if (!(flags & CF_CONST_UP_VECTOR)) up  = Matrix::transformNormal(up, m);
		pos = Matrix::transformCoords(pos, m);
	}
	
} // namespace Roe


