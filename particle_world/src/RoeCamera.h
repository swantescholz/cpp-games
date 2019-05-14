#ifndef ROECAMERA_H_
#define ROECAMERA_H_

#include "RoeVector3.h"
#include "RoeMatrix.h"
#include <string>

namespace Roe {

	class Camera
	{
	public:
		enum ECameraFlag {
			CF_DIR_IS_ABS      = 1,    //the direction vector is not relative, but absolute
			CF_CONST_UP_VECTOR = 2     //you can not pitch 360 degree (no weightless effect), yaws always around y-axis!
		};
		
		Camera(unsigned int pflags, Vector3 ppos, Vector3 pdir, Vector3 pup)
			: flags(pflags)
			, pos  (ppos)
			, dir  (pdir)
			, up   (pup) {}
		Camera(unsigned pflags = 0, Vector3 ppos = Vector3(0,0,0), Vector3 pdir = Vector3(0,0,-1)); //this constructor calculates the up-vector adequately
		~Camera() {}
		
		unsigned int flags; //extra flags, std: 0
		Vector3 pos;    //always absolute value
		Vector3 dir;    //either the relative direction or the absolute position you look at (you can choose)
		Vector3 up;     //always relative value
		static const Camera STD_CAMERA; //a simple standard camera (0,0,10), (0,0,-1), (0,1,0)
		
		void lookAt (const Vector3& v); //lets the camera look to v
		void pitch  (float radian);     //rotation around local x-axis CCW (up)
		void yaw    (float radian);     //rotation around local y-axis CCW (right)
		void roll   (float radian);     //rotation around local z-axis CCW (left)
		void transformPos(const Matrix& m); //transforms pos vector
		void transformDir(const Matrix& m); //transforms dir and up vector (transforms up EVEN if CF_PITCH_TRANS_UP_VECTOR is inactive)
		void transformCam(const Matrix& m); //transforms dir and up and pos vector
		
		std::string  toString() const {return std::string("Camera(") + pos.toString() + ", " + dir.toString() + ", " + up.toString() + " )";}
		unsigned int getFlags() const {return flags;}
		bool         isDirAbs() const {return flags & CF_DIR_IS_ABS;}
		bool         hasConstUpVector() const {return flags & CF_CONST_UP_VECTOR;}
		Vector3      getAbsDir() const {return (flags&CF_DIR_IS_ABS)?dir:(pos+dir);}
		Vector3      getRelDir() const {return (flags&CF_DIR_IS_ABS)?(dir-pos):dir;}
	};
	
} // namespace Roe

#endif /* ROECAMERA_H_ */
