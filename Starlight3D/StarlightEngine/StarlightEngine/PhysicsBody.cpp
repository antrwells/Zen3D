#include "pch.h"
#include "PhysicsBody.h"

#include "Common/interface/BasicMath.hpp"

using namespace Diligent;



	float3 PhysicsBody::GetPosition() {

		auto tm = body->getGlobalPose();

		float3 pos;

		pos.x = tm.p.x;
		pos.y = tm.p.y;
		pos.z = tm.p.z;

		return pos;

	}

	void PhysicsBody::SetConstraint(bool x, bool y, bool z) {

		body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, x);
		body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, y);
		body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, z);

	}

	void PhysXMat4ToglmMat4(const physx::PxMat44& mat4, float4x4& newMat)
	{

		newMat[0][0] = mat4[0][0];
		newMat[0][1] = mat4[0][1];
		newMat[0][2] = mat4[0][2];
		newMat[0][3] = mat4[0][3];

		newMat[1][0] = mat4[1][0];
		newMat[1][1] = mat4[1][1];
		newMat[1][2] = mat4[1][2];
		newMat[1][3] = mat4[1][3];

		newMat[2][0] = mat4[2][0];
		newMat[2][1] = mat4[2][1];
		newMat[2][2] = mat4[2][2];
		newMat[2][3] = mat4[2][3];

		newMat[3][0] = mat4[3][0];
		newMat[3][1] = mat4[3][1];
		newMat[3][2] = mat4[3][2];
		newMat[3][3] = mat4[3][3];

	}

	float4x4 PhysicsBody::GetRotation() {

		auto tm = body->getGlobalPose();

		auto rot = tm.q;

		auto grot =  Quaternion(rot.x, rot.y, rot.z, rot.w);


		return grot.ToMatrix();

		//return res;



	}

	void PhysicsBody::SetPosition(float3 pos) {

		auto tm = body->getGlobalPose();

		tm.p = physx::PxVec3(pos.x, pos.y, pos.z);

		body->setGlobalPose(tm, true);

	}

	void PhysicsBody::Turn(float x, float y, float z) {

		body->addTorque(physx::PxVec3(x, y, z));

	}

	void PhysicsBody::SetAngularForce(float x, float y, float z) {

		body->setAngularVelocity(physx::PxVec3(x, y, z));

	}

	void PhysicsBody::ApplyForce(float x, float y, float z) {

		body->addForce(physx::PxVec3(x, y, z), physx::PxForceMode::eIMPULSE, true);

	}

	void PhysicsBody::ApplyLocalForce(float x, float y, float z) {

		float4x4 r = GetRotation();
		float4 i = float4(x, y, z, 1.0f);

		i = r * i;

		ApplyForce(i.x, i.y, i.z);

	}

	void PhysicsBody::SetForce(float x, float y, float z) {

		float py = body->getLinearVelocity().y;

		body->setLinearVelocity(physx::PxVec3(x, y, z));

	}

	void PhysicsBody::AddLocalForce(float x, float y, float z, float dampen) {

		float3 cf = GetForce();

		cf.x *= dampen;
		cf.z *= dampen;


		float4x4 r = GetRotation();
    	float4 i = float4(x, y, z, 1.0f);

		i = r * i;


		SetForce(cf.x + i.x, cf.y + i.y, cf.z + i.z);

	}

	void PhysicsBody::SetLocalForce(float x, float y, float z) {


		float4x4 r = GetRotation();
		float4 i = float4(x, y, z, 1.0f);

		i = r * i;

		SetForce(i.x, i.y, i.z);

	}

	float3 PhysicsBody::GetForce() {

		auto lf = body->getLinearVelocity();


		return float3(lf.x, lf.y, lf.z);

	}

