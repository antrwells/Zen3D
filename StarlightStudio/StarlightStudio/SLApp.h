#pragma once
#include "Application.h"
#include "SmartDraw.h"

class SLApp :
    public Application
{
public:
    void InitApp();
    void UpdateApp();
    void RenderApp();
private:

    SmartDraw* mDraw;
    Texture2D* mTex1;
    Texture2D* mTex2;
    Texture2D* mTex3;

};

