#ifndef ROEPHYSICSENGINE_H_
#define ROEPHYSICSENGINE_H_

#include "RoeCommon.h"

#ifdef ROE_COMPILE_PHYSICS

#include <ode/ode.h>
#include <string>
#include <map>
#include "RoeVector3.h"
#include "RoePhysicsModel.h"

namespace Roe {
namespace Physics {
	
	
	class PhysicsEngine {
	private:
		static void nearCollisionCallback( void* _this, dGeomID id1, dGeomID id2 ); //near collision function
	public:
		PhysicsEngine();
		~PhysicsEngine();
		
		void setStepSize(float f) {m_fStepSize = f;}
		bool doPhysics();
		PhysicsModel createPhysicsTerrain(std::string imagePath, Vector3 vScale = Vector3(1,1,1)); //creates a physical terrain from an image
		
		dWorldID getWorldID() {return m_WorldID;}
		dSpaceID getSpaceID() {return m_SpaceID;}
	private:
		
		std::map<std::string, dBodyID> m_mBodies;
		dWorldID m_WorldID;
		dSpaceID m_SpaceID;
		dJointGroupID m_ContactJointGroup;
		float m_fStepSize;
	};
	
} // namespace Physics
} // namespace Roe

#endif //ROE_COMPILE_PHYSICS
#endif /* ROEPHYSICSENGINE_H_ */
