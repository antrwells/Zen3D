#pragma once
#include "Application.h"
#include "UI.h"
#include "UIBase.h"
class UITest :
    public Application
{
public:

    void InitApp();
    void UpdateApp();
    void RenderApp();

private:

    UI* mUI;

};

