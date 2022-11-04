#include "pch.h"
#include "PhysicsBody.h"

#include "Common/interface/BasicMath.hpp"

using namespace Diligent;


float qqx, qqy, qqz, qqw;

inline Quaternion fromMatrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
	// Use the Graphics Gems code, from 
	// ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z
	float x, y, z, w;
	float t = m00 + m11 + m22;
	// we protect the division by s by ensuring that s>=1
	if (t >= 0) { // by w
		float s = sqrt(t + 1);
		w = 0.5f * s;
		s = 0.5f / s;
		x = (m21 - m12) * s;
		y = (m02 - m20) * s;
		z = (m10 - m01) * s;
	}
	else if ((m00 > m11) && (m00 > m22)) { // by x
		float s = sqrt(1 + m00 - m11 - m22);
		x = s * 0.5f;
		s = 0.5f / s;
		y = (m10 + m01) * s;
		z = (m02 + m20) * s;
		w = (m21 - m12) * s;
	}
	else if (m11 > m22) { // by y
		float s = sqrt(1 + m11 - m00 - m22);
		y = s * 0.5f;
		s = 0.5f / s;
		x = (m10 + m01) * s;
		z = (m21 + m12) * s;
		w = (m02 - m20) * s;
	}
	else { // by z
		float s = sqrt(1 + m22 - m00 - m11);
		z = s * 0.5f;
		s = 0.5f / s;
		x = (m02 + m20) * s;
		y = (m21 + m12) * s;
		w = (m10 - m01) * s;
	}
	Quaternion q(x, y, z, w);
	qqx = x;
	qqy = y;
	qqz = z;
	qqw = w;
	return q;
}


//-------------


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

	physx::PxMat33 tomat(float3x3 mat)
	{

		physx::PxMat33 r;

		r[0][0] = mat[0][0];
		r[0][1] = mat[0][1];
		r[0][2] = mat[0][2];
		r[1][0] = mat[1][0];
		r[1][1] = mat[1][1];
		r[1][2] = mat[1][2];
		r[2][0] = mat[2][0];
		r[2][1] = mat[2][1];
		r[2][2] = mat[2][2];

		return r;

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

	void PhysicsBody::SetRotation(float3x3 rot)
	{
		
		auto pos = body->getGlobalPose();
		
		physx::PxTransform trans;
		trans.p = pos.p;

		auto rotm = tomat(rot);
		

		trans.q = physx::PxQuat(rotm);
		
	
//		auto qr = fromMatrix(rot.m00,rot.m01,rot.m02,rot.m10,rot.m11,rot.m12,rot.m20,rot.m21,rot.m22);
		

	//	trans.q = physx::PxQuat(qqx, qqy, qqz, qqw);

		body->setGlobalPose(trans, true);

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

