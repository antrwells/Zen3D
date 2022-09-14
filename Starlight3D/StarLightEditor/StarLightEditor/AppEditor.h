#pragma once
#include "Application.h"
#include "SmartDraw.h"
#include "UI.h"
#include "kFont.h"
#include "Importer.h"
#include "SceneGraph.h"
#include "RenderTarget2D.h"
#include "RenderTargetCube.h"
#include "CubeRenderer.h"
#include "GBuffer.h"
#include "SceneRenderer.h"
#include "SceneRayTracer.h"

class AppEditor : public Application
{
public:

	AppEditor();
    void InitApp();
    void UpdateApp();
    void RenderApp();

private:

    SmartDraw* mDraw;
    Texture2D* mTex1;
    Texture2D* mTex2;
    Texture2D* mTex3;
    UI* mUI;
    //TTFont* mFont1;
    kFont* mFont1;
    SceneGraph* mGraph;
    NodeEntity* mEnt1,* mEnt2;
    NodeActor* mAct1;
    NodeLight* mLight1, * mLight2;
    RenderTarget2D* mRT1;
    RenderTargetCube* mRTC1;
    CubeRenderer* mRC;
    GBuffer* mGB1;
    SceneRenderer* mRenderer;
    SceneRayTracer* mRTRenderer;

};

