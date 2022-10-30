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
#include "NodeEntity.h"
#include "SceneGraph.h"
#include "RayPicker.h"
#include "ZContextVar.h"
#include "SceneGlobal.h"
#include "GameUI.h"
#include "VideoDecoder.h"
#include "Audio.h"
bool Node3D::mSysInit = false;

	Node3D::Node3D() {

		mRotation = float4x4::Identity();

		//mRotation = glm::mat4(1.0f);
		mName = "Root";
		mPosition = float3(0, 0, 0);
		mScale = float3(1, 1, 1);
		mComponents.resize(0);
		mType = NodeType::Node;
		if (mSysInit == false) {
			AddSystemFunctions();
			mSysInit = true;
		}
		mChanged = true;
		SetChanged();
		mEnabled = true;
		

	}

	void Node3D::RenderUI() {

		if (mEnabled == false) return;
		for (int i = 0; i < mScriptObjs.size(); i++)
		{
			auto obj = mScriptObjs[i];

			obj->CallUI();
		}

	}

	void Node3D::Update() {

		if (mEnabled == false) return;
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
		if (mEnabled == false) return;
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
				final_matrix = scale_matrix * rot_matrix * position_matrix;// *scale_matrix;// *scale_matrix;
			}


			 

			


			mValidTransform = previous_matrix * final_matrix;

			mTransformInvalidated = false;

			return  final_matrix * previous_matrix;

		
		

	

	}

	void Node3D::AddComponent(NodeComponent* component) {

		mComponents.push_back(component);

	}

	void Node3D::SetRootNode(Node3D* node) {

		mRootNode = node;
		InvalidateTransform();

	}

	void Node3D::AddNode(Node3D* node) {

		if (node->GetRootNode() != nullptr) {
			auto rn = node->GetRootNode();
			rn->RemoveNode(node);
		}
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
		mChanged = true;
		SetChanged();


	}

	void Node3D::SetRotation(float pitch, float yaw, float roll) {

		
		
		pitch = Maths::Deg2Rad(pitch);
		yaw = Maths::Deg2Rad(yaw);
		roll = Maths::Deg2Rad(roll);

		float4x4 pitch_matrix = float4x4::RotationX(pitch);
		float4x4 yaw_matrix = float4x4::RotationY(yaw);
		float4x4 roll_matrix = float4x4::RotationZ(roll);


		mRotation = yaw_matrix * pitch_matrix * roll_matrix;
	//	mRotation = mRotation.Inverse();
		mChanged = true;
		SetChanged();
		//InvalidateTransform();
	}

	void Node3D::SetScale(float3 scale) {

		mScale = scale;
		mChanged = true;
		SetChanged();
	//	InvalidateTransform();

	}

	void Node3D::Move(float3 move) {

		if (mType == NodeType::Camera)
		{
			float4 mv = mRotation * float4(move, 1.0f);// *mRotation;

			mPosition += float3(mv.x, mv.y, mv.z);
		}
		else {

			float4 mv = float4(move, 1.0f) *mRotation;

			mPosition += float3(mv.x, mv.y, mv.z);

		}
		SetChanged();
		mChanged = true;
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
		mChanged = true;
		SetChanged();
		UpdateRotation();
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
		mChanged = true;
		SetChanged();
		UpdateRotation();
	}

	void Node3D::SetRotation4x4(float4x4 rotation) {

		mRotation = rotation;
		InvalidateTransform();
		mChanged = true;
		SetChanged();

	}

	ScriptObject* Node3D::AddScript(std::string path,std::string cls_name) {

		ScriptObject* new_obj = new ScriptObject;
		auto con = ZScriptContext::CurrentContext;
		
		new_obj->mContext = con;
		new_obj->mPath = path;
		new_obj->mClsName = cls_name;

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
		return new_obj;
	}
	
	//Kinetic::FX::Effect* Node3D::FXDepth = nullptr;
	//Kinetic::FX::Effect* Node3D::FXDepthAnim = nullptr;

	std::vector<NodeProperty*> Node3D::GetProperties() {

		return mProperties;

	}


	void Node3D::BeginNode() {

		if (mEnabled == false) return;
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
		if (mEnabled == false) return;
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

		
		auto pv = ZScriptContext::CurrentContext->CreateInstance("Vec3", "v", {VMakeFloat(nc->GetPosition().x),VMakeFloat(nc->GetPosition().y),VMakeFloat(nc->GetPosition().z)});

		//pv->FindVar("x")->SetFloat(nc->GetPosition().x);
		//pv->FindVar("y")->SetFloat(nc->GetPosition().y);
		//pv->FindVar("z")->SetFloat(nc->GetPosition().z);


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

	ZContextVar* node3d_setpybox(const std::vector<ZContextVar*>& args)
	{

		auto nc = (Node3D*)args[0]->GetCObj();
		
		float size = args[1]->GetFloatVal();

		NodeEntity* ent = (NodeEntity*)nc;
	
		ent->SetPhysicsBoxSize(size);

		return nullptr;

	}

	ZContextVar* node3d_setpycylinder(const std::vector<ZContextVar*>& args)
	{

		auto nc = (Node3D*)args[0]->GetCObj();

		float height = args[1]->GetFloatVal();
		float width = args[2]->GetFloatVal();

		NodeEntity* ent = (NodeEntity*)nc;

		ent->SetPhysicsCapsuleSize(height,width);

		return nullptr;

	}

	ZContextVar* node3d_setpytris(const std::vector<ZContextVar*>& args)
	{

		auto nc = (Node3D*)args[0]->GetCObj();

	

		NodeEntity* ent = (NodeEntity*)nc;

		ent->SetPhysicsTris();

		return nullptr;

	}


	ZContextVar* node3d_setpyconvex(const std::vector<ZContextVar*>& args)
	{

		auto nc = (Node3D*)args[0]->GetCObj();

		NodeEntity* ent = (NodeEntity*)nc;

		ent->SetPhysicsConvex();

		return nullptr;

	}

	ZContextVar* node_GetName(const std::vector<ZContextVar*>& args)
	{
		int a = 5;
		auto node =(Node3D*) args[0]->GetCObj();

		return VMakeString(node->GetName());

	}

	void SetVec3(ZClassNode* v,float3 p)
	{
		v->FindVar("x")->SetFloat(p.x);
		v->FindVar("y")->SetFloat(p.y);
		v->FindVar("z")->SetFloat(p.z);
	}

	// GameScene - Bindings

	ZContextVar* gs_loadScene(const std::vector<ZContextVar*>& args)
	{

		std::string path = args[0]->GetStringVal();

		auto ms = SceneGraph::GetMainScene();

		ms->SetRoot(new Node3D);
		ms->LoadGraph(path);


		return nullptr;
	}

	ZContextVar* gs_raycast(const std::vector<ZContextVar*>& args) {

		auto origin = args[0]->GetClassVal();
		auto dest = args[1]->GetClassVal();
		auto ignore = (Node3D*)args[2]->GetCObj();

		
		float3 v_origin = float3(origin->FindVar("x")->GetFloatVal(), origin->FindVar("y")->GetFloatVal(), origin->FindVar("z")->GetFloatVal());
		float3 v_dest = float3(dest->FindVar("x")->GetFloatVal(),dest->FindVar("y")->GetFloatVal(), dest->FindVar("z")->GetFloatVal());
		rpRay ray;
		ray.pos = v_origin;
		ray.dir = v_dest;
		auto res = SceneGraph::mThis->mRayPick->RayPick(ray,ignore);

		auto hit = VMakeClass(ZScriptContext::CurrentContext->CreateInstance("RayHit", "res", {}));

		auto cls = hit->GetClassVal();

		if (res.hit) {
	//		printf("Hit!!!\n");
			cls->FindVar("Hit")->SetInt(1);
			auto hp = cls->FindVar("HitPosition")->GetClassVal();
			SetVec3(hp, res.hit_point);
			SetVec3(cls->FindVar("HitOrigin")->GetClassVal(), v_origin);
			cls->FindVar("HitDistance")->SetFloat(res.hit_distance);
			
			auto hcls = VMakeClass(ZScriptContext::CurrentContext->CreateInstance("Node3D", "resnh", {}));
			auto nh = hcls->GetClassVal()->FindVar("Node");
			nh->SetCObj(res.hit_node);


			cls->FindVar("HitNode")->SetClass(hcls->GetClassVal());




		}
		else {
			cls->FindVar("Hit")->SetInt(0);
		}

		return hit;



	}

	ZContextVar* gs_getNode(const std::vector<ZContextVar*>& args) {

		auto node = SceneGlobal::mCurrentScene->FindNode(args[0]->GetStringVal());

		int aa = 5;

		auto  n3 = VMakeClass(ZScriptContext::CurrentContext->CreateInstance("Node3D", "res", {}));
	
		n3->GetClassVal()->FindVar("Node")->SetCObj(node);
		


		return n3;
	}

	// General

	ZContextVar* sys_random(const std::vector<ZContextVar*>& args)
	{

		float min = args[0]->GetFloatVal();
		float max = args[1]->GetFloatVal();

		float r3 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));

		return VMakeFloat(r3);

	}

	// GameUI

	ZContextVar* ui_button(const std::vector<ZContextVar*>& args) {


		std::string text = args[0]->GetStringVal();
		int x, y, w, h;
		x = args[1]->GetIntVal();
		y = args[2]->GetIntVal();
		w = args[3]->GetIntVal();
		h = args[4]->GetIntVal();

		bool res = GameUI::UI->Button(text, x, y, w, h);

		if (res) {
			return VMakeInt(1);
		}
		else {
			return VMakeInt(0);
		}
	}

	ZContextVar* ui_image(const std::vector<ZContextVar*>& args)
	{
		int x, y, w, h;
		auto img = (Texture2D*)args[0]->GetClassVal()->FindVar("Image")->GetCObj();
		x = args[1]->GetIntVal();
		y = args[2]->GetIntVal();
		w = args[3]->GetIntVal();
		h = args[4]->GetIntVal();
		float r, g, b, a;
		r = args[5]->GetFloatVal();
		g = args[6]->GetFloatVal();
		b = args[7]->GetFloatVal();
		a = args[8]->GetFloatVal();


		GameUI::UI->Image(img, x, y, w, h,r,g,b,a);

		return nullptr;
	}

	ZContextVar* ui_displayWidth(const std::vector<ZContextVar*>& args) {

		return VMakeInt(Application::GetApp()->GetWidth());

	}

	ZContextVar* ui_displayHeight(const std::vector<ZContextVar*>& args)
	{

		return VMakeInt(Application::GetApp()->GetHeight());
	}
	//image

	ZContextVar* img_load(const std::vector<ZContextVar*>& args) {

		Texture2D* img = new Texture2D(args[0]->GetStringVal().c_str());
	
		return VMakeC((void*)img);

	}

	//sound/song

	ZContextVar* sound_PlaySong(const std::vector<ZContextVar*>& args)
	{

		Audio* audio = new Audio;
		audio->PlaySong(args[0]->GetStringVal().c_str());
		return nullptr;
	}


	// Video

	ZContextVar* video_Start(const std::vector<ZContextVar*>& args)
	{
		auto path = args[0]->GetStringVal();

		VideoDecoder* decoder = new VideoDecoder;
		decoder->OpenVideo(path.c_str());


		return VMakeC((void*)decoder);
	}

	ZContextVar* video_NextFrame(const std::vector<ZContextVar*>& args)
	{
		auto video = (VideoDecoder*)args[0]->GetCObj();
		video->GetCurrentFrame();
		return VMakeC((void*)video->GetCurrentImage());
	}

	ZContextVar* util_DrawTexture2D(const std::vector<ZContextVar*>& args)
	{
		int x, y, w, h;
		auto img = (Texture2D*)args[0]->GetCObj();
		if (img == nullptr) return nullptr;

		x = args[1]->GetIntVal();
		y = args[2]->GetIntVal();
		w = args[3]->GetIntVal();
		h = args[4]->GetIntVal();
		float r, g, b, a;
		r = args[5]->GetFloatVal();
		g = args[6]->GetFloatVal();
		b = args[7]->GetFloatVal();
		a = args[8]->GetFloatVal();
		GameUI::UI->Image(img, x, y, w, h, r, g, b, a);
		return nullptr;
	}

	//----- SYSTEM FUNCTIONS -> ZSCRIPT



	void Node3D::AddSystemFunctions() {

		ZScriptContext* con1 = new ZScriptContext();

		con1->LoadLib("math");
		con1->LoadLib("scene");
		con1->LoadLib("input");
		con1->LoadLib("ui");
		con1->LoadLib("sound");
		con1->LoadLib("video");

		auto funcs = ZScriptContext::CurrentContext->GetSysFuncs();
		
		//Node Functions
		ZSystemFunction n_turn("Node3DTurn", node3d_turn);
		ZSystemFunction n_getpos("Node3DGetPos", node3d_getpos);
		ZSystemFunction n_setpos("Node3DSetPos", node3d_setpos);
		ZSystemFunction n_setrot("Node3DSetRotation", node3d_setrot);
		ZSystemFunction n_getmousemovex("Node3DGetMouseMoveX", node3d_getmousemovex);
		ZSystemFunction n_getmousemovey("Node3DGetMouseMoveY", node3d_getmousemovey);
		ZSystemFunction n_setpybox("Node3DSetPyToBox", node3d_setpybox);
		ZSystemFunction n_setpytris("Node3DSetPyTriMesh", node3d_setpytris);
		ZSystemFunction n_setcap("Node3DSetPyToCylinder", node3d_setpycylinder);
		ZSystemFunction n_setpyconvex("Node3DSetPyToConvex", node3d_setpyconvex);
		ZSystemFunction n_GetName("Node3DGetName", node_GetName);
		ZSystemFunction gs_raycast("GameSceneRayCast", gs_raycast);
		ZSystemFunction gs_getNode("GameSceneGetNode", gs_getNode);
		ZSystemFunction sys_rnd("random", sys_random);
		ZSystemFunction gui_button("GUIButton", ui_button);
		ZSystemFunction gui_image("GUIImage", ui_image);
		ZSystemFunction i_load("GUILoadImage", img_load);
		ZSystemFunction ui_disWidth("GUIDisplayWidth", ui_displayWidth);
		ZSystemFunction ui_disHeight("GUIDisplayHeight", ui_displayHeight);
		ZSystemFunction s_playSong("MusicPlaySong", sound_PlaySong);
		ZSystemFunction gs_loadscene("GameSceneLoadScene", gs_loadScene);
		ZSystemFunction v_play("VideoStart", video_Start);
		ZSystemFunction v_nextFrame("VideoNextFrame", video_NextFrame);
		ZSystemFunction util_DrawTexture("UtilDrawTexture", util_DrawTexture2D);
		funcs->RegisterFunction(n_turn);
		funcs->RegisterFunction(n_getpos);
		funcs->RegisterFunction(n_setpos);
		funcs->RegisterFunction(n_setrot);
		funcs->RegisterFunction(n_getmousemovex);
		funcs->RegisterFunction(n_getmousemovey);
		funcs->RegisterFunction(n_setpybox);
		funcs->RegisterFunction(n_setpytris);
		funcs->RegisterFunction(n_setpyconvex);
		funcs->RegisterFunction(n_GetName);
		funcs->RegisterFunction(gs_raycast);
		funcs->RegisterFunction(sys_rnd);
		funcs->RegisterFunction(gs_getNode);
		funcs->RegisterFunction(gui_button);
		funcs->RegisterFunction(i_load);
		funcs->RegisterFunction(gui_image);
		funcs->RegisterFunction(ui_disWidth);
		funcs->RegisterFunction(ui_disHeight);
		funcs->RegisterFunction(s_playSong);
		funcs->RegisterFunction(gs_loadscene);
		funcs->RegisterFunction(n_setcap);
		funcs->RegisterFunction(v_play);
		funcs->RegisterFunction(v_nextFrame);
		funcs->RegisterFunction(util_DrawTexture);
		int aa=5;
	}


	void Node3D::WriteScripts(VFile* file) {

		file->WriteInt(mScriptObjs.size());

		for (int i = 0; i < mScriptObjs.size(); i++) {

			auto obj = mScriptObjs[i];

			file->WriteString(obj->mPath.c_str());
			file->WriteString(obj->mClsName.c_str());

			WriteClass(file,obj->mMainClass);

			


		}

	}

	void Node3D::ReadScripts(VFile* file) {

		int snum = file->ReadInt();
		for (int i = 0; i < snum; i++) {

			std::string spath = std::string(file->ReadString());
			std::string cls_name = std::string(file->ReadString());

			auto obj = AddScript(spath, cls_name);
			
			ReadClass(file, obj->mMainClass);




		}

	}

	void Node3D::WriteClass(VFile* file, ZClassNode* cls) {
		
		auto vars = cls->GetVars();

		for (int i = 0; i < vars.size(); i++)
		{
			auto v = vars[i];
			//file->WriteInt((int)v->GetType());


			switch ((int)v->GetType()) {
			case 0:
				file->WriteInt(v->GetIntVal());
				break;
			case 1:
				file->WriteFloat(v->GetFloatVal());
				break;
			case 2:
				file->WriteString(v->GetStringVal().c_str());
				break;
			case 4:
				if (v->GetClassVal() == nullptr)
				{
					continue;
				}
				WriteClass(file, v->GetClassVal());

				break;
			}

		}

	}

	ZClassNode* Node3D::ReadClass(VFile* file,ZClassNode* cls) {


		auto vars = cls->GetVars();


		for (int i = 0; i < vars.size(); i++)
		{
			auto v = vars[i];
			//file->WriteInt((int)v->GetType());


			switch ((int)v->GetType()) {
			case 0:
				//file->WriteInt(v->GetIntVal());
				v->SetInt(file->ReadInt());
				break;
			case 1:
				//file->WriteFloat(v->GetFloatVal());
				v->SetFloat(file->ReadFloat());
				break;
			case 2:
				v->SetString(std::string(file->ReadString()));
				//file->WriteString(v->GetStringVal().c_str());
				break;
			case 4:
				if (v->GetClassVal() == nullptr)
				{
					continue;
				}
				ReadClass(file, v->GetClassVal());
				//v->SetClass(ReadClass(file));


			}

		}
		return nullptr;
	}