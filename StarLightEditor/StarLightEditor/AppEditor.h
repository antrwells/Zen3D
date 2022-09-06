#pragma once
#include "Application.h"
#include "SmartDraw.h"
#include "UI.h"
#include "kFont.h"
#include "Importer.h"
#include "SceneGraph.h"
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
    NodeEntity* mEnt1;
};

