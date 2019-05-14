#include "RoePhysicsEngine.h"

#ifdef ROE_COMPILE_PHYSICS

#include "RoeCommon.h"
#include "RoeMaterial.h"
#include "RoeUtil.h"
#include "RoeTexture.h"

namespace Roe {
namespace Physics {
	
#define MAX_CONTACTS 150
	
	void PhysicsEngine::nearCollisionCallback(void *_this, dGeomID id1, dGeomID id2) {
		PhysicsEngine *engine = (PhysicsEngine*)(_this);
		dBodyID body1 = dGeomGetBody(id1);
		dBodyID body2 = dGeomGetBody(id2);
		
		dContact contact[MAX_CONTACTS];
		
		for (int i = 0; i < MAX_CONTACTS; i++) {
			contact[i].surface.mode =  dContactBounce | dContactSoftCFM; // Bouncy surface
			contact[i].surface.bounce = 0.5; // bounce is the amount of 'bouncyness'
			contact[i].surface.bounce_vel = 0.1; // bounce_vel is the minimum incoming velocity to cause a bounce
			contact[i].surface.mu = dInfinity; // Friction
			contact[i].surface.soft_cfm = 0.00001; // constraint force mixing parameter
		}
		
		int collisions = dCollide(id1, id2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
		if (collisions) {
			for (int i = 0; i < collisions; ++i) {
				dJointID c = dJointCreateContact(engine->m_WorldID, engine->m_ContactJointGroup, &contact[i]);
				dJointAttach(c, body1, body2);
				/*Model mod("arrow2");
				Material::GOLD.render();
				mod.setTransformation(Matrix::rotationY(ROE_DEG_TO_RAD(90.0f))*Matrix::translation(Vector3(contact[i].geom.pos)));
				mod.render();//*/
			}
		}
	}
	
	PhysicsEngine::PhysicsEngine() {
		m_fStepSize = 0.0078f;
		dInitODE();
		m_WorldID = dWorldCreate();
		m_SpaceID = dHashSpaceCreate(0);
		dWorldSetGravity(m_WorldID, 0.0, -9.81, 0.0);
		m_ContactJointGroup = dJointGroupCreate(MAX_CONTACTS);
		
		dWorldSetCFM (m_WorldID, 1e-5);
		dWorldSetERP (m_WorldID, 0.1);
		//dWorldSetAutoDisableFlag(m_WorldID, 1);
		
		//dCreatePlane (m_SpaceID,-0.0,1,0, 0);
		
		// Plane geometry
		/*dGeomID plane;
		const int indexes[6] = {2, 1, 0, 3, 2, 0};
		const dVector3 triVert[4] = {
			{ 10000.0, 0.0,  10000.0},
			{-10000.0, 0.0,  10000.0},
			{-10000.0, 0.0, -10000.0},
			{ 10000.0, 0.0, -10000.0}};
		
		dTriMeshDataID triMesh;
		triMesh = dGeomTriMeshDataCreate();
		dGeomTriMeshDataBuildSimple(triMesh, (dReal*)triVert, 4, (const dTriIndex*)indexes, 6);
		dGeomTriMeshDataSet( triMesh, 1, (void *) (Matrix::IDENTITY.n) );
		plane = dCreateTriMesh(m_SpaceID, triMesh, NULL, NULL, NULL);
		//plane = dCreateBox(m_SpaceID, 10000, 0.01, 10000);
		dGeomSetPosition(plane, 0, 0.0, 0);//*/
	}
	
	PhysicsEngine::~PhysicsEngine() {
		m_mBodies.clear();
		dJointGroupDestroy( m_ContactJointGroup );
		dSpaceDestroy(m_SpaceID);
		dWorldDestroy(m_WorldID);
		dCloseODE();
	}
	
	bool PhysicsEngine::doPhysics()
	{
		dSpaceCollide  ( m_SpaceID, this, &nearCollisionCallback );
		dWorldQuickStep( m_WorldID, m_fStepSize);
		dJointGroupEmpty( m_ContactJointGroup );
		return true;
	}
	
	PhysicsModel PhysicsEngine::createPhysicsTerrain(std::string imagePath, Vector3 vScale) {
		std::string name = "res/tmp/" + Util::removeFileExtensionOfString(imagePath) + ".off";
		Texture::createHeightmapOFF(imagePath, name);
		PhysicsModel::loadOFF(name, false, CW_CCW, Matrix::scaling(vScale));
		Model::loadOFF       (name, false, CW_CCW, 0.0, Matrix::scaling(vScale));
		//Util::removeFile(name);
		PhysicsModel mod(m_WorldID, m_SpaceID, name, false, Model(name, false), -1.0f, Vector3(0,0,0), Vector3::ZERO);
		return mod;
	}
	
} // namespace Physics
} // namespace Roe


#endif //ROE_COMPILE_PHYSICS
