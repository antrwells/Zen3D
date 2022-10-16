#pragma once
#include <ctype.h>


	class Physics
	{
	public:


		Physics();
		//static physx::PxPhysics* GetSDK()
		//{
	//		return Main->gPhysics;
//		}
		//static physx::PxScene* GetScene() {
		//	return Main->scene;
		//}
		//static physx::PxCooking* GetCooking() {

			//return Main->gCooking;

//		}
		void Simulate(float time);

	private:

	//	physx::PxFoundation* gFoundation;
		//physx::PxPhysics* gPhysics;
	//	physx::PxPvd* gPvd = NULL;
	//	physx::PxScene* scene;
		//physx::PxSceneDesc* sceneDesc;
		//physx::PxSceneDesc oScene;
	//	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
	//	physx::PxCooking* gCooking;
	};
