#include "AppZen.h"
#include "UITheme_Neon.h"
#include "ButtonControl.h"
#include "Maths.h"
#include "ActorAnim.h"
#include "PostProcessing.h"
#include "PPBloom.h"
#include "ScriptHost.h"
#include "ScriptObject.h"

AppZen::AppZen() {



}


void AppZen::InitApp() {

	mGraph = new SceneGraph;

	Importer* imp = new Importer;

	//Application::GetApp()->TextToScript("puts(\"Hey everyone!\")");
//	Application::GetApp()->FileToScript("script/application_init.ch");


	
	int a = 5;
//	NodeEntity* n1 = imp->ImportAI("data/3d/map1.fbx", true);
//	NodeEntity* act1 = (NodeActor*)imp->ImportAI("data/test/b1.fbx");;//->GetChild(0



	//auto real_node = (NodeEntity*)n1;//n1->GetChild(0);
	//real_node->SetPhysicsTris();

	//act1->SetPosition(float3(4, 3, 0));
	

	//int a = 5;

	//auto norm1 = new Texture2D("data/3d/norm3.png");
	//auto norm2 = new Texture2D("data/3d/norm3.png");
	//auto norm3 = new Texture2D("data/3d/norm3.png");

	//mGraph->AddNode(real_node);
	//mGraph->AddNode(act1);

	//real_node->GetMesh(0)->GetMaterial()->SetNormalMap(norm1);
	//real_node->GetMesh(1)->GetMaterial()->SetNormalMap(norm2);

	//ActorAnim* walk = new ActorAnim("Walk", 0, 78, 0.4f, AnimType::Forward);

	/*
	act1->AddAnim(walk);
	act1->PlayAnim("Walk");
	//act1->SetRotation(0, 180, 0);
	mAct1 = act1;
	*/

	//mEnt1 = real_node;
	//mEnt2 = real2;

	mLight1 = new NodeLight(false);
	//mLight2 = new NodeLight(false);

	mGraph->AddLight(mLight1);




	//mGraph->AddLight(mLight2);
//	mLight2->SetRange(30);
	mLight1->SetRange(30);
	mLight1->SetDiffuse(float3(1, 1, 1));
	mLight1->SetPosition(float3(0, 10, 0));
	//mLight2->SetPosition(float3(2, 8, 0));
	//mLight2->SetDiffuse(float3(2, 1, 1));

	auto cam = mGraph->GetCamera();
	cam->SetPosition(float3(0, 5, -10));

	zUI = new ZenUI(mGraph);



}

void AppZen::UpdateApp() {

	
}
bool fu = true;
float anX = 0;
float anY = 0;
float cX = 0, cY = 0;
float lx = 0, lz = 0;
float la = 0;
int dx = 0;
bool mUseRT = false;
bool mUsePP = false;
void AppZen::RenderApp() {



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
	
	if (Application::GetInput()->IsKeyDown(KeyID::Space))
	{
		float3 cp = cam->GetPosition();

		mLight1->SetPosition(cp);
	}

	float spd = -0.15f;

	if (Application::GetInput()->IsKeyDown(KeyID::KeyW))
	{

		cam->Move(float3(0, 0, -spd));

	}
	if (Application::GetInput()->IsKeyDown(KeyID::KeyS)) {
		cam->Move(float3(0, 0, spd));
	}
	if (Application::GetInput()->IsKeyDown(KeyID::KeyA))
	{
		cam->Move(float3(spd, 0, 0));
	}
	if (Application::GetInput()->IsKeyDown(KeyID::KeyD))
	{
		cam->Move(float3(-spd, 0, 0));
	}

	zUI->RenderUI();

	//zUI->RenderUI();
	
}