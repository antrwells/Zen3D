#include "pch.h"
#include "PBConvex.h"
#include "NodeEntity.h"


	PBConvex::PBConvex(Mesh3D* ent)
	{
		mesh = ent;
		InitBody();

	}

	void PBConvex::InitBody() {

		physx::PxCookingParams params = Physics::GetCooking()->getParams();

		// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
		params.convexMeshCookingType = physx::PxConvexMeshCookingType::eQUICKHULL;/// convexMeshCookingType;


		// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
		// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
		params.gaussMapLimit = 64;
		Physics::GetCooking()->setParams(params);




		if (mesh->GetTri(0).v0 == 0 && mesh->GetTri(0).v1 == 0) {

			return;
		}

		//	return;

		int vc = (int)mesh->NumVertices();
		//	int tc = mesh->NumTris();

		physx::PxVec3* vertData = new physx::PxVec3[vc];



		int vid = 0;

		//for (int i = 0; i < entity->NumMeshes(); i++) {




		for (int k = 0; k < (int)mesh->NumVertices(); k++) {

			auto vert = mesh->GetVertex(k);
			vertData[vid] = physx::PxVec3(vert.position.x, vert.position.y, vert.position.z);
			vid++;

		}



		physx::PxConvexMeshDesc desc;

		// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
		desc.points.data = vertData;
		desc.points.count = vc;
		desc.points.stride = sizeof(physx::PxVec3);
		desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxU32 meshSize = 0;
		physx::PxConvexMesh* convex = NULL;

		convex = Physics::GetCooking()->createConvexMesh(desc, Physics::GetSDK()->getPhysicsInsertionCallback());

		physx::PxMaterial* mat;

		mat = Physics::GetSDK()->createMaterial(0.5f, 0.5f, 0.6f);

		auto shape = Physics::GetSDK()->createShape(physx::PxConvexMeshGeometry(convex), *mat);;

		physx::PxTransform   tm(0, 0, 0);

		body = Physics::GetSDK()->createRigidDynamic(tm);

		body->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);

		body->attachShape(*shape);


		Physics::GetScene()->addActor(*body);

	}

