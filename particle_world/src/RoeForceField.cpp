#include "RoeForceField.h"
#include "RoeException.h"
#include "RoeRandom.h"

namespace Roe {
	
	ForceField::ForceField(Matrix mTransform) {
		m_mTransform = mTransform;
		m_mInvTransform = Matrix::invert(mTransform);
		m_pDirFunc        = nullptr;
		m_pStrengthFunc   = nullptr;
		m_pCollisionFunc  = nullptr;
		m_bCreatedDirFunc = m_bCreatedStrengthFunc = m_bCreatedCollisionFunc = false;
	}
	ForceField::ForceField(const ForceField& rhs)
	: m_mTransform(rhs.m_mTransform)
	, m_mInvTransform(rhs.m_mInvTransform)
	{
		m_pDirFunc        = rhs.m_pDirFunc;
		m_pStrengthFunc   = rhs.m_pStrengthFunc;
		m_pCollisionFunc  = rhs.m_pCollisionFunc;
		m_bCreatedDirFunc = m_bCreatedStrengthFunc = m_bCreatedCollisionFunc = false;
	}
	ForceField& ForceField::operator=(const ForceField& rhs) {
		m_mTransform = rhs.m_mTransform;
		m_mInvTransform = rhs.m_mInvTransform;
		if (m_bCreatedDirFunc      ) {ROE_SAFE_DELETE(m_pDirFunc);}
		if (m_bCreatedStrengthFunc ) {ROE_SAFE_DELETE(m_pStrengthFunc);}
		if (m_bCreatedCollisionFunc) {ROE_SAFE_DELETE(m_pCollisionFunc);}
		m_bCreatedDirFunc = m_bCreatedStrengthFunc = m_bCreatedCollisionFunc = false;
		return (*this);
	}
	ForceField::~ForceField() {
		if (m_bCreatedDirFunc      ) {ROE_SAFE_DELETE(m_pDirFunc);}
		if (m_bCreatedStrengthFunc ) {ROE_SAFE_DELETE(m_pStrengthFunc);}
		if (m_bCreatedCollisionFunc) {ROE_SAFE_DELETE(m_pCollisionFunc);}
	}
	Vector3 ForceField::calculateInfluence(const Vector3& pos, const Vector3& vel) {
		/*if (m_pDirFunc == nullptr)
			throw Exception("no direction function found", "ForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
		if (m_pStrengthFunc == nullptr)
			throw Exception("no strength function found", "ForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
		if (m_pCollisionFunc == nullptr)
			throw Exception("no collision function found", "ForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
		//*/
		if (!(*m_pCollisionFunc)(pos)) return Vector3::ZERO; //no collision
		
		return (*m_pDirFunc)(pos,vel) * (*m_pStrengthFunc)(pos,vel);
	}
	
