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

		/// <summary>
		/// Will update the scene graph by calling each node's update method.
		/// </summary>
		void Update();

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

		/// <summary>
		/// Adds a light to the scene. You can use as many as you'd like.
		/// </summary>
		/// <param name="light"></param>
		void AddLight(NodeLight* light);

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

		void RenderNodeBasic(NodeEntity* entity);
		void RenderNodeLit(NodeEntity* entity);
	

	private:
	

		//Kinetic::FX::Effect* FXDepth;

		std::vector<NodeLight*> mLights;

		std::vector<Node3D*> mNoRender;

		/// <summary>
		/// The root node of the graph, a root node can contain child nodes, creating the graph structurally.
		/// </summary>
		Node3D * mRootNode;
		std::vector<NodeBillboard*> mBillboards;

		std::vector<NodeBillboard*> mParticles;

		NodeCamera* mCam;

		MeshRenderer* mRenderer;

	};
