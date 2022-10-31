#pragma once
#include <vector>
#include "Node3D.h"
#include "NodeCamera.h"
#include "NodeLight.h"
#include "NodeEntity.h"
//#include "Effect.h"
//#include "Physics.h"
#include "NodeBillboard.h"
#include "MeshRenderer.h"
#include "BigBuffer.h"
#include "VFile.h"

#include "SmartDraw.h"


struct TexItem {

	Texture2D* color;
	Texture2D* normal;
	Texture2D* specular;

};

class CubeRenderer;
class GameUI;
class RayPicker;

/// <summary>
/// Vivid::Graph is the namespace for the Vivid3D scene graph, this is what allows you to construct/render and update game scenes.
/// </summary>

	/// <summary>
	/// SceneGraph is the base class for any type of scene graph.
	/// </summary>
	class SceneGraph
	{
	public:

		/// <summary>
		/// Initializes an empty scene Graph.
		/// </summary>
		SceneGraph();

		void ClearNodes() {

			mRootNode->ClearNodes();

		}
		void AddCamera(NodeCamera* cam)
		{
			mCams.push_back(cam);
			AddNode(cam);
		}
		void AddNodeTemp(Node3D* node);

		/// <summary>
		/// Will update the scene graph by calling each node's update method.
		/// </summary>
		void Update();


		Node3D* FindNode(std::string name);

		/// <summary>
		/// Internal use.
		/// </summary>
		void StartComponents();
		/// <summary>
		/// internal use.
		/// </summary>
		void EndComponents();
		/// <summary>
		/// Internal use.
		/// </summary>
		void PauseComponents();
		/// <summary>
		/// Internal use.
		/// </summary>
		void ResumeComponents();
		/// <summary>
		/// Internal use.
		/// </summary>
		void SaveState();
		/// <summary>
		/// Internal use.
		/// </summary>
		void RestoreState();

		/// <summary>
		/// Internal use.
		/// </summary>
		/// <param name="node"></param>
		void UpdateNodePhysics(Node3D* node);
		void UpdatePhyscs()
		{
			UpdateNodePhysics(mRootNode);
		}
		/// <summary>
		/// Internal use.
		/// </summary>
		/// <param name="light"></param>
		void DrawShadowMap(NodeLight* light);
		/// <summary>
		/// Internal use.
		/// </summary>
		void RenderShadowMaps();

		/// <summary>
		/// Internal use.
		/// </summary>
		void RenderDepth();

		/// <summary>
		/// Internal use.
		/// </summary>
		/// <param name="node"></param>
		void CheckRemove(Node3D* node);

		/// <summary>
		/// Pre-Render is to generate/render any framebuffers needed for the final render, such as CubeMaps and ShadowMaps
		/// </summary>
		void PreRender();

		/// <summary>
		/// Will render the final scene.
		/// </summary>
		void Render();

		void RenderTextures();

		void RenderNormals();

		void RenderPositions();

		/// <summary>
		/// Will render anything needed post-render.
		/// </summary>
		void PostRender();


		/// <summary>
		/// Adds a node to the root node.
		/// </summary>
		/// <param name="node"></param>
		void AddNode(Node3D* node);

		/// <summary>
		/// Returns the active camera being used by the SceneGraph
		/// </summary>
		/// <returns></returns>
		NodeCamera* GetCamera();

		/// <summary>
		/// Sets the active camera.
		/// </summary>
		/// <param name="cam"></param>
		void SetCamera(NodeCamera* cam) {
			mCam = cam;
		}

		//lights

		std::vector<NodeLight*> GetLights() {

			return mLights;

		}

		void SetLights(std::vector<NodeLight*> lights) {
			mLights = lights;
		}

		/// <summary>
		/// Adds a light to the scene. You can use as many as you'd like.
		/// </summary>
		/// <param name="light"></param>
		void AddLight(NodeLight* light,bool addtoscene=true);

		/// <summary>
		/// Adds a billboard to the scene.
		/// </summary>
		/// <param name="billboard"></param>
		void AddBillboard(NodeBillboard* billboard) {

			mBillboards.push_back(billboard);

		}

		/// <summary>
		/// Clears all particles. Internal use.
		/// </summary>
		void ClearParticles() {

			mParticles.resize(0);

		}

		/// <summary>
		/// Internal use.
		/// </summary>
		/// <param name="nb"></param>
		void AddParticle(NodeBillboard* nb)
		{
			mParticles.push_back(nb);
		}
		void RenderNodeNormals(NodeEntity* entity);

		void RenderNodeBasic(NodeEntity* entity);
		void RenderNodeLit(NodeEntity* entity);
		void RenderNodeActorLit(NodeActor* actor);
		void RenderNodeActorDepth(NodeActor* actor);
		void RenderNodeDepth(NodeEntity* entity);
		void RenderNodePositions(NodeEntity* entity);

		int LightCount();
		NodeLight* GetLight(int i);

		void CheckRT(Node3D* node);
		void UpdateRT();
		RefCntAutoPtr<ITopLevelAS> GetTLAS() {
			return mTLAS;
		}
		size_t RTInstanceCount() {
			return mRTMeshes.size();
		}
		std::vector<TexItem> GetRTTextureList() {
			return mRTTextureList;
		}
		Mesh3D* GetRTInstance(int id) {
			return mRTMeshes[id];
		}

		BigBuffer* GetRTBigBuffer() {

			BigBuffer* bb = new BigBuffer;

			for (int i = 0;i < mRTMeshes.size();i++) {

				bb->AddData(mRTMeshes[i]->GetVertices(), mRTMeshes[i]->GetTris(),mRTMeshes[i]->GetOwner());

			}

			bb->CreateBuffers();

			return bb;

			
		}


		Node3D* GetRoot() {
			return mRootNode;
		}
		void RenderNodeUI(Node3D* node);
		void RenderUI(float4x4 proj);

		void EndNode(Node3D* node);
		void BeginNode(Node3D* node);
		void BeginPlay();
		void EndPlay();

		void SetRoot(Node3D* node) {
			mRootNode = node;
		}
		void Reset() {
			mLights.resize(0);
			
		}

		void RemoveLight(Node3D* node)
		{
			std::vector<NodeLight*> new_list;
			for (int i = 0; i < mLights.size(); i++)
			{
				if (mLights[i] == node) {

				}
				else {
					new_list.push_back(mLights[i]);
				}
			}
			mLights = new_list;
		}

		void LoadGraph(std::string path)
		{
			VFile* file = new VFile(path.c_str(), FileMode::Read);
		
			//Node3D* root = new Node3D;
			mLights.resize(0);
			mCams.resize(0);
			mCam = nullptr;

			mRootNode = ReadNodeHeader(file);
			ReadNode(file,mRootNode);
			mProperties.resize(0);

			int pc = file->ReadInt();
			for (int i = 0; i < pc; i++) {

				
				PropertyType type = (PropertyType)file->ReadInt();
				std::string name(file->ReadString());
				
				NodeProperty* prop = new NodeProperty(name);
				prop->SetType(type);
				switch (type) {
				case PropertyType::Int:
					prop->SetInt(file->ReadInt());
					break;
				case PropertyType::Float:
					prop->SetFloat(file->ReadFloat());
					break;
				case PropertyType::Float2:
					
				{float x, y;
				x = file->ReadFloat();
				y = file->ReadFloat();
				prop->SetFloat2(float2(x,y));
				}
					break;
				case PropertyType::Float3:
					prop->SetFloat3(file->ReadVec3());
					break;
				case PropertyType::Bool:
					prop->SetBool(file->ReadBool());
					break;
				case PropertyType::String:
					prop->SetString(file->ReadString());
					break;
				}
				mProperties.push_back(prop);
			}

			file->Close();
		
		}

		void ReadNode(VFile* file, Node3D* node)
		{
			switch (node->GetType()) {
			case NodeType::Node:
			{
				node->ReadScripts(file);

				int cc = file->ReadInt();
				for (int i = 0; i < cc; i++) {

					auto sub = ReadNodeHeader(file);
					ReadNode(file, sub);
					node->AddNode(sub);

				}
			}
				break;
			case NodeType::Entity:
			{
				auto ent = (NodeEntity*)node;

				int mc = file->ReadInt();
				for (int i = 0; i < mc; i++) {

					Mesh3D* mesh = new Mesh3D();
					mesh->ReadMesh(file);
					ent->AddMesh(mesh);
					mesh->SetOwner(node);


				}

				node->ReadScripts(file);
				ent->SetPhysicsType((PhysicsType)file->ReadInt());

				int cc = file->ReadInt();
				for (int i = 0; i < cc; i++) {

					auto sub = ReadNodeHeader(file);
					ReadNode(file, sub);
					node->AddNode(sub);

				}

			}
				break;
			case NodeType::Light:
			{
				auto l = (NodeLight*)node;
				l->SetDiffuse(file->ReadVec3());
				l->SetSpecular(file->ReadVec3());
				l->SetRange(file->ReadFloat());
				l->SetCastShadows(file->ReadBool());

				l->ReadScripts(file);


				int cc = file->ReadInt();
				for (int i = 0; i < cc; i++) {

					auto sub = ReadNodeHeader(file);
					ReadNode(file, sub);
					node->AddNode(sub);


				}
			}
				break;
			case NodeType::Camera:
				break;
			}
		}

		void WriteTransform(VFile* file,Node3D* node) {

			file->WriteVec3(node->GetPosition());
			file->WriteVec3(node->GetScale());
			file->WriteMatrix(node->GetRotation4x4());

		}
		void ReadTransform(VFile* file,Node3D* node) {

			node->SetPosition(file->ReadVec3());
			node->SetScale(file->ReadVec3());
			node->SetRotation(file->ReadMatrix());


		}
		void SaveGraph(std::string path)
		{
			VFile* file = new VFile(path.c_str(), FileMode::Write);
			SaveNodeHeader(file,mRootNode);
			SaveNode(file,mRootNode);

			file->WriteInt((int)mProperties.size());
			for (int i = 0; i < mProperties.size(); i++) {

				auto prop = mProperties[i];
				file->WriteInt((int)prop->GetType());
				file->WriteString(prop->GetName().c_str());
				switch (prop->GetType()) {
				case PropertyType::Int:
					file->WriteInt(prop->GetInt());
					break;
				case PropertyType::Float:
					file->WriteFloat(prop->GetFloat());
					break;
				case PropertyType::Float2:
					file->WriteFloat(prop->GetFloat2().x);
					file->WriteFloat(prop->GetFloat2().y);
					break;
				case PropertyType::Float3:
					file->WriteVec3(prop->GetFloat3());
					break;
				case PropertyType::Bool:
					file->WriteBool(prop->GetBool());
					break;
				case PropertyType::String:
					file->WriteString(prop->GetString().c_str());
					break;
				}
			}
			//SaveProperties - > Global 
			file->Close();
		}
		Node3D* ReadNodeHeader(VFile* file) {

			Node3D* res = nullptr;
			NodeType type = (NodeType)file->ReadInt();
			switch (type) {
			case NodeType::Node:
				res = new Node3D;
				break;
			case NodeType::Entity:
				res = new NodeEntity;
				break;
			case NodeType::Light:
				res = new NodeLight;
				mLights.push_back((NodeLight*)res);
				break;
			case NodeType::Camera:
				res = new NodeCamera;
				mCams.push_back((NodeCamera*)res);
				if (mCam == nullptr) {
					mCam = (NodeCamera*)res;
				}
				//mCams.push_back((NodeCamera*)res);

				break;
			}

			res->SetName(file->ReadString());
			res->SetEnabled(file->ReadBool());
			ReadTransform(file, res);
			return res;
		}
		void SaveNodeHeader(VFile* file,Node3D* node) {

			file->WriteInt((int)node->GetType());
			file->WriteString(node->GetName());
			file->WriteBool(node->GetEnabled());
			WriteTransform(file,node);

		}

		void SaveNode(VFile* file, Node3D* node)
		{

			switch (node->GetType())
			{
			case NodeType::Node:
			{
				node->WriteScripts(file);

				file->WriteInt((int)node->ChildrenCount());
				for (int i = 0; i < node->ChildrenCount(); i++) {

					auto sub = node->GetChild(i);
					SaveNodeHeader(file, sub);
					SaveNode(file, sub);
					//mChildren[i]->WriteNode(file);

				}

			}
				break;
			case NodeType::Entity:
			{
				auto ent = (NodeEntity*)node;

				file->WriteInt((int)ent->GetMeshes().size());
				for (int i = 0; i < ent->GetMeshes().size(); i++) {

					ent->GetMesh(i)->WriteMesh(file);


				}

				ent->WriteScripts(file);
				file->WriteInt((int)ent->GetPhysicsType());

				file->WriteInt((int)node->ChildrenCount());
				for (int i = 0; i < node->ChildrenCount(); i++) {

					auto sub = node->GetChild(i);
					SaveNodeHeader(file, sub);
					SaveNode(file, sub);
					//mChildren[i]->WriteNode(file);

				}
			}
				break;
			case NodeType::Light:
			{
				auto l = (NodeLight*)node;

				file->WriteVec3(l->GetDiffuse());
				file->WriteVec3(l->GetSpecular());
				file->WriteFloat(l->GetRange());
				file->WriteBool(l->GetCastShadows());

				node->WriteScripts(file);

				file->WriteInt((int)node->ChildrenCount());
				for (int i = 0; i < node->ChildrenCount(); i++) {

					auto sub = node->GetChild(i);
					SaveNodeHeader(file, sub);
					SaveNode(file, sub);
					//mChildren[i]->WriteNode(file);

				}
			}
				break;
			}

	
		}

		static SceneGraph* mThis;
		RayPicker* mRayPick = nullptr;
		void AddProperty(NodeProperty* prop)
		{
			for (int i = 0; i < mProperties.size(); i++)
			{

				if (mProperties[i]->GetName() == prop->GetName())
				{
					mProperties.erase(mProperties.begin() + i);
					break;
				}

			}
			mProperties.push_back(prop);
		}
		NodeProperty* GetProperty(std::string name) {

			for (int i = 0; i < mProperties.size(); i++) {

				if (mProperties[i]->GetName() == name) {
					return mProperties[i];
				}

			}
			return nullptr;

		}

		std::vector<NodeCamera*> GetCams() {
			return mCams;
		}
		MeshRenderer* GetRenderer() {
			return mRenderer;
		}
		void SetCams(std::vector<NodeCamera*> cams)
		{
			mCams = cams;
		}
		void SetMain() {
			mMainScene = this;
		}
		static SceneGraph* GetMainScene() {
			return mMainScene;
		}
	private:
		static SceneGraph* mMainScene;

		std::vector<NodeCamera*> mCams;
		std::vector<Node3D*> mRTNodes;
		std::vector<Mesh3D*> mRTMeshes;
		//Kinetic::FX::Effect* FXDepth;

		std::vector<NodeLight*> mLights;

		std::vector<Node3D*> mNoRender;
		std::vector<TexItem> mRTTextureList;

		/// <summary>
		/// The root node of the graph, a root node can contain child nodes, creating the graph structurally.
		/// </summary>
		Node3D * mRootNode;
		std::vector<NodeBillboard*> mBillboards;

		std::vector<NodeBillboard*> mParticles;

		NodeCamera* mCam;

		MeshRenderer* mRenderer;
		CubeRenderer* mShadowRenderer;
		RefCntAutoPtr<ITopLevelAS> mTLAS;
		RefCntAutoPtr<IBuffer>             m_ScratchBuffer;
		RefCntAutoPtr<IBuffer>             m_InstanceBuffer;
		std::vector<NodeProperty*> mProperties;
		GameUI* mGameUI;
		SmartDraw* mDraw;
};