	//------------------------------------
	ForceFieldGroup::ForceFieldGroup(Matrix mInvTransform)  : ForceField(mInvTransform) {}
	ForceFieldGroup::ForceFieldGroup(const ForceFieldGroup& rhs)
	: ForceField(rhs)
	{
		clear();
		for (auto it = rhs.begin(); it != rhs.end(); ++it) {
			addForceField(*(*it));
		}
	}
	ForceFieldGroup& ForceFieldGroup::operator=(const ForceFieldGroup& rhs) {
		ForceField::operator=(rhs);
		for (auto it = rhs.begin(); it != rhs.end(); ++it) {
			addForceField(*(*it));
		}
		return (*this);
	}
	ForceFieldGroup::~ForceFieldGroup() {
		clear();
	}
	void ForceFieldGroup::clear() {
		for (auto it = begin(); it != end(); ++it) {
			delete (*it);
		}
		std::list<ForceField*>::clear();
	}
	Vector3 ForceFieldGroup::calculateInfluence(const Vector3& pos, const Vector3& vel) {
		Vector3 acc(0,0,0);
		for (auto it = begin(); it != end(); ++it) {
			acc += (*it)->influence(pos, vel);
		}
		return acc;
	}
	/*
	//------------------------
	SphereForceField::SphereForceField(Vector3 center, double r) 
	: ForceField(Matrix::scaling(Vector3(r,r,r))*Matrix::translation(center))
	{}
	SphereForceField::SphereForceField(Matrix mTransform) 
	: ForceField(mTransform)
	{}
	SphereForceField::SphereForceField(const SphereForceField& rhs)
	: ForceField(rhs)
	{}
	SphereForceField& SphereForceField::operator=(const SphereForceField& rhs) {
		ForceField::operator=(rhs);
		return (*this);
	}
	SphereForceField::~SphereForceField() {}
	Vector3 SphereForceField::calculateInfluence(Vector3 pos, Vector3 vel) {
		if (Util::isPointInSphere(pos, Vector3::ZERO, 1.0) == false) return Vector3::ZERO;
		if (m_pDirFunc != nullptr && m_pStrengthFunc != nullptr) {
			return (*m_pDirFunc)(pos,vel) * (*m_pStrengthFunc)(pos,vel);
		}
		if (*m_ppDirFunc != nullptr && *m_ppStrengthFunc != nullptr) {
			return (**m_ppDirFunc)(pos,vel) * (**m_ppStrengthFunc)(pos,vel);
		}
		throw Exception("no direction or strength function found", "SphereForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
	}
	
	//------------------------
	CylinderForceField::CylinderForceField(Vector3 center, Vector3 dir, double r) 
	: ForceField(Matrix::scaling(Vector3(r,dir.length(),r))*Matrix::rotationFromTo(Vector3::Y, dir)*Matrix::translation(center))
	{}
	CylinderForceField::CylinderForceField(Matrix mTransform) 
	: ForceField(mTransform)
	{}
	CylinderForceField::CylinderForceField(const CylinderForceField& rhs)
	: ForceField(rhs)
	{}
	CylinderForceField& CylinderForceField::operator=(const CylinderForceField& rhs) {
		ForceField::operator=(rhs);
		return (*this);
	}
	CylinderForceField::~CylinderForceField() {}
	Vector3 CylinderForceField::calculateInfluence(Vector3 pos, Vector3 vel) {
		if (pos.y > 1.0 || pos.y < -1.0) return Vector3::ZERO;
		if (Vector3(pos.x,0.0,pos.z).squaredLength() > 1.0) return Vector3::ZERO;
		if (m_pDirFunc != nullptr && m_pStrengthFunc != nullptr) {
			return (*m_pDirFunc)(pos,vel) * (*m_pStrengthFunc)(pos,vel);
		}
		if (*m_ppDirFunc != nullptr && *m_ppStrengthFunc != nullptr) {
			return (**m_ppDirFunc)(pos,vel) * (**m_ppStrengthFunc)(pos,vel);
		}
		throw Exception("no direction or strength function found", "CylinderForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
	}
	
	//------------------------
	BoxForceField::BoxForceField(Vector3 center, Vector3 size, Vector3 dir) 
	: ForceField(Matrix::scaling(size)*Matrix::rotationFromTo(Vector3::Y, dir)*Matrix::translation(center))
	{}
	BoxForceField::BoxForceField(Vector3 center, double a, Vector3 dir) 
	: ForceField(Matrix::scaling(Vector3(a/2.0, dir.length(), a/2.0))*Matrix::rotationFromTo(Vector3::Y, dir)*Matrix::translation(center))
	{}
	BoxForceField::BoxForceField(Vector3 center, double x, double z, Vector3 dir) 
	: ForceField(Matrix::scaling(Vector3(x/2.0, dir.length(), z/2.0))*Matrix::rotationFromTo(Vector3::Y, dir)*Matrix::translation(center))
	{}
	BoxForceField::BoxForceField(Matrix mTransform) 
	: ForceField(mTransform)
	{}
	BoxForceField::BoxForceField(const BoxForceField& rhs)
	: ForceField(rhs)
	{}
	BoxForceField& BoxForceField::operator=(const BoxForceField& rhs) {
		ForceField::operator=(rhs);
		return (*this);
	}
	BoxForceField::~BoxForceField() {}
	Vector3 BoxForceField::calculateInfluence(Vector3 pos, Vector3 vel) {
		if (pos.x > 1.0 || pos.x < -1.0) return Vector3::ZERO;
		if (pos.y > 1.0 || pos.y < -1.0) return Vector3::ZERO;
		if (pos.z > 1.0 || pos.z < -1.0) return Vector3::ZERO;
		if (m_pDirFunc != nullptr && m_pStrengthFunc != nullptr) {
			return (*m_pDirFunc)(pos,vel) * (*m_pStrengthFunc)(pos,vel);
		}
		if (*m_ppDirFunc != nullptr && *m_ppStrengthFunc != nullptr) {
			return (**m_ppDirFunc)(pos,vel) * (**m_ppStrengthFunc)(pos,vel);
		}
		throw Exception("no direction or strength function found", "BoxForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
	}
	
	//------------------------
	TorusForceField::TorusForceField(double rSmall, Vector3 center, double rBig, Vector3 dir)
	: ForceField(Matrix::scaling(Vector3(rBig,rBig,rBig))*Matrix::rotationFromTo(Vector3::Y, dir)*Matrix::translation(center))
	, m_r(rSmall)
	{}
	TorusForceField::TorusForceField(double rSmall, Matrix mTransform) 
	: ForceField(mTransform)
	, m_r(rSmall)
	{}
	TorusForceField::TorusForceField(const TorusForceField& rhs)
	: ForceField(rhs)
	{}
	TorusForceField& TorusForceField::operator=(const TorusForceField& rhs) {
		ForceField::operator=(rhs);
		return (*this);
	}
	TorusForceField::~TorusForceField() {}
	Vector3 TorusForceField::calculateInfluence(Vector3 pos, Vector3 vel) {
		if ((Vector3::normalize(Vector3(pos.x,0.0,pos.z)) - pos).squaredLength() > m_r*m_r) return Vector3::ZERO;
		
		if (m_pDirFunc != nullptr && m_pStrengthFunc != nullptr) {
			return (*m_pDirFunc)(pos,vel) * (*m_pStrengthFunc)(pos,vel);
		}
		if (*m_ppDirFunc != nullptr && *m_ppStrengthFunc != nullptr) {
			return (**m_ppDirFunc)(pos,vel) * (**m_ppStrengthFunc)(pos,vel);
		}
		throw Exception("no direction or strength function found", "TorusForceField::calculateInfluence", "RoeForceField.cpp", ROE_LINE);
	}
	*/
	
} //namespace Roe
