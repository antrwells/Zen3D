#include "pch.h"
#include "PBSphere.h"



	PBSphere::PBSphere(float ssize) {

		size = ssize;
		InitBody();
	}

	void PBSphere::InitBody() {

		physx::PxTransform   tm(0, 0, 0);

		material = Physics::GetSDK()->createMaterial(0.5f, 0.5f, 0.6f);


		shape = Physics::GetSDK()->createShape(physx::PxSphereGeometry(size), *material);
		body = Physics::GetSDK()->createRigidDynamic(tm);
		body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
		body->attachShape(*shape);
		physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
		Physics::GetScene()->addActor(*body);

	}

