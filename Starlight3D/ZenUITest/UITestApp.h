#pragma once
#include "Application.h"
#include "SmartDraw.h"
class UITestApp :
    public Application
{
public:

    void InitApp();
    void UpdateApp();
    void RenderApp();
    void SetPayload() {};


private:

    SmartDraw* mDraw;
    Texture2D* tex1;
};

