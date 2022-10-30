#include "pch.h"
#include "NodeEntity.h"
//#include "EffectGlobal.h"
#include "PBTriangles.h"

#include "NoRenderList.h"

	NodeEntity::NodeEntity() {

		mType = NodeType::Entity;


	}

	
	void NodeEntity::AddMesh(Mesh3D* mesh) {

		mesh->SetOwner(this);
		mMeshes.push_back(mesh);

	}

	std::vector<Mesh3D*> NodeEntity::GetMeshes() {

		return mMeshes;

	}


	Mesh3D* NodeEntity::GetMesh(int id) {

		return mMeshes[id];

	}

	void NodeEntity::RenderDepth() {

		/*
		Kinetic::FX::Global::EffectGlobal::CurrentNode = this;


		glm::mat4 matrix_projection = Kinetic::FX::Global::EffectGlobal::CurrentCamera->GetProjectionMatrix();
		glm::mat4 matrix_view = Kinetic::FX::Global::EffectGlobal::CurrentCamera->GetWorldMatrix();
		glm::mat4 matrix_model = Kinetic::FX::Global::EffectGlobal::CurrentNode->GetWorldMatrix();

		Kinetic::Graph::Nodes::NodeCamera* Cam = Kinetic::FX::Global::EffectGlobal::CurrentCamera;
		Kinetic::Graph::Nodes::NodeLight* Light = Kinetic::FX::Global::EffectGlobal::CurrentLight;


		auto vp = Cam->GetPosition();

		Kinetic::FX::Effect * fx = Node3D::FXDepth;
		
		fx->bind();

		auto vpos = Cam->GetPosition();

		fx->setMat("mProj", matrix_projection);
		fx->setMat("mModel", matrix_model);
		fx->setMat("mView", matrix_view);

		glm::vec3 cp = Cam->GetPosition();
		fx->setVec3("camP", cp.x, cp.y, cp.z);
		fx->setFloat("minZ", Cam->GetMinZ());
		fx->setFloat("maxZ", Cam->GetMaxZ());


		for (int i = 0; i < mMeshes.size(); i++) {

			auto mesh = mMeshes[i];

			//mesh->BindMaterial();
			mesh->BindVBO();
			mesh->DrawVBO();
			mesh->ReleaseVBO();
			//mesh->ReleaseMaterial();


		}

		fx->unbind();
		*/
		RenderDepthChildren();

	}

	void NodeEntity::RenderSmartMesh() {

		RenderChildren();

	}

	void NodeEntity::Render() {
		if (GetEnabled() == false) return;
		for (int i = 0; i < NoRenderList::mNoRender.size(); i++) {

			if (this == NoRenderList::mNoRender[i])
			{
				RenderChildren();
				return;
			}

		};


		
		int a = 0;

		RenderChildren();
	}

	//Anim

	void NodeEntity::ConvertToSmartMesh() {

		

	}

	float3 NodeEntity::GetBounds() {

		float3 bb(0, 0, 0);

		float sx, sy, sz, bx, by, bz;
		sx = sy = sz = 10000;
		bx = by = bz = -10000;

		for (int i = 0; i < mMeshes.size(); i++) {

			auto mesh = mMeshes[i];

			for (int j = 0; j < mesh->NumVertices(); j++) {

				auto v = mesh->GetVertex(j);

				if (v.position.x < sx) sx = v.position.x;
				if (v.position.y < sy) sy = v.position.y;
				if (v.position.z < sz) sz = v.position.z;
				if (v.position.x > bx) bx = v.position.x;
				if (v.position.y > by) by = v.position.y;
				if (v.position.z > bz) bz = v.position.z;


			}

		}

		bb.x = bx - sx;
		bb.y = by - sy;
		bb.z = bz - sz;

		return bb;


	}
	void NodeEntity::SetPhysicsBoxSize(float size) {
		mBody = new PBBox(size,size,size);

		mBody->SetPosition(mPosition);
		mBody->SetRotation(GetRotation());
	}
	void NodeEntity::SetPhysicsBox() {

		
		float3 bb = GetBounds();
		//bb.x = bb.x + 0.1f;
		//bb.y = bb.y + 0.1f;
		//bb.z = bb.z + 0.1f;
		mBody = new PBBox(bb.x*2, bb.y*2, bb.z*2);
	    
		mBody->SetPosition(mPosition);
		mBody->SetRotation(GetRotation());
		
	}

	void NodeEntity::SetPhysicsSphere() {

		
		float3 bb = GetBounds();
		mBody = new PBSphere(bb.y / 2.0f + 0.1f);
		mBody->SetPosition(mPosition);
		mBody->SetRotation(GetRotation());
	}

	void NodeEntity::SetPhysicsCapsule() {

		
		float3 bb = GetBounds();
		mBody = new PBCapsule((bb.y / 2.0f) - 1.1f, bb.x / 2.0f);
		mBody->SetPosition(mPosition);
		mBody->SetRotation(GetRotation());
		
		
	}

	void NodeEntity::SetPhysicsCapsuleSize(float width, float height) {


		float3 bb = GetBounds();
		mBody = new PBCapsule(height, width);
		mBody->SetPosition(mPosition);
		mBody->SetRotation(GetRotation());


	}

	void NodeEntity::SetConstraint(bool x, bool y, bool z) {
		mBody->SetConstraint(x, y, z);
	}

	void NodeEntity::SetPhysicsConvex() {
		mBody = new PBConvex(mMeshes[0]);
		mBody->SetPosition(mPosition);
		mBody->SetRotation(GetRotation());
	
	}


	void NodeEntity::UpdatePhysics() {

		if (mBody == NULL) return;

		mPosition = mBody->GetPosition();
		mRotation = mBody->GetRotation();

	}

	void NodeEntity::SetPosition(float3 position) {

		mPosition = position;
		SetChanged();
		if (mBody == NULL) return;

		mBody->SetPosition(position);
	
	}

	void NodeEntity::SetRotation(float pitch, float yaw, float roll) {

		Node3D::SetRotation(pitch, yaw, roll);
		SetChanged();
		mBody->SetRotation(GetRotation());

	}

	void NodeEntity::SetPhysicsTris() {
		for (int i = 0; i < mMeshes.size(); i++) {

			mTriBody = new PBTriangles(mMeshes, i);


		}
	//	mBody->SetPosition(mPosition);
	}

	Node3D* NodeEntity::Clone() {

		auto clone = new NodeEntity;

		for (int i = 0; i < mMeshes.size(); i++) {

			clone->AddMesh(mMeshes[i]->Clone());

		}

		clone->SetScripts(GetScripts());

		clone->SetName(GetName());
		clone->SetEnabled(GetEnabled());

		return (Node3D*)clone;

	}




