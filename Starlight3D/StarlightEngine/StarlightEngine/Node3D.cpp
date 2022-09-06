#include "pch.h"
#include "Node3D.h"

//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtx/transform.hpp"
//#include "Maths.h"





	Node3D::Node3D() {

		mRotation = float4x4::Identity();

		//mRotation = glm::mat4(1.0f);

		mPosition = float3(0, 0, 0);
		mScale = float3(1, 1, 1);
		mComponents.resize(0);

	

	}

	void Node3D::Update() {

		UpdateComponents();

		UpdateChildren();
	}

	void Node3D::UpdateComponents() {

		for (int i = 0; i < mComponents.size(); i++) {

			if (mComponents[i]->Playing()) {
				mComponents[i]->Update();
			}

		}

	}

	void Node3D::StartComponents() {

		for (int i = 0; i < mComponents.size(); i++) {

			if (!mComponents[i]->Playing()) {
				mComponents[i]->Start();
			}

		}

		for (int i = 0; i < mChildren.size(); i++) {

			mChildren[i]->StartComponents();

		}

	}

	void Node3D::EndComponents() {

		for (int i = 0; i < mComponents.size(); i++) {

			if (mComponents[i]->Playing()) {
				mComponents[i]->End();
			}


		}

		for (int i = 0; i < mChildren.size(); i++) {

			mChildren[i]->EndComponents();

		}


	}

	void Node3D::UpdateChildren() {

		for (int i = 0; i < mChildren.size(); i++) {

			auto child = mChildren[i];
			child->Update();

		}

	}

	float4x4 Node3D::GetWorldMatrix() {

		if (mTransformInvalidated) {

			float4x4 previous_matrix = float4x4::Identity();

			if (mRootNode != nullptr) {

				previous_matrix = mRootNode->GetWorldMatrix();

			}

			float4x4 scale_matrix = float4x4::Scale(mScale);

			float4x4 position_matrix = float4x4::Translation(mPosition);

			float4x4 final_matrix = position_matrix * mRotation * scale_matrix;

			mValidTransform = previous_matrix * final_matrix;

			mTransformInvalidated = false;

			return previous_matrix * final_matrix;

		}
		else {

			return mValidTransform;

		}

	

	}

	void Node3D::AddComponent(NodeComponent* component) {

		mComponents.push_back(component);

	}

	void Node3D::SetRootNode(Node3D* node) {

		mRootNode = node;
		InvalidateTransform();

	}

	void Node3D::AddNode(Node3D* node) {

		mChildren.push_back(node);
		node->SetRootNode(this);

	}

	float3 Node3D::GetPosition() {

		return mPosition;

	}

	//Position/Rotation/Scale
	void Node3D::SetPosition(float3 position) {

		mPosition = position;
		InvalidateTransform();

	}

	void Node3D::SetRotation(float pitch, float yaw, float roll) {

		//***
		/*
		pitch = Kinetic::Maths::Deg2Rad(pitch);
		yaw = Kinetic::Maths::Deg2Rad(yaw);
		roll = Kinetic::Maths::Deg2Rad(roll);

		glm::mat4 pitch_matrix = glm::rotate(pitch, glm::vec3(1.0, 0, 0));
		glm::mat4 yaw_matrix = glm::rotate(yaw, glm::vec3(0, 1.0, 0));
		glm::mat4 roll_matrix = glm::rotate(roll, glm::vec3(0, 0, 1.0));

		mRotation = yaw_matrix * pitch_matrix * roll_matrix;
		*/
		InvalidateTransform();
	}

	void Node3D::SetScale(float3 scale) {

		mScale = scale;
		InvalidateTransform();

	}

	void Node3D::Move(float3 move) {

		float4 mv = mRotation * float4(move,1.0f);


		mPosition += float3(mv.x, mv.y, mv.z);
		InvalidateTransform();
	}

	Kinetic::Graph::Data::NodeProperty* Node3D::AddProperty(Kinetic::Graph::Data::NodeProperty* _property) {

		mProperties.push_back(_property);
		return _property;

	}

	Kinetic::Graph::Data::NodeProperty* Node3D::GetProperty(std::string name) {

		for (int i = 0; i < mProperties.size(); i++) {

			auto _property = mProperties[i];

			if (_property->GetName() == name) {
				return _property;
			}

		}

		return nullptr;
	}

	const char* Node3D::GetName() {
		return mName;
	}

	void Node3D::SetName(const char* name) {

		mName = name;

	}

	//Kinetic::FX::Effect* Node3D::FXDepth = nullptr;
	//Kinetic::FX::Effect* Node3D::FXDepthAnim = nullptr;

