#include "pch.h"
#include "Texture2D.h"

Texture2D::Texture2D(const char* path) {



    auto m_pDevice = Application::GetDev();
    TextureLoadInfo loadInfo;
    loadInfo.IsSRGB = true;
    RefCntAutoPtr<ITexture> Tex;
    CreateTextureFromFile(path, loadInfo, m_pDevice, &Tex);
   //  Get shader resource view from the texture
   m_TextureSRV = Tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);


}