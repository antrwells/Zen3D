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

Texture2D::Texture2D(Application* app,int w, int h, bool alpha,const char* buf) {

    TextureDesc TexDesc;
    TexDesc.Type = RESOURCE_DIM_TEX_2D;
    TexDesc.Format = TEX_FORMAT_RGBA8_UNORM_SRGB;
    TexDesc.Width = w;
    TexDesc.Height = h;
    TexDesc.MipLevels = 1;
    TexDesc.BindFlags = BIND_SHADER_RESOURCE;
    TexDesc.Usage = USAGE_DEFAULT;

    mWidth = w;
    mHeight = h;

    TextureSubResData data;
    data.pData = buf;
    data.Stride = w * 4;
  
    TextureData InitData;
    // Pointer to subresouce data, one for every mip level
    InitData.pSubResources = &data;
    InitData.NumSubresources = 1;

    app->GetDevice()->CreateTexture(TexDesc, &InitData, &Texture);
    m_TextureSRV = Texture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

}