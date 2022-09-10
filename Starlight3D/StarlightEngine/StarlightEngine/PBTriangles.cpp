#include "pch.h"
#include "PBTriangles.h"
#include "NodeEntity.h"
//PBTriangles(WorldEntity* entity) {



	// Setup common cooking params
	void setupCommonCookingParams(physx::PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData)
	{
		// we suppress the triangle mesh remap table computation to gain some speed, as we will not need it 
	// in this snippet
		params.suppressTriangleMeshRemapTable = true;

		// If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking. The input mesh must be valid. 
		// The following conditions are true for a valid triangle mesh :
		//  1. There are no duplicate vertices(within specified vertexWeldTolerance.See PxCookingParams::meshWeldTolerance)
		//  2. There are no large triangles(within specified PxTolerancesScale.)
		// It is recommended to run a separate validation check in debug/checked builds, see below.

		if (!skipMeshCleanup)
			params.meshPreprocessParams &= ~static_cast<physx::PxMeshPreprocessingFlags>(physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
		else
			params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

		// If DISABLE_ACTIVE_EDGES_PREDOCOMPUTE is set, the cooking does not compute the active (convex) edges, and instead 
		// marks all edges as active. This makes cooking faster but can slow down contact generation. This flag may change 
		// the collision behavior, as all edges of the triangle mesh will now be considered active.
		if (!skipEdgeData)
			params.meshPreprocessParams &= ~static_cast<physx::PxMeshPreprocessingFlags>(physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
		else
			params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	}


	//}

	PBTriangles::PBTriangles(std::vector<Mesh3D*>meshes, int meshid) {

		auto mesh = meshes[meshid];

		if (mesh->GetTri(0).v0 == 0 && mesh->GetTri(0).v1 == 0) {

			return;
		}

		//	return;

		int vc = mesh->NumVertices();
		int tc = mesh->NumTris();

		physx::PxVec3* vertData = new physx::PxVec3[vc];



		int vid = 0;

		//for (int i = 0; i < entity->NumMeshes(); i++) {




		for (int k = 0; k < mesh->NumVertices(); k++) {

			auto vert = mesh->GetVertex(k);
			vertData[vid] = physx::PxVec3(vert.position.x, vert.position.y, vert.position.z);
			vid++;

		}




		physx::PxU32* triData = new physx::PxU32[tc * 3];

		int tid = 0;


		for (int k = 0; k < mesh->NumTris(); k++) {

			auto tri = mesh->GetTri(k);

			triData[tid] = (physx::PxU32)tri.v0;
			triData[tid + 1] = (physx::PxU32)tri.v2;
			triData[tid + 2] = (physx::PxU32)tri.v1;
			tid += 3;

		}


		physx::PxTriangleMeshDesc meshDesc;

		meshDesc.points.count = vc;
		meshDesc.points.data = vertData;
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.triangles.count = tc;
		meshDesc.triangles.data = triData;
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);

		physx::PxCookingParams params = Physics::GetCooking()->getParams();

		params.midphaseDesc = physx::PxMeshMidPhase::eBVH33;

		setupCommonCookingParams(params, false, false);

		params.midphaseDesc.mBVH33Desc.meshCookingHint = physx::PxMeshCookingHint::eSIM_PERFORMANCE;

		params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.55f;

		Physics::GetCooking()->setParams(params);

		auto res = Physics::GetCooking()->validateTriangleMesh(meshDesc);

		triMesh = Physics::GetCooking()->createTriangleMesh(meshDesc, Physics::GetSDK()->getPhysicsInsertionCallback());

		//triMesh->release();
		//while (true) {}

		physx::PxMaterial* mat;

		mat = Physics::GetSDK()->createMaterial(0.5f, 0.5f, 0.6f);

		auto shape = Physics::GetSDK()->createShape(physx::PxTriangleMeshGeometry(triMesh), *mat);;

		physx::PxTransform   tm(0, 0, 0);

		sbody = Physics::GetSDK()->createRigidStatic(tm);

		sbody->attachShape(*shape);

		Physics::GetScene()->addActor(*sbody);

		//physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);






	}

