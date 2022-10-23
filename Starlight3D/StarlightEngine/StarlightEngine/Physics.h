#pragma once
#include <ctype.h>

#include "PxPhysicsAPI.h"

	class Physics
	{
	public:
		static Physics* Main;

		Physics();
		static physx::PxPhysics* GetSDK()
		{
			return Main->gPhysics;
		}
		static physx::PxScene* GetScene() {
			return Main->scene;
		}
		static physx::PxCooking* GetCooking() {

			return Main->gCooking;

		}
		void Simulate(float time);

		void RemoveStatic(physx::PxRigidStatic* body)
		{
			body->release();
			//scene->removeActor(body->release());

		}

		void Remove(physx::PxRigidDynamic* body)
		{
			body->release();
			//scene->removeActor(body);
		}

	private:

		physx::PxFoundation* gFoundation;
		physx::PxPhysics* gPhysics;
		physx::PxPvd* gPvd = NULL;
		physx::PxScene* scene;
		//physx::PxSceneDesc* sceneDesc;
		//physx::PxSceneDesc oScene;
		physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
		physx::PxCooking* gCooking;
	};
