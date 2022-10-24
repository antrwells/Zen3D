#include "pch.h"
#include "UIImage.h"
#include "UIRenderer.h"
#include "Texture2D.h"
#include "UITheme.h"

void UIImage::Render() {

	

	mUIRender->Draw(UITheme::Theme->mFrame, GetRenderRect(), UITheme::Theme->mFrameColor);

	

}