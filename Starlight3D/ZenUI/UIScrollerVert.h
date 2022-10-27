#pragma once
#include "UIBase.h"
class UIScrollerVert :
    public UIBase
{
public:

    UIScrollerVert();
    void Render();
    void SetCurrent(int val);
    void SetMax(int max);
    float GetValue();
    void MouseDown(int but);
    void MouseUp(int but);
    void MouseMove(float2 pos, float2 delta);
    int GetY() {

        return (int)((float)(mMax)*GetValue());

    }
private:

    int mCurrent = 0;
    int mMax = 0;
    float av2;
    int dh;
    bool drag = false;

};

