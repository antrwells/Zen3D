#include "pch.h"
#include "PPRenderer.h"
#include "SmartDraw.h"
#include "Application.h"

struct ColorLimitConstants {

    float4x4 g_WorldViewProj;
    float4 colorLimits;

};

struct BlurConstants {
    float4x4 g_WorldViewProj;
    float4 blur;

};

struct CombineConstants {

    float4x4 g_WorldViewProj;
    float4 combine;

};

PPRenderer::PPRenderer() {

	CreateColorLimitGP();
    CreateBlurGP();
    CreateCombineGP();
    mDraw = new SmartDraw(Application::GetApp());

}

void PPRenderer::CreateCombineGP() {

    auto app = Application::GetApp();

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = false;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_ONE;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_ZERO;

    //vertices = new Vertex2D[2048];
    //indices = new Uint32[4096];


    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "PPRenderer Combine PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = app->GetSwap()->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat = app->GetSwap()->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.BlendDesc = BlendState;

    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    //PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthFunc = COMPARISON_FUNC_GREATER;

    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // In this tutorial, we will load shaders from file. To be able to do that,
    // we need to create a shader source stream factory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    app->GetFactory()->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "PPRenderer - combine VS";
        ShaderCI.FilePath = "data/pp_combine.vsh";
        app->GetDevice()->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(CombineConstants);
        CBDesc.Usage = USAGE_DYNAMIC;
        CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        app->GetDevice()->CreateBuffer(CBDesc, nullptr, &m_VSConstants_Combine);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "PPRenderer - Combine PS";
        ShaderCI.FilePath = "data/pp_combine.psh";
        app->GetDevice()->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False},
        // Attribute 1 - vertex color
        LayoutElement{1, 0, 4, VT_FLOAT32, False},
        LayoutElement{2,0,2,VT_FLOAT32,False}
    };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    ShaderResourceVariableDesc Vars[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
    {SHADER_TYPE_PIXEL,"g_Texture2",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
    };


    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL,"g_Texture2",SamLinearClampDesc}
    
    };

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO_Combine);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_pPSO_Combine->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants_Combine);

    // Create a shader resource binding object and bind all static resources in it
    m_pPSO_Combine->CreateShaderResourceBinding(&m_pSRB_Combine, true);


}

void PPRenderer::CreateBlurGP() {

    auto app = Application::GetApp();

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = false;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_ONE;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_ZERO;

    //vertices = new Vertex2D[2048];
    //indices = new Uint32[4096];


    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "PPRenderer Blur PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = app->GetSwap()->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat = app->GetSwap()->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.BlendDesc = BlendState;

    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    //PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthFunc = COMPARISON_FUNC_GREATER;

    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // In this tutorial, we will load shaders from file. To be able to do that,
    // we need to create a shader source stream factory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    app->GetFactory()->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "PPRenderer - blur VS";
        ShaderCI.FilePath = "data/pp_blur.vsh";
        app->GetDevice()->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(BlurConstants);
        CBDesc.Usage = USAGE_DYNAMIC;
        CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        app->GetDevice()->CreateBuffer(CBDesc, nullptr, &m_VSConstants_Blur);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "PPRenderer - Blur PS";
        ShaderCI.FilePath = "data/pp_blur.psh";
        app->GetDevice()->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False},
        // Attribute 1 - vertex color
        LayoutElement{1, 0, 4, VT_FLOAT32, False},
        LayoutElement{2,0,2,VT_FLOAT32,False}
    };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    ShaderResourceVariableDesc Vars[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO_Blur);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_pPSO_Blur->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants_Blur);

    // Create a shader resource binding object and bind all static resources in it
    m_pPSO_Blur->CreateShaderResourceBinding(&m_pSRB_Blur, true);


}

