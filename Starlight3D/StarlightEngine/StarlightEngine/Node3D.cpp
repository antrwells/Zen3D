#include "pch.h"
#include "Node3D.h"

//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtx/transform.hpp"
#include "Maths.h"

#include "ScriptObject.h"
#include "ZScriptContext.h"
#include "ZContextVar.h"
#include "ZSource.h"
#include "ZTokenizer.h"
#include "ZParser.h"
#include "ZClassNode.h"
#include "ZSystemFunction.h"
#include "ZSystemFunctions.h"
#include "Application.h"
#include "ZContextVar.h"

bool Node3D::mSysInit = false;

	Node3D::Node3D() {

		mRotation = float4x4::Identity();

		//mRotation = glm::mat4(1.0f);

		mPosition = float3(0, 0, 0);
		mScale = float3(1, 1, 1);
		mComponents.resize(0);
		mType = NodeType::Node;
		if (mSysInit == false) {
			AddSystemFunctions();
			mSysInit = true;
		}

		

	}

	void Node3D::Update() {

		for (int i = 0; i < mScriptObjs.size(); i++)
		{
			auto obj = mScriptObjs[i];

			obj->CallUpdate();
		}

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



			float4x4 previous_matrix = float4x4::Identity();
			 
			if (mRootNode != nullptr) {

				previous_matrix = mRootNode->GetWorldMatrix();

			}

			//float4x4 scale_matrix = float4x4::Scale(mScale);
			float4x4 position_matrix;
			float4x4 rot_matrix;
			float4x4 final_matrix;
			float4x4 scale_matrix = float4x4::Scale(mScale);
			if (mType == NodeType::Camera) {

				position_matrix = float4x4::Translation(mPosition).Inverse();
				rot_matrix = mRotation;
				final_matrix = position_matrix * rot_matrix;// *scale_matrix;
			}
			else {
				position_matrix = float4x4::Translation(mPosition);
				rot_matrix = mRotation;
				final_matrix = scale_matrix*rot_matrix * position_matrix;// *scale_matrix;// *scale_matrix;
			}


			 

			


			mValidTransform = previous_matrix * final_matrix;

			mTransformInvalidated = false;

			return previous_matrix * final_matrix;

		
		

	

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
		//InvalidateTransform();


	}

	void Node3D::SetRotation(float pitch, float yaw, float roll) {

		
		
		pitch = Maths::Deg2Rad(pitch);
		yaw = Maths::Deg2Rad(yaw);
		roll = Maths::Deg2Rad(roll);

		float4x4 pitch_matrix = float4x4::RotationX(pitch);
		float4x4 yaw_matrix = float4x4::RotationY(yaw);
		float4x4 roll_matrix = float4x4::RotationZ(roll);


		mRotation = yaw_matrix * pitch_matrix * roll_matrix;
		
		//InvalidateTransform();
	}

	void Node3D::SetScale(float3 scale) {

		mScale = scale;
	//	InvalidateTransform();

	}

	void Node3D::Move(float3 move) {

		if (mType == NodeType::Camera)
		{
			float4 mv = mRotation * float4(move, 1.0f);


			mPosition += float3(mv.x, mv.y, mv.z);
		}
		else {

			float4 mv = float4(move, 1.0f) * mRotation;

			mPosition += float3(mv.x, mv.y, mv.z);

		}
		//InvalidateTransform();
	}

	NodeProperty* Node3D::AddProperty(NodeProperty* _property) {

		mProperties.push_back(_property);
		return _property;

	}

	NodeProperty* Node3D::GetProperty(std::string name) {

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

	void Node3D::RotateLocal(float pitch, float yaw, float roll) {

		pitch = Maths::Deg2Rad(pitch);
		yaw = Maths::Deg2Rad(yaw);
		roll = Maths::Deg2Rad(roll);

		float4x4 pitch_matrix = float4x4::RotationX(pitch);
		float4x4 yaw_matrix = float4x4::RotationY(yaw);
		float4x4 roll_matrix = float4x4::RotationZ(roll);


		mRotation = (yaw_matrix * pitch_matrix * roll_matrix) * mRotation;

		InvalidateTransform();

	}


	void Node3D::RotateGlobal(float pitch, float yaw, float roll) {

		pitch = Maths::Deg2Rad(pitch);
		yaw = Maths::Deg2Rad(yaw);
		roll = Maths::Deg2Rad(roll);

		float4x4 pitch_matrix = float4x4::RotationX(pitch);
		float4x4 yaw_matrix = float4x4::RotationY(yaw);
		float4x4 roll_matrix = float4x4::RotationZ(roll);


		mRotation = mRotation *(yaw_matrix * pitch_matrix * roll_matrix);


		InvalidateTransform();

	}

	void Node3D::SetRotation4x4(float4x4 rotation) {

		mRotation = rotation;
		InvalidateTransform();

	}

	void Node3D::AddScript(std::string path,std::string cls_name) {

		ScriptObject* new_obj = new ScriptObject;
		auto con = ZScriptContext::CurrentContext;
		
		new_obj->mContext = con;

		auto src = new ZSource(path);
		ZTokenizer* toker = new ZTokenizer(src);
		auto stream = toker->Tokenize();
		auto parser = new ZParser(stream);
		ZMainNode* node = parser->Parse();

		new_obj->mContext->AddNode(node);

		auto v1 = VMakeC((void*)this);

		new_obj->mMainClass = new_obj->mContext->CreateInstance(cls_name, cls_name + "Instance",{v1});

		auto n_var = new_obj->mMainClass->FindVar("Node");
	
		n_var->SetCObj((void*)this);
	
	  

		int vb = 5;

	//	int aa = 5;
	
		//new_obj->CallInit();

		new_obj->name = cls_name;
		mScriptObjs.push_back(new_obj);
		int aa = 5;
	
	}
	
	//Kinetic::FX::Effect* Node3D::FXDepth = nullptr;
	//Kinetic::FX::Effect* Node3D::FXDepthAnim = nullptr;

	std::vector<NodeProperty*> Node3D::GetProperties() {

		return mProperties;

	}


	void Node3D::BeginNode() {

		mPushPos = mPosition;
		mPushRot = mRotation;
		mPushScale = mScale;

		printf("Node begun.");
		for (int i = 0; i < mScriptObjs.size(); i++)
		{
			auto obj = mScriptObjs[i];

			obj->PushVars();
			obj->CallInit();
		}

	}

	void Node3D::EndNode() {

		SetPosition(mPushPos);
		SetScale(mPushScale);
		SetRotation(mPushRot);

	
		for (int i = 0; i < mScriptObjs.size(); i++)
		{

			auto obj = mScriptObjs[i];
			obj->PopVars();
			

		}

	}

	Node3D* getNode(ZContextVar* v)
	{
		return (Node3D*)v->GetCObj();
	}


	ZContextVar* node3d_turn(const std::vector<ZContextVar*>& args)
	{

		auto node = (Node3D*)args[0]->GetCObj();

		node->RotateGlobal(VGetInt(args[1]), VGetInt(args[2]), VGetInt(args[3]));

		int bb = 5;

		return nullptr;
	}

	ZContextVar* node3d_setrot(const std::vector<ZContextVar*>& args)
	{

		auto node = getNode(args[0]);

		node->SetRotation(args[1]->GetFloatVal(), args[2]->GetFloatVal(), args[3]->GetFloatVal());

		return nullptr;

	}

	ZContextVar* node3d_getpos(const std::vector<ZContextVar*>& args)
	{

		auto nc = (Node3D*)args[0]->GetCObj();

		
		auto pv = ZScriptContext::CurrentContext->CreateInstance("Vec3", "v", {});

		pv->FindVar("x")->SetFloat(nc->GetPosition().x);
		pv->FindVar("y")->SetFloat(nc->GetPosition().y);
		pv->FindVar("z")->SetFloat(nc->GetPosition().z);

		int v = 0;

		return VMakeClass(pv);
	}

	ZContextVar* node3d_setpos(const std::vector<ZContextVar*>& args)
	{
		auto nc = (Node3D*)args[0]->GetCObj();

		auto pos = args[1]->GetClassVal();

		auto x = pos->FindVar("x")->GetFloatVal();
		auto y = pos->FindVar("y")->GetFloatVal();
		auto z = pos->FindVar("z")->GetFloatVal();

		nc->SetPosition(float3(x, y, z));

		int a = 5;

		return nullptr;

	}

	ZContextVar* node3d_getmousemovex(const std::vector<ZContextVar*>& args)
	{

		return VMakeFloat(Application::GetApp()->GetInput()->GetMouseDX());

	}

	ZContextVar* node3d_getmousemovey(const std::vector<ZContextVar*>& args)
	{

		return VMakeFloat(Application::GetApp()->GetInput()->GetMouseDY());

	}

	//----- SYSTEM FUNCTIONS -> ZSCRIPT

	void Node3D::AddSystemFunctions() {

		ZScriptContext* con1 = new ZScriptContext();

		con1->LoadLib("math");
		con1->LoadLib("scene");
		con1->LoadLib("input");

		auto funcs = ZScriptContext::CurrentContext->GetSysFuncs();
		
		//Node Functions
		ZSystemFunction n_turn("Node3DTurn", node3d_turn);
		ZSystemFunction n_getpos("Node3DGetPos", node3d_getpos);
		ZSystemFunction n_setpos("Node3DSetPos", node3d_setpos);
		ZSystemFunction n_setrot("Node3DSetRotation", node3d_setrot);
		ZSystemFunction n_getmousemovex("Node3DGetMouseMoveX", node3d_getmousemovex);
		ZSystemFunction n_getmousemovey("Node3DGetMouseMoveY", node3d_getmousemovey);

		funcs->RegisterFunction(n_turn);
		funcs->RegisterFunction(n_getpos);
		funcs->RegisterFunction(n_setpos);
		funcs->RegisterFunction(n_setrot);
		funcs->RegisterFunction(n_getmousemovex);
		funcs->RegisterFunction(n_getmousemovey);
		

		int aa=5;
	}