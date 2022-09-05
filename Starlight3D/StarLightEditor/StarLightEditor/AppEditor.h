#pragma once
#include "Application.h"
#include "SmartDraw.h"
class AppEditor : public Application
{
public:

	AppEditor();
    void InitApp();
    void RenderApp();

private:

    SmartDraw* mDraw;
    Texture2D* mTex1;
    Texture2D* mTex2;
    Texture2D* mTex3;


};

