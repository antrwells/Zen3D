#include "pch.h"
#include "SceneGraph.h"
#include "CubeRenderer.h"
#include "VString.h"
#define HIT_GROUP_STRIDE  2
	SceneGraph::SceneGraph() {

		mRootNode = new Node3D();
		mCam = new NodeCamera;
		mRenderer = new MeshRenderer;
		mShadowRenderer = new CubeRenderer(this, nullptr);
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
		Physics::Main->Simulate(1.0f);

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

		for (int i = 0;i < mLights.size();i++) {

			mShadowRenderer->SetRenderTargetCube(mLights[i]->GetShadowCube());

			mShadowRenderer->RenderDepth(mLights[i]->GetPosition(),mLights[i]->GetRange());

		}
	//	printf("DS:%d\n",(int)(et - bt));

	}



	void SceneGraph::RenderDepth() {

		const float ClearColor[] = { 1,1,1, 0.0f };


		if (RenderTargetCube::BoundTarget != nullptr || RenderTarget2D::BoundTarget != nullptr)
		{

		}
		else {
			auto con = Application::GetApp()->GetContext();
			auto* pRTV = Application::GetApp()->GetSwap()->GetCurrentBackBufferRTV();
			// Let the engine perform required state transitions
			con->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
			//con->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		}

		for (int i = 0;i < mRootNode->ChildrenCount();i++)
		{
			auto entity = (NodeEntity*)mRootNode->GetChild(i);

			if (entity->IsHidden()) continue;

			RenderNodeDepth((NodeEntity*)entity);
		}

		//printf("RDTime:%d\n", (int)(et - bt));

	}

	void SceneGraph::PreRender() {

	}

	void SceneGraph::RenderNodeBasic(NodeEntity* entity)
	{

		//int a = 5;
		//mRenderer->RenderSimple(entity, mCam);
		bool first = true;
		if (entity->GetMeshes().size() > 0) {


			mRenderer->RenderSimple(entity, mCam);
			first = false;

		}

		for (int i = 0;i < entity->ChildrenCount();i++) {

			RenderNodeBasic((NodeEntity*)entity->GetChild(i));

		}

	}

	void SceneGraph::RenderNodePositions(NodeEntity* entity) {


		bool first = true;
		if (entity->GetMeshes().size() > 0) {


			mRenderer->RenderPositions(entity, mCam);
			first = false;

		}

		for (int i = 0;i < entity->ChildrenCount();i++) {

			RenderNodePositions((NodeEntity*)entity->GetChild(i));

		}

	}

	void SceneGraph::RenderNodeDepth(NodeEntity* entity)
	{

		bool first = true;
		if (entity->GetMeshes().size() > 0) {
		

				mRenderer->RenderDepth(entity, mCam);
				first = false;
		
		}

		for (int i = 0;i < entity->ChildrenCount();i++) {

			RenderNodeDepth((NodeEntity*)entity->GetChild(i));

		}

	}

	void SceneGraph::RenderNodeLit(NodeEntity* entity) {

		bool first = true;
		if (entity->GetMeshes().size() > 0) {
			for (int i = 0;i < mLights.size();i++)
			{

				mRenderer->RenderLit(entity, mCam, mLights[i], first);
				first = false;
			}
		}

		for (int i = 0;i < entity->ChildrenCount();i++) {

			RenderNodeLit((NodeEntity*)entity->GetChild(i));

		}

	}

	void SceneGraph::RenderNodeNormals(NodeEntity* entity)
	{

		bool first = true;
		if (entity->GetMeshes().size() > 0) {


			mRenderer->RenderNormals(entity, mCam);
			first = false;

		}

		for (int i = 0;i < entity->ChildrenCount();i++) {

			RenderNodeNormals((NodeEntity*)entity->GetChild(i));

		}

	}

	void SceneGraph::RenderNormals() {

		for (int i = 0;i < mRootNode->ChildrenCount();i++)
		{
			auto entity = (NodeEntity*)mRootNode->GetChild(i);

			if (entity->IsHidden()) continue;

			RenderNodeNormals((NodeEntity*)entity);
		}

	}

	void SceneGraph::RenderTextures() {


		for (int i = 0;i < mRootNode->ChildrenCount();i++)
		{
			auto entity = (NodeEntity*)mRootNode->GetChild(i);

			if (entity->IsHidden()) continue;

			RenderNodeBasic((NodeEntity*)entity);
		}


	}

	void SceneGraph::RenderPositions() {

		for (int i = 0;i < mRootNode->ChildrenCount();i++)
		{
			auto entity = (NodeEntity*)mRootNode->GetChild(i);

			if (entity->IsHidden()) continue;

			RenderNodePositions((NodeEntity*)entity);
		}

	}

	void SceneGraph::Render() {
	
		for (int i = 0;i < mRootNode->ChildrenCount();i++)
		{
			auto entity = (NodeEntity*)mRootNode->GetChild(i);
			
			if (entity->IsHidden()) continue;

			RenderNodeLit((NodeEntity*)entity);
		}

		
	
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

	int SceneGraph::LightCount() {

		return mLights.size();

	}

	NodeLight* SceneGraph::GetLight(int i) {

		return mLights[i];

	}

	void SceneGraph::CheckRT(Node3D* node) {

		if (node->GetType() == NodeType::Entity)
		{
			//printf("Found Entity Node.\n");
			NodeEntity* entity = (NodeEntity*)node;
			
//			for (int i = 0;i < entity->GetMeshes().size();i++) {

				mRTNodes.push_back(entity);

				for (int i = 0;i < entity->GetMeshes().size();i++) {

					mRTMeshes.push_back(entity->GetMesh(i));

				}

			

		}

		for(int i = 0;i < node->ChildrenCount();i++) {

			CheckRT(node->GetChild(i));

		}

	}

	void SceneGraph::InitializeRT() {

		mRTNodes.resize(0);
		mRTMeshes.resize(0);
		CheckRT(mRootNode);
		printf("Initialized RT structures. Total Entities:%d Total Meshes:%d\n", (int)mRTNodes.size(), (int)mRTMeshes.size());
		TopLevelASDesc TLASDesc;
		TLASDesc.Name = "TLAS";
		TLASDesc.MaxInstanceCount = mRTMeshes.size();
		TLASDesc.Flags = RAYTRACING_BUILD_AS_ALLOW_UPDATE | RAYTRACING_BUILD_AS_PREFER_FAST_TRACE;

		Application::GetApp()->GetDevice()->CreateTLAS(TLASDesc, &mTLAS);


		// Create scratch buffer
		if (!m_ScratchBuffer)
		{
			BufferDesc BuffDesc;
			BuffDesc.Name = "TLAS Scratch Buffer";
			BuffDesc.Usage = USAGE_DEFAULT;
			BuffDesc.BindFlags = BIND_RAY_TRACING;
			BuffDesc.Size = std::max(mTLAS->GetScratchBufferSizes().Build, mTLAS->GetScratchBufferSizes().Update);

			Application::GetApp()->GetDevice()->CreateBuffer(BuffDesc, nullptr, &m_ScratchBuffer);
			VERIFY_EXPR(m_ScratchBuffer != nullptr);
		}

		//m_pRayTracingSRB->GetVariableByName(SHADER_TYPE_RAY_GEN, "g_TLAS")->Set(m_pTLAS);
		//m_pRayTracingSRB->GetVariableByName(SHADER_TYPE_RAY_CLOSEST_HIT, "g_TLAS")->Set(m_pTLAS);
		if (!m_InstanceBuffer)
		{
			BufferDesc BuffDesc;
			BuffDesc.Name = "TLAS Instance Buffer";
			BuffDesc.Usage = USAGE_DEFAULT;
			BuffDesc.BindFlags = BIND_RAY_TRACING;
			BuffDesc.Size = TLAS_INSTANCE_DATA_SIZE * mRTMeshes.size();

			Application::GetApp()->GetDevice()->CreateBuffer(BuffDesc, nullptr, &m_InstanceBuffer);
			VERIFY_EXPR(m_InstanceBuffer != nullptr);
		}


		std::vector<TLASBuildInstanceData> Instances;

		for (int i = 0;i < mRTMeshes.size();i++) {

			TLASBuildInstanceData inst;

			VString name = VString("Instance ");

			name.Add(VString(i));


			inst.InstanceName = name.GetConst();
			inst.CustomId = i;
			inst.pBLAS = mRTMeshes[i]->GetBlas();
			inst.Mask = 1;
	

			float3 pos = mRTMeshes[i]->GetOwner()->GetPosition();
			float3x3 rot = mRTMeshes[i]->GetOwner()->GetRotation();

			TexItem tex_item;

			tex_item.color = mRTMeshes[i]->GetMaterial()->GetColorMap();
			tex_item.normal = mRTMeshes[i]->GetMaterial()->GetNormalMap();
			tex_item.specular = mRTMeshes[i]->GetMaterial()->GetSpecularMap();

			mRTTextureList.push_back(tex_item);

			inst.Transform.SetTranslation(pos.x, pos.y, pos.z);
			inst.Transform.SetRotation(rot.Data());

			Instances.push_back(inst);

		}

		BuildTLASAttribs Attribs;
		Attribs.pTLAS = mTLAS;
		Attribs.Update = false;
		Attribs.pScratchBuffer = m_ScratchBuffer;

		Attribs.pInstanceBuffer = m_InstanceBuffer;

		Attribs.pInstances = Instances.data();
		Attribs.InstanceCount = Instances.size();

		Attribs.BindingMode = HIT_GROUP_BINDING_MODE_PER_INSTANCE;
		Attribs.HitGroupStride = HIT_GROUP_STRIDE;

		Attribs.TLASTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
		Attribs.BLASTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
		Attribs.InstanceBufferTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
		Attribs.ScratchBufferTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

		Application::GetApp()->GetContext()->BuildTLAS(Attribs);

		int done = 1;

		//TLASBuildInstanceData Instances[mRTNodes.size()] = {};



		int aa = 5;
		

	}