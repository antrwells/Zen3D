#pragma once 
#include "RenderTargetCube.h"
class TextureCube
{
public:

	TextureCube(RenderTargetCube* rt);

	RefCntAutoPtr<ITextureView> GetView() { return m_pColorRTV; };

private:

	RefCntAutoPtr<ITexture> pRTColor;
	//RefCntAutoPtr<ITexture> pRTDepth;

	RefCntAutoPtr<ITextureView> m_pColorRTV;

};
