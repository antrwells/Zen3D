#pragma once
#include "UIBase.h"
#include <string>

#include "WindowPage.h"

class UIWindowTitle :
    public UIBase
{
public:

    UIWindowTitle(std::string title);
    void Render();
    WindowPage AddPage(std::string title);
    void MouseDown(int but);
    void MouseUp(int but);
    void MouseMove(float2 pos,float2 delta);
    WindowPage GetPage(int id);

private:

    std::vector<WindowPage> mPages;
    WindowPage mCurrentPage;
    float2 mpos;
    bool mMove = false;


};

