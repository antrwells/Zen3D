#include "pch.h"
#include "Physics.h"
using namespace physx;


	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

#define PVD_HOST "127.0.0.1"

	Physics* Physics::Main = nullptr;
	static PxFilterFlags filterShader(
		PxFilterObjectAttributes attributes0,
		PxFilterData filterData0,
		PxFilterObjectAttributes attributes1,
		PxFilterData filterData1,
		PxPairFlags& pairFlags,
		const void* constantBlock,
		PxU32 constantBlockSize)
	{
		pairFlags = PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
		return PxFilterFlags();
	}


	Physics::Physics() {

		Main = this;
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
		gPvd = PxCreatePvd(*gFoundation);
		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		//PxRegisterHeightFields(*gPhysics);

		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

		gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));



		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -19.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
		sceneDesc.filterShader = filterShader;
		sceneDesc.ccdMaxPasses = 4;
		sceneDesc.solverType = physx::PxSolverType::eTGS;
		sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
		scene = gPhysics->createScene(sceneDesc);
		PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}



		//sceneDesc = &sceneDesc2;
		//oScene = sceneDesc2;
		//sceneDesc = &oScene;
		printf("Physics created successfully.\n");
		//while(true){}

	}

	void Physics::Simulate(float time) {

		scene->simulate(time / 60.0f);
		scene->fetchResults(true);
		

	}