void PPRenderer::CreateColorLimitGP() {


    auto app = Application::GetApp();

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = false;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_ONE;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_ZERO;

    //vertices = new Vertex2D[2048];
    //indices = new Uint32[4096];


    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "PPRenderer ColorLimit PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = app->GetSwap()->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat = app->GetSwap()->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_BACK;
    PSOCreateInfo.GraphicsPipeline.BlendDesc = BlendState;

    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    //PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthFunc = COMPARISON_FUNC_GREATER;

    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // In this tutorial, we will load shaders from file. To be able to do that,
    // we need to create a shader source stream factory
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    app->GetFactory()->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "PPRenderer - ColorLimit VS";
        ShaderCI.FilePath = "data/pp_colorLimit.vsh";
        app->GetDevice()->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(ColorLimitConstants);
        CBDesc.Usage = USAGE_DYNAMIC;
        CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        app->GetDevice()->CreateBuffer(CBDesc, nullptr, &m_VSConstants_ColorLimit);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "PPRenderer - ColorLimit PS";
        ShaderCI.FilePath = "data/pp_colorLimit.psh";
        app->GetDevice()->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False},
        // Attribute 1 - vertex color
        LayoutElement{1, 0, 4, VT_FLOAT32, False},
        LayoutElement{2,0,2,VT_FLOAT32,False}
    };
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    ShaderResourceVariableDesc Vars[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] =
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO_ColorLimit);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_pPSO_ColorLimit->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants_ColorLimit);

    // Create a shader resource binding object and bind all static resources in it
    m_pPSO_ColorLimit->CreateShaderResourceBinding(&m_pSRB_ColorLimit, true);


}

void PPRenderer::RenderCombine(Texture2D* texture,Texture2D* tex2, int w, int h, float i1, float i2)
{
    CombineConstants consts;

    //col_const.lightPos = float4(light->GetPosition(), 0);
    //col_const.lightDiff = float4(light->GetDiffuse(), 1);
   // co_const.lightProps = float4(light->GetRange(), 0, 0, 0);
    //dr_const.g_ViewProjInv = viewProjInv;
    float4x4 proj = float4x4::OrthoOffCenter(0, Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 0, 0, 100.0f, false);

    consts.g_WorldViewProj = proj.Transpose();
    consts.combine = float4(i1, i2, 0, 1.0);

    MapHelper<CombineConstants> CBConstants(Application::GetApp()->GetContext(), m_VSConstants_Combine, MAP_WRITE, MAP_FLAG_DISCARD);
    *CBConstants = consts;

    m_pSRB_Combine->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture2")->Set(tex2->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);


    mDraw->Begin();

    mDraw->DrawTexture(0, 0, w, h, texture, 1, 1, 1, 1, false);

    mDraw->End(m_pPSO_Combine, m_VSConstants_Combine, m_pSRB_Combine);

}

void PPRenderer::RenderColorLimit(Texture2D* texture, int w, int h,float limit)
{

    ColorLimitConstants consts;

    //col_const.lightPos = float4(light->GetPosition(), 0);
    //col_const.lightDiff = float4(light->GetDiffuse(), 1);
   // co_const.lightProps = float4(light->GetRange(), 0, 0, 0);
    //dr_const.g_ViewProjInv = viewProjInv;
    float4x4 proj = float4x4::OrthoOffCenter(0, Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 0, 0, 100.0f, false);

    consts.g_WorldViewProj = proj.Transpose();
    consts.colorLimits = float4(limit, limit, limit, 1.0);

    MapHelper<ColorLimitConstants> CBConstants(Application::GetApp()->GetContext(), m_VSConstants_ColorLimit, MAP_WRITE, MAP_FLAG_DISCARD);
    *CBConstants = consts;


    mDraw->Begin();

    mDraw->DrawTexture(0, 0, w, h, texture, 1, 1, 1, 1, false);

    mDraw->End(m_pPSO_ColorLimit, m_VSConstants_ColorLimit, m_pSRB_ColorLimit);



}


void PPRenderer::RenderBlur(Texture2D* texture, int w, int h, float blur)
{

    BlurConstants consts;

    //col_const.lightPos = float4(light->GetPosition(), 0);
    //col_const.lightDiff = float4(light->GetDiffuse(), 1);
   // co_const.lightProps = float4(light->GetRange(), 0, 0, 0);
    //dr_const.g_ViewProjInv = viewProjInv;
    float4x4 proj = float4x4::OrthoOffCenter(0, Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 0, 0, 100.0f, false);

    consts.g_WorldViewProj = proj.Transpose();
    consts.blur = float4(blur, blur, blur, blur);

    MapHelper<BlurConstants> CBConstants(Application::GetApp()->GetContext(), m_VSConstants_Blur, MAP_WRITE, MAP_FLAG_DISCARD);
    *CBConstants = consts;


    mDraw->Begin();

    mDraw->DrawTexture(0, 0, w, h, texture, 1, 1, 1, 1, false);

    mDraw->End(m_pPSO_Blur, m_VSConstants_Blur, m_pSRB_Blur);

}