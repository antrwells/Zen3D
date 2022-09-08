#include "pch.h"
#include "TextureCube.h"

TextureCube::TextureCube(RenderTargetCube* rt)
{
	pRTColor = rt->GetColorTexture();
	m_pColorRTV = rt->GetColorView();

}