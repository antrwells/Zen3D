#include "pch.h"
#include "PhysicsBody.h"

#include "Common/interface/BasicMath.hpp"

using namespace Diligent;



	float3 PhysicsBody::GetPosition() {


		return float3();

	}

	void PhysicsBody::SetConstraint(bool x, bool y, bool z) {

		

	}



	float4x4 PhysicsBody::GetRotation() {

		
		//return res;


		return float4x4();
	}

	void PhysicsBody::SetPosition(float3 pos) {


	}

	void PhysicsBody::Turn(float x, float y, float z) {

	}

	void PhysicsBody::SetAngularForce(float x, float y, float z) {

	

	}

	void PhysicsBody::ApplyForce(float x, float y, float z) {

		

	}

	void PhysicsBody::ApplyLocalForce(float x, float y, float z) {

		float4x4 r = GetRotation();
		float4 i = float4(x, y, z, 1.0f);

		i = r * i;

		ApplyForce(i.x, i.y, i.z);

	}

	void PhysicsBody::SetForce(float x, float y, float z) {

		

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

		return float3();

	}

