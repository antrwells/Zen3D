#include "pch.h"
#include "UIScrollerVert.h"
#include "UIRenderer.h"
#include "Texture2D.h"
#include "UITheme.h"

UIScrollerVert::UIScrollerVert() {

    mCanScroll = false;
    mIsContent = false;

}

void UIScrollerVert::Render() {

	UIColor final_col = UITheme::Theme->mButonColor;

	final_col.Mix(mColor);

    GetValue();
   // printf("Value:%f\n", GetValue());
  //  printf("Y:%d\n", GetY());

    UIRect bar = UIRect(GetRenderRect().x,GetRenderRect().y+mCurrent,GetRect().w,dh);

    //DrawDragger(RenderPosition.X + 1, RenderPosition.Y + CurrentValue, Size.X - 1, (int)dh, new OpenTK.Mathematics.Vector4(1.5f, 1.5f, 1.5f, 1));

	mUIRender->Draw(UITheme::Theme->mWhite, GetRenderRect(), UIColor(0.8, 0.8,0.8, 0.85));
	mUIRender->Draw(UITheme::Theme->mScrollBar, bar, final_col);

}

void UIScrollerVert::SetCurrent(int v) {

	mCurrent = v;

}

void UIScrollerVert::SetMax(int v) {

	mMax = v;

}

float UIScrollerVert::GetValue() {

	float yi, hd, av, ov;
	float nm = 0;
	float ay = 0;
	float max_V = 0;
	yi = hd = av = ov = av2 = 0.0f;
	dh = 0;
    ov = (float)(GetRect().h - mMax) / (float)(mMax);

    if (ov > 0.9)
    {

        ov = 0.9f;
    }
    if (ov < 0.1)
    {
        ov = 0.1f;
    }





    dh = (int)(GetRect().h * ov);






    if (mCurrent + dh > GetRect().h)
    {
        //if (dh != float.PositiveInfinity)
        ///{
            mCurrent =GetRect().h - (int)dh;
            if (mCurrent < 0) mCurrent = 0;
        //}
    }



    max_V = GetRect().h - (dh);


    av2 = mCurrent / max_V;

    return av2;

}

int msx, msy;

void UIScrollerVert::MouseDown(int but) {
    if (msy > GetRenderRect().y + mCurrent && msy < GetRenderRect().y + mCurrent + dh)
    {
        drag = true;
    }
    else
    {
        drag = false;
    }
}

void UIScrollerVert::MouseUp(int but) {
    drag = false;
}

void UIScrollerVert::MouseMove(float2 pos, float2 delta) {

    msx = pos.x;
    msy = pos.y;
    if (drag) {
        mCurrent += delta.y;
        if (mCurrent < 0) mCurrent = 0;
        if (mCurrent > GetRect().h) mCurrent = GetRect().h;
    }

}