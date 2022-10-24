#include "pch.h"
#include "UIRenderer.h"
#include "SmartDraw.h"
#include "Application.h"
#include "kFont.h"

UIRenderer::UIRenderer() {

	mDraw = new SmartDraw(Application::GetApp());

}

void UIRenderer::Begin() {

	mDraw->Begin();

}

void UIRenderer::Draw(Texture2D* texture, UIRect rect, UIColor color)
{

	mDraw->DrawTexture(rect.x, rect.y, rect.w, rect.h, texture, color.r, color.g, color.b, color.a);

}

void UIRenderer::Text(std::string text,kFont* font, int x, int y, UIColor color) {

	font->drawText(text.c_str(), x, y, color.r, color.g, color.b, color.a,mDraw);

}

void UIRenderer::End() {

	mDraw->End();

}