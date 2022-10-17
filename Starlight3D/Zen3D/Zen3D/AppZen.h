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
#include "PostProcessing.h"
#include "ZenUI.h"

class AppZen : public Application
{
public:

    AppZen();
    void InitApp();
    void UpdateApp();
    void RenderApp();
    void SetPayload();

private:
    ZenUI* zUI;
    RenderTarget2D* t1;
    SmartDraw* mDraw;
    Texture2D* mTex1;
    Texture2D* mTex2;
    Texture2D* mTex3;
    UI* mUI;
    //TTFont* mFont1;
    kFont* mFont1;
    SceneGraph* mGraph;
    NodeEntity* mEnt1, * mEnt2;
    NodeActor* mAct1;
    NodeLight* mLight1, * mLight2;
    RenderTarget2D* mRT1;
    RenderTargetCube* mRTC1;
    CubeRenderer* mRC;
    GBuffer* mGB1;
    SceneRenderer* mRenderer;
    SceneRayTracer* mRTRenderer;
    PostProcessing* mPP;
};

