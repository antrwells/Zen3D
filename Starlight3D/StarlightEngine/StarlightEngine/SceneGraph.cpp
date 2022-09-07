#include "pch.h"
#include "SceneGraph.h"


	SceneGraph::SceneGraph() {

		mRootNode = new Node3D();
		mCam = new NodeCamera;
		mRenderer = new MeshRenderer;
	//	FXDepth = new Kinetic::FX::Effect("engine/shader/depthVS.glsl", "engine/shader/depthFS.glsl");


	}
	
	void SceneGraph::Update() {

		mRootNode->Update();

		for (int i = 0; i < mBillboards.size(); i++) {

			mBillboards[i]->Update();

		}

		std::vector<NodeBillboard*> new_list;

		for (int i = 0; i < mBillboards.size(); i++) {

			if (mBillboards[i]->IsQueuedRemove()) {

			}
			else {
				new_list.push_back(mBillboards[i]);
			}

		}

		mBillboards = new_list;

		//***
		//Kinetic::Physics::Physics::Main->Simulate(1.0f);

		UpdateNodePhysics(mRootNode);

		CheckRemove(mRootNode);

	}

	void SceneGraph::CheckRemove(Node3D* node) {

		if (node->IsQueuedRemove()) {

			node->GetRootNode()->RemoveNode(node);

		}
		else {

			for (int i = 0; i < node->ChildrenCount(); i++) {
				CheckRemove(node->GetChild(i));

			}
		}

	}

	void SceneGraph::StartComponents() {

		mRootNode->StartComponents();

	}

	void SceneGraph::EndComponents() {

		mRootNode->EndComponents();

	}

	void SceneGraph::UpdateNodePhysics(Node3D * node) {

		node->UpdatePhysics();

		for (int i = 0; i < node->ChildrenCount(); i++) {

			auto child = node->GetChild(i);

			UpdateNodePhysics(child);

		}


	}

	void SetCam(int t, NodeCamera* cam) {
		//***
		/*
		switch (t) {
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			cam->LookAtZero(glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			cam->LookAtZero(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			cam->LookAtZero(glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			cam->LookAtZero(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			cam->LookAtZero(glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			cam->LookAtZero(glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		*/
	}
	//int NodeLight::ShadowMapRes = 1024;

	NodeCamera* scam = nullptr;


	void SceneGraph::DrawShadowMap(NodeLight* light) {

		
		if (!light->GetShadowMapping()) {
			return;
		}
		//Active = this;


		//	if (!Reshadow) return;
		//	Reshadow = false;
		NodeCamera* cam = GetCamera();
		

		
		if (scam == nullptr) {
			scam = new NodeCamera;

			scam->SetFOV(90);
			scam->SetMinZ(cam->GetMinZ());
			scam->SetMaxZ(cam->GetMaxZ());
			//***
			//scam->SetViewport(0, 0,ShadowMapRes, NodeLight::ShadowMapRes);
		}
		scam->SetPosition(light->GetPosition());


		//auto vp = scam->GetPosition();
		//vp.Z = -vp.Z;
		//scam->SetPosition(vp);

		/*
		glDisable(GL_BLEND);

		//glDisable(GL_SCISSOR_TEST);

		Kinetic::Graph::Nodes::NodeCamera* old = GetCamera();
		glClearColor((float)1, 1, 1, 1);

		int ShadowMapRes = Kinetic::Graph::Nodes::NodeLight::ShadowMapRes;

		SetCamera(scam);
		glViewport(0, 0, ShadowMapRes, ShadowMapRes);
		//scam->SetViewport(0, 0, ShadowMapRes, ShadowMapRes);

		Kinetic::Framebuffer::FramebufferCube* ShadowFB = light->GetShadowFB();
		Kinetic::FX::Global::EffectGlobal::CurrentCamera = mCam;
	//	FXDepth->bind();

	
		int tar = 0;//ShadowFB->SetFace(0);
		SetCam(tar, scam);

		RenderDepth();

		SetCam(ShadowFB->SetFace(1), scam);
		RenderDepth();

		SetCam(ShadowFB->SetFace(2), scam);
		RenderDepth();

		SetCam(ShadowFB->SetFace(3), scam);
		RenderDepth();

		SetCam(ShadowFB->SetFace(4), scam);
		RenderDepth();

		SetCam(ShadowFB->SetFace(5), scam);
		RenderDepth();

		SetCamera(old);

		ShadowFB->Release();
			//FXDepth->unbind();
		glViewport(0, 0, Kinetic::Info::AppInfo::aWidth, Kinetic::Info::AppInfo::aHeight);

		*/
	}


	/*
	void NodeLight::SetCam(int t, NodeCamera* cam) {

		switch (t) {
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			cam->LookAtZero(glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			cam->LookAtZero(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			cam->LookAtZero(glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			cam->LookAtZero(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			cam->LookAtZero(glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			cam->LookAtZero(glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	*/


	void SceneGraph::RenderShadowMaps() {

		int bt = clock();
		for (int i = 0; i < mLights.size(); i++) {

			DrawShadowMap(mLights[i]);

		}
		int et = clock();
	//	printf("DS:%d\n",(int)(et - bt));

	}



	void SceneGraph::RenderDepth() {


		int bt = clock();


		mRootNode->RenderDepth();

	
		int et = clock();

		//printf("RDTime:%d\n", (int)(et - bt));

	}

	void SceneGraph::PreRender() {

	}

	void SceneGraph::RenderNodeBasic(NodeEntity* entity)
	{

		int a = 5;
		mRenderer->RenderSimple(entity, mCam);

	}

	void SceneGraph::RenderNodeLit(NodeEntity* entity) {

		bool first = true;
		for (int i = 0;i < mLights.size();i++)
		{

			mRenderer->RenderLit(entity, mCam, mLights[i],first);
			first = false;
		}



	}

	void SceneGraph::Render() {
		Application* gApp = Application::GetApp();

		auto m_pImmediateContext = gApp->GetContext();
		if (RenderTarget2D::BoundTarget != nullptr) {

			const float ClearColor[] = { 0.350f, 0.350f, 0.350f, 1.0f };
			m_pImmediateContext->SetRenderTargets(1, &RenderTarget2D::BoundTarget->GetColorView(), RenderTarget2D::BoundTarget->GetDepthView(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			m_pImmediateContext->ClearRenderTarget(RenderTarget2D::BoundTarget->GetColorView(), ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			m_pImmediateContext->ClearDepthStencil(RenderTarget2D::BoundTarget->GetDepthView(), CLEAR_DEPTH_FLAG, 1.0f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}
		for (int i = 0;i < mRootNode->ChildrenCount();i++)
		{
			auto entity = (NodeEntity*)mRootNode->GetChild(i);
			
			RenderNodeLit((NodeEntity*)entity);
		}
		if (RenderTarget2D::BoundTarget != nullptr) {

			auto* pRTV = gApp->GetSwap()->GetCurrentBackBufferRTV();
			// Clear the default render target
			const float Zero[] = { 0.1f, 0.1f, 0.1f, 1.0f };
			m_pImmediateContext->SetRenderTargets(1, &pRTV, gApp->GetSwap()->GetDepthBufferDSV(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			m_pImmediateContext->ClearRenderTarget(pRTV, Zero, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		}
		/*
		Kinetic::FX::Global::EffectGlobal::CurrentCamera = mCam;
		
		bool first = true;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		Kinetic::FX::Global::EffectGlobal::CurrentLight = mLights[0];

		for (int i = 0; i < mBillboards.size(); i++) {
			Kinetic::FX::Global::EffectGlobal::CurrentNode = mBillboards[i];
			mBillboards[i]->Render();

		}



		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		std::vector<Kinetic::Graph::Nodes::NodeLight*> new_list;

		for (int i = 0; i < mLights.size(); i++) {
			if (mLights[i]->IsQueueRemoved()) {

			}
			else {

				new_list.push_back(mLights[i]);

			}

		}

		mLights = new_list;

		for (int i = 0; i < mLights.size(); i++) {

			Kinetic::FX::Global::EffectGlobal::CurrentLight = mLights[i];

			mRootNode->Render();

			if (first) {

//				glClear(GL_DEPTH_BUFFER_BIT);

				glBlendFunc(GL_ONE, GL_ONE);

				first = false;
			}
		}
		

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_GEQUAL);
		Kinetic::FX::Global::EffectGlobal::CurrentLight = mLights[0];
		
		for (int i = 0; i < mParticles.size(); i++) {

			Kinetic::FX::Global::EffectGlobal::CurrentNode = mParticles[i];
			mParticles[i]->Render();

		}

		

		*/
	}

	void SceneGraph::PostRender() {


	}

	void SceneGraph::AddNode(Node3D* node)
	{

		mRootNode->AddNode(node);

	}


	NodeCamera* SceneGraph::GetCamera() {

		return mCam;

	}

	void SceneGraph::AddLight(NodeLight* light) {

		mLights.push_back(light);

	}

