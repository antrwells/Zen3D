#include "pch.h"
#include "PBCapsule.h"


	PBCapsule::PBCapsule(float height, float radius) {

		Height = height;
		Radius = radius;
		InitBody();

	}

	void PBCapsule::InitBody() {

		physx::PxTransform   tm(0, 0, 0);

		material = Physics::GetSDK()->createMaterial(0.4f, 0.4f, 0.02f);

		physx::PxTransform ps(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
		shape = Physics::GetSDK()->createShape(physx::PxCapsuleGeometry(Radius, Height), *material);



		shape->setLocalPose(ps);

		body = Physics::GetSDK()->createRigidDynamic(tm);
		body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
		body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
		body->attachShape(*shape);
		physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		Physics::GetScene()->addActor(*body);


	}
