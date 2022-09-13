#include "AppEditor.h"
#include "UITheme_Neon.h"
#include "ButtonControl.h"
#include "Maths.h"

AppEditor::AppEditor() {



}


void AppEditor::InitApp() {
	mDraw = new SmartDraw(this);
	mTex1 = new Texture2D("data/test1.jpg");
	mTex2 = new Texture2D("data/test2.jpg");
	mTex3 = new Texture2D("data/test3.jpg");
	mUI = new UI(0, 0);
	mUI->SetInput(GetInput());
	UI::SetTheme(new UITheme_Neon);

	ButtonControl* but1 = new ButtonControl;
	but1->Set(20, 20, 200, 100);
	but1->SetText("Testing!");
	mUI->GetRoot()->AddControl(but1);

	Importer* imp = new Importer;

	NodeEntity* n1 = imp->ImportAI("data/3d/map1.fbx", true);
	NodeEntity* sphere = imp->ImportAI("data/test2.fbx", true);

	int b = 5;

	mGraph = new SceneGraph();

	auto real_node = (NodeEntity*)n1;//n1->GetChild(0);
	real_node->SetPhysicsTris();

	auto real2 = (NodeEntity*)sphere;

 	int a = 5;

	auto norm1 = new Texture2D("data/3d/norm3.png");
	auto norm2 = new Texture2D("data/3d/norm3.png");
	auto norm3 = new Texture2D("data/3d/norm3.png");

	mGraph->AddNode(real_node);
	mGraph->AddNode(real2);

	real_node->GetMesh(0)->GetMaterial()->SetNormalMap(norm1);
	real_node->GetMesh(1)->GetMaterial()->SetNormalMap(norm2);

	

	real2->SetPosition(float3(4, 5.5f, 0));


	//real2->SetPhysicsConvex();
	real2->SetPosition(float3(0, 2.5f, 0));
//	real2->GetBody()->ApplyForce(0, 0, 0);

	mEnt1 = real_node;
	mEnt2 = real2;

	mLight1 = new NodeLight(false);
	mLight2 = new NodeLight(false);

	mGraph->AddLight(mLight1);



	//mGraph->AddLight(mLight2);
	mLight2->SetRange(30);
	mLight1->SetRange(30);
	mLight1->SetDiffuse(float3(1, 1, 1));
	mLight1->SetPosition(float3(0, 10, 0));
	mLight2->SetPosition(float3(2, 8, 0));
	mLight2->SetDiffuse(float3(2, 1, 1));

	float ax, ay;

	srand(clock());

	ax = -40;
	ay = -40;



	auto tex = new Texture2D("data/testNorm.jpg");
	//real_node->GetMesh(0)->GetMaterial()->SetNormalMap(tex);
//	real_node->GetMesh(1)->GetMaterial()->SetNormalMap(tex);
	//real2->GetMesh(0)->GetMaterial()->SetNormalMap(tex);e
	//mFont1 = new TTFont("data/f1.ttf");
	//mFont1 = new kFont("data/fonts/air.pf");
	auto cam = mGraph->GetCamera();
	cam->SetPosition(float3(0, 5, -10));
	//mRT1 = new RenderTarget2D(Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight());
	mRTC1 = new RenderTargetCube(1024, 1024);
	mRC = new CubeRenderer(mGraph, mRTC1);
	mGB1 = new GBuffer(Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight());
	mRenderer = new SceneRenderer(mGraph);

	mGraph->InitializeRT();
	int c = 5;
	mRTRenderer = new SceneRayTracer(mGraph);
}

void AppEditor::UpdateApp() {

	mUI->Update();
	mGraph->Update();

}



float anX = 0;
float anY = 0;
float cX=0, cY=0;
float lx = 0, lz = 0;
float la = 0;
int dx = 0;
bool mUseRT = false;
void AppEditor::RenderApp() {

	std::cout << "Rendering App.\n";

	anY++;
	anX++;

	//control

	auto cam = mGraph->GetCamera();
	//cam->SetPosition(float3(0, -5, 10));
   // mEnt1->SetRotation(anX, anY, 0);
	la = la + 0.7f;
	lx = cos(Maths::Deg2Rad(la)) * 8;
	lz = sin(Maths::Deg2Rad(la)) * 8;
	int dx, dy;
	// mEnt1->SetRotation(0, la, 0);
	dx = Application::GetInput()->GetMouseDX();
	dy = Application::GetInput()->GetMouseDY();
	// mLight1->SetPosition(float3(lx, 8, lz));
	
	// mUI->Render();
	if (Application::GetInput()->IsKeyDown(KeyID::Shift))
	{
		cX -= dy;
		cY -= dx;
		cam->SetRotation(cX, cY, 0);
	}
	if (Application::GetInput()->IsKeyDown(KeyID::Space))
	{
		float3 cp = cam->GetPosition();

		mLight1->SetPosition(cp);
	}

	float spd = -0.05f;

	if (Application::GetInput()->IsKeyDown(KeyID::W))
	{

		cam->Move(float3(0, 0, -spd));

	}
	if (Application::GetInput()->IsKeyDown(KeyID::S)) {
		cam->Move(float3(0, 0, spd));
	}
	if (Application::GetInput()->IsKeyDown(KeyID::A))
	{
		cam->Move(float3(spd, 0, 0));
	}
	if (Application::GetInput()->IsKeyDown(KeyID::D))
	{
		cam->Move(float3(-spd, 0, 0));
	}



	if (!mUseRT)
	{
	
	mGraph->RenderShadowMaps();
	mGraph->Render();
}
	else {
		mRTRenderer->Render();
		//		 int a = 5;
	}


	ImGui::Begin("Starlight3D - Test 001");


	ImGui::Text("Below are some settings");

	ImGui::Checkbox("RayTracing?", &mUseRT);

	ImGui::End();


		 //mGraph->RenderShadowMaps();
		 //mRenderer->RenderSceneDeferred();


		 //mGB1->Render(mGraph);

		 //mGB1->Debug(3);



		 //mGraph->RenderShadowMaps();


		 //mGraph->RenderTextures();


		// cam->SetMaxZ(mLight1->GetRange());
		 

		 //mGraph->RenderDepth();

		


		 //mGraph->RenderDepth();


		// mRC->Render(float3(0, 2.8, 0));
		 //mUI->Render();
		 //mRTC1->Bind(0);
		// mGraph->Render();
		// mRTC1->Release(0);
		 
		 //return

		 return;
		 
		 mDraw->Begin();
		 mDraw->DrawTexture(20, 20, 512, 512,new Texture2D(mRTC1,0), 1, 1, 1, 1);
		 mDraw->End();
		
		


		 // mFont1->drawText("aaabbbcccddddeeefffggghhhiiijjjklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 20, 300, 1, 1, 1, 1,mDraw);
		 // mFont1->drawText("This is another test to see if text rendering is working 0123456789", 20, 400, 1, 1, 1, 1, mDraw);
		  /*
	mDraw->Begin();

	mFont

	


	mDraw->DrawTexture(dx+20, dx+20, 290, 200,mTex1, 1, 0, 0, 1);
	mDraw->DrawTexture(dx+200, 200, 255, 255,mTex2, 0, 1, 0, 1);
	mDraw->DrawTexture(dx+100, 600, 96, 96,mTex3, 0, 0, 1,1);


	dx = dx + 1;
	if (dx > 1024) {
		dx = 0;
	}

	mDraw->End();
	*/
	dx = dx + 1;

}