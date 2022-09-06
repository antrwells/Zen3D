#include "pch.h"
#include "NodeBillboard.h"


	NodeBillboard::NodeBillboard(Texture2D* map, float size,bool useZ)
	{

		mMesh = new Mesh3D;

		Vertex* v1, * v2, * v3, * v4;

		v1 = new Vertex();
		v2 = new Vertex();
		v3 = new Vertex();
		v4 = new Vertex();

		if (useZ) {
			v1->position = float3(-size / 2,0, -size / 2);
			v2->position = float3(size / 2,0, -size / 2);
			v3->position = float3(size / 2,0, size / 2);
			v4->position = float3(-size / 2,0, size / 2);
		}
		else {
			v1->position = float3(-size / 2, -size / 2, 0);
			v2->position = float3(size / 2, -size / 2, 0);
			v3->position = float3(size / 2, size / 2, 0);
			v4->position = float3(-size / 2, size / 2, 0);
		}

		v1->texture_coord = float3(0, 0, 0);
		v2->texture_coord = float3(1, 0, 0);
		v3->texture_coord = float3(1, 1, 0);
		v4->texture_coord = float3(0, 1, 0);

		v1->color = float4(1, 1, 1, 1);
		v2->color = float4(1, 1, 1, 1);
		v3->color = float4(1, 1, 1, 1);
		v4->color = float4(1, 1, 1, 1);

		auto mat = new Material;

		mat->SetType(MaterialType::FullBright);

		mat->SetColorMap(map);

		mMesh->SetMaterial(mat);

		mMesh->AddVertex(*v1,true);
		mMesh->AddVertex(*v2,true);
		mMesh->AddVertex(*v3,true);
		mMesh->AddVertex(*v4, true);

		Tri* tri1, * tri2;

		tri1 = new Tri;
		tri2 = new Tri;

		tri1->v0 = 0;
		tri1->v1 = 1;
		tri1->v2 = 2;

		tri2->v0 = 2;
		tri2->v1 = 3;
		tri2->v2 = 0;

		mMesh->AddTri(*tri1);
		mMesh->AddTri(*tri2);

		//mMesh->CreateVBO();





	}

	void NodeBillboard::Render() {

	//	Kinetic::FX::Global::EffectGlobal::CurrentNode = this;




		auto mesh = mMesh;

		

		mesh->BindMaterial();
		mesh->BindVBO();
		mesh->DrawVBO();
		mesh->ReleaseVBO();
		mesh->ReleaseMaterial();




	}

	void NodeBillboard::SetColor(float3 col) {

		auto vertices = mMesh->GetVertices();

		for (int i = 0; i < vertices.size(); i++) {

			auto vertex = vertices[i];

			vertex.color = float4(col.x, col.y, col.z, 1.0);

			vertices[i] = vertex;

		}

		mMesh->SetVertices(vertices);
		mMesh->UpdateVBO();

	}

	void NodeBillboard::SetColors(float4 col) {

		SetColor(float3(col.x, col.y, col.z));



	}

