#pragma once
#include <vector>
//#include <glm/glm.hpp>
//#include <glm/common.hpp>
#include "NodeComponent.h"
//#include "Effect.h"


#include "NodeProperty.h"
#include "Common/interface/BasicMath.hpp"
#include "VFile.h"

class ScriptObject;
class ZClassNode;
class NodeEntity;
using namespace Diligent;

enum NodeType {
	Entity,Node,Camera,Light,Other,Actor
};

/// <summary>
/// Vivid::Graph::Nodes contains all the base-line graph nodes.
/// </summary>

	/// Node3D is a empty 3D graph node. For rendering capabilities see NodeEntity.
	/// </summary>
	class Node3D
	{
	public:

		/// <summary>
		/// The base constructor, will place the node at 0,0,0 with no rotation and a scale of 1,1,1
		/// </summary>
		Node3D();

		NodeType GetType() {

			return mType;

		}

		virtual void UpdateRotation() {};


		float3x3 GetRotation() {

			auto r1 = mRotation[0];
			auto r2 = mRotation[1];
			auto r3 = mRotation[2];

			float3 row1(r1[0], r1[1], r1[2]);
			float3 row2(r2[0], r2[1], r2[2]);
			float3 row3(r3[0], r3[1], r3[2]);


			return float3x3(row1,row2,row3);
			
		}

		float4x4 GetRotation4x4() {

			return mRotation;

		}

		/// <summary>
		/// Updates a node and all it's children. Custom implentations of nodes may rely on this to function correctly.
		/// </summary>
		virtual void Update();


		/// <summary>
		/// All children will be updated.
		/// </summary>
		void UpdateChildren();


		/// <summary>
		/// returns the world matrix, which is the position, rotation and scale of a node, and is modulated by it's root nodes.
		/// </summary>
		/// <returns></returns>
		virtual float4x4 GetWorldMatrix();

		/// <summary>
		/// Adds a node component to the node, this is for extending functionality of nodes for games/apps.
		/// </summary>
		/// <param name="component"></param>
		void AddComponent(NodeComponent* component);

		/// <summary>
		/// Internal use.
		/// </summary>
		void UpdateComponents();
		/// <summary>
		/// Internal use.
		/// </summary>
		void StartComponents();
		/// <summary>
		/// Internal use.
		/// </summary>
		void EndComponents();
		/// <summary>
		/// Internal use.
		/// </summary>
		void PauseComponents() {};
		/// <summary>
		/// Internal use.
		/// </summary>
		void ResumeComponents() {};
	
		/// <summary>
		/// Internal use.
		/// </summary>
		virtual void Render() {
			RenderChildren();
		};//overriden usually to render nodes onto the framebuffer.
		/// <summary>
		/// Internal use.
		/// </summary>
		virtual void RenderDepth() {
			RenderDepthChildren();
		}
		/// <summary>
		/// Internal use.
		/// </summary>
		void RenderChildren() {
			for (int i = 0; i < mChildren.size(); i++) {
				
				auto child = mChildren[i];
				if (child->GetEnabled() == false) continue;
				child->Render();

			}
		}
		/// <summary>
		/// Internal use.
		/// </summary>
		void RenderDepthChildren() {

			for (int i = 0; i < mChildren.size(); i++) {

				auto child = mChildren[i];
				if (child->GetEnabled() == false) continue;
				child->RenderDepth();

			}

		}

		/// <summary>
		/// Internal use.
		/// </summary>
		void SetRootNode(Node3D* node);

		/// <summary>
		/// Adds a node as a child of this node.
		/// </summary>
		/// <param name="node"></param>
		void AddNode(Node3D* node);
		void AddNodeTemp(Node3D* node) {

			mChildren.push_back(node);

		}
		/// <summary>
		/// Returns the 3D position of the node as a glm::vec3 
		/// </summary>
		/// <returns></returns>
		float3 GetPosition();

		float3 GetScale() {
			return mScale;
		}

		/// <summary>
		/// Sets the 3D position of the node.
		/// </summary>
		/// <param name="position"></param>
		virtual void SetPosition(float3 position);

		/// <summary>
		/// Sets the euler rotation of the node.
		/// </summary>
		/// <param name="pitch"></param>
		/// <param name="yaw"></param>
		/// <param name="roll"></param>
		virtual void SetRotation(float pitch, float yaw, float roll);
		void SetRotation4x4(float4x4 rotation);
		void SetRotation(float4x4 mat) {

			mRotation = mat;
			//mChanged = true;
			SetChanged();


			//InvalidateTransform();

		}

		virtual void RotateLocal(float pitch, float yaw, float roll);
		virtual void RotateGlobal(float pitch, float yaw, float roll);

		/// <summary>
		/// Sets the scale of the node. Default is 1,1,1
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(float3 scale);


		/// <summary>
		/// Moves the node with the given delta.
		/// </summary>
		/// <param name="move"></param>
		virtual void Move(float3 move);

		/// <summary>
		/// Points a node at the L vector.
		/// </summary>
		/// <param name="l"></param>
		/// <param name="up"></param>
		void LookAtZero(float3 l, float3 up)
		{
			//***


			InvalidateTransform();
		}

		

	//	static Kinetic::FX::Effect* FXDepth;
	//	static Kinetic::FX::Effect* FXDepthAnim;

		/// <summary>
		/// Returnns how many children this node has.
		/// </summary>
		/// <returns></returns>
		int ChildrenCount() {
			return mChildren.size();
		}
		/// <summary>
		/// Internal use.
		/// </summary>
		virtual void UpdatePhysics() {};

		/// <summary>
		/// Returns the given child node. starts at 0.
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		Node3D* GetChild(int id) {
			return mChildren[id];
		}

		/// <summary>
		/// Adds a custom property to the given node.
		/// </summary>
		/// <param name="property"></param>
		/// <returns></returns>
		NodeProperty* AddProperty(NodeProperty* property);
		
		/// <summary>
		/// Returns the given property.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		NodeProperty* GetProperty(std::string name);

		std::vector<NodeProperty*> GetProperties();


		/// <summary>
		/// Clones a node, including any data asscoiated with it.
		/// </summary>
		/// <returns></returns>
		virtual Node3D* Clone() { return nullptr; };
		
		/// <summary>
		/// If called, the node will be removed from the current scene graph at the earliest possible moment.
		/// </summary>
		void QueueRemove()
		{
			mQueueForRemove = true;
		}
		/// <summary>
		/// Returns true if the node is scheduled to be removed.
		/// </summary>
		/// <returns></returns>
		bool IsQueuedRemove() {
			return mQueueForRemove;
		}

		/// <summary>
		/// Returns the root(parent) node of the node.
		/// </summary>
		/// <returns></returns>
		Node3D* GetRootNode() {
			return mRootNode;
		}
		virtual void SetChanged() {
			for (int i = 0; i < mChildren.size(); i++) {
				mChildren[i]->SetChanged();
			}
		}
		float3 GetPositionWorld() {

			//return float4x4::Translation(0,0,0)* GetWorldMatrix();

			return float3(0, 0, 0) * GetWorldMatrix();
		}

		Node3D* FindNode(std::string name) {

			if (std::string(mName) == name) {
				return this;
			}
			for (int i = 0; i < mChildren.size(); i++) {
				
				auto res = mChildren[i]->FindNode(name);
				if (res != nullptr) {
					return res;
				}

			}

			return nullptr;
		}
		/// <summary>
		/// Removes a node from the children nodes.
		/// </summary>
		/// <param name="node"></param>
		void RemoveNode(Node3D* node) {

			std::vector<Node3D*> new_list;
			for (int i = 0; i < mChildren.size(); i++) {

				if (mChildren[i] == node) {

				}
				else {
					new_list.push_back(mChildren[i]);
				}

			}
			mChildren = new_list;

		}


		/// <summary>
		/// points at node at the given position.
		/// </summary>
		/// <param name="lookAt"></param>
		void LookAt(float3 lookAt)
		{


			//***
			//mRotation = glm::lookAt(mPosition,lookAt, glm::vec3(0, 1, 0));
			//mRotation[3] = glm::vec4(0, 0, 0, 1);
			//mRotation = glm::inverse(mRotation);
			InvalidateTransform();

		}

		/// <summary>
		/// Sets the name of the node.
		/// </summary>
		/// <param name="name"></param>
		void SetName(const char* name);
		/// <summary>
		/// returns the name of the node.
		/// </summary>
		/// <returns></returns>
		const char* GetName();
		void InvalidateTransform() {
			return;
			mTransformInvalidated = true;
			for (int i = 0; i < mChildren.size(); i++) {
				mChildren[i]->InvalidateTransform();
			}
		}

		void SetHidden(bool hidden) {
			mHidden = hidden;
		}

		bool IsHidden() {
			return mHidden;
		}

		ScriptObject* AddScript(std::string path,std::string name);

		std::vector<ScriptObject*> GetScripts() {
			return mScriptObjs;
		}
		void SetFilePath(std::string path)
		{
			mPath = path;
		}
		std::string GetFilePath() {
			return mPath;
		}
		void SetScripts(std::vector<ScriptObject*> objs)
		{
			mScriptObjs = objs;
		}
		void BeginNode();
		void EndNode();
		void AddSystemFunctions();
	
		virtual void ReadNode(VFile* file,bool read_type = true) {


			/*
			if (read_type) {
				mType = (NodeType)file->ReadInt();
			}
			mName = file->ReadString();
			ReadTransform(file);
			int cc = file->ReadInt();
			for (int i = 0; i < cc; i++) {

				NodeType mt = (NodeType)file->ReadInt();
				Node3D* new_node;
				if (mt == NodeType::Node) {
					new_node = new Node3D;
				}
				else if (mt == NodeType::Entity) {
					new_node = (Node3D*)new NodeEntity;
				}
				new_node->ReadNode(file,false);
				AddNode(new_node);

			}
			*/
		}

		void WriteScripts(VFile* file);
		void ReadScripts(VFile* file);
		void WriteClass(VFile* file,ZClassNode* cls);
		ZClassNode* ReadClass(VFile* file,ZClassNode* cls);
		virtual void WriteNode(VFile* file) {


			file->WriteInt((int)mType);
			file->WriteBool(mEnabled);
			file->WriteString(mName);
			//WriteTransform(file);

			file->WriteInt(0);

			WriteScripts(file);

			file->WriteInt(mChildren.size());
			for (int i = 0; i < mChildren.size(); i++) {

				mChildren[i]->WriteNode(file);

			}


		}

		bool Changed() {
			if (mChanged) {
				mChanged = false;
				return true;
			}
			return false;
		}

		void SetEnabled(bool enable) {
			mEnabled = enable;
		}

		bool GetEnabled() {
			return mEnabled;
		}

		void ClearNodes() {

			mChildren.resize(0);

		}

	protected:
		static bool mSysInit;
		bool mTransformInvalidated = true;

		bool mQueueForRemove = false;

		Node3D* mRootNode = nullptr;
		std::string mPath = "None";

		/// <summary>
		/// Children of this node.
		/// </summary>
		std::vector<Node3D *> mChildren;

		//Placement
		float4x4 mRotation;
		float3 mPosition;
		float3 mScale;

		float4x4 mValidTransform;

		//name
		const char* mName;

		NodeType mType;

		bool mHidden = false;

		//Components
		std::vector<NodeComponent*> mComponents;

		//Properties
		std::vector<NodeProperty*> mProperties;

		std::vector<std::string> mScripts;
		std::vector<ScriptObject*> mScriptObjs;

		float4x4 mPushRot;
		float3 mPushPos;
		float3 mPushScale;
		bool mChanged = false;
		bool mEnabled = true;
	};
