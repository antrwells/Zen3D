#include "pch.h"
#include "SceneRenderer.h"
#include "GBuffer.h"
#include "SceneGraph.h"
#include "Application.h"
#include "SmartDraw.h"

struct DRConstants {

    float4x4 g_WorldViewProj;
    float4 lightPos;
    float4 lightDiff;
    float4 lightSpec;
    float4 lightProps;
    float4x4 g_ViewProjInv;
    float4 camPos;
};;


SceneRenderer::SceneRenderer(SceneGraph* scene) {

	mWidth = Application::GetApp()->GetWidth();
	mHeight = Application::GetApp()->GetHeight();
	mGBuffer = new GBuffer(mWidth, mHeight);
	mGraph = scene;
    mDraw = new SmartDraw(Application::GetApp());
    CreateDRPSO();

}

void SceneRenderer::RenderData(SceneGraph* graph) {

	mGraph = graph;
	RenderData();

}

void SceneRenderer::RenderData() {

	mGBuffer->Render(mGraph);

}


//deferred
void SceneRenderer::CreateDRPSO() {

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = true;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_ONE;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_ONE;
    //BlendState.RenderTargets[0].BlendEnable



    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    Application* app = Application::GetApp();

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "SceneRenderer - DR GP PSO";

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
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthFunc = COMPARISON_FUNC_LESS_EQUAL;

    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    //ShaderCI.UseCombinedTextureSamplers = true;


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
        ShaderCI.Desc.Name = "Mesh Draw - VS";
        ShaderCI.FilePath = "data/draw_dr.vsh";
        app->GetDevice()->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(DRConstants);
        CBDesc.Usage = USAGE_DYNAMIC;
        CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        app->GetDevice()->CreateBuffer(CBDesc, nullptr, &m_DRConstants);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Mesh Draw - PS";
        ShaderCI.FilePath = "data/draw_dr.psh";
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
        {SHADER_TYPE_PIXEL, "g_Textures",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        {SHADER_TYPE_PIXEL,"g_Normals",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        {SHADER_TYPE_PIXEL,"g_Depth",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
        {SHADER_TYPE_PIXEL,"g_Positions",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
           {SHADER_TYPE_PIXEL,"g_Shadow",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}

     //   {SHADER_TYPE_PIXEL,"g_Shadow",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP
    };
    SamplerDesc EnvSam{

        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR,
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };

    ImmutableSamplerDesc ImtblSamplers[] =
    {
        {SHADER_TYPE_PIXEL, "g_Textures", SamLinearClampDesc},
        {SHADER_TYPE_PIXEL,"g_Normals",SamLinearClampDesc},
        {SHADER_TYPE_PIXEL,"g_Depth",SamLinearClampDesc},
        {SHADER_TYPE_PIXEL, "g_Positions",SamLinearClampDesc},
          {SHADER_TYPE_PIXEL,"g_Shadow",EnvSam}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO_DR);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_PSO_DR->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_DRConstants);
    //m_PSO_DR->GetStaticVariableByName(SHADER_TYPE_PIXEL, "Constants")->Set(m_DRConstants);


    // Create a shader resource binding object and bind all static resources in it
    m_PSO_DR->CreateShaderResourceBinding(&m_SRB_DR, true);

}

void SceneRenderer::RenderSceneDeferred() {

    RenderData();

    mDraw->Begin();                                                                                           

    mDraw->DrawTexture(0, 0,mWidth,mHeight, new Texture2D(mGBuffer->GetTextures()),1,1,1,1);

    auto tex_textures = new Texture2D(mGBuffer->GetTextures());
    auto tex_normals = new Texture2D(mGBuffer->GetNormals());
    auto tex_depth = new Texture2D(mGBuffer->GetDepth());
    auto tex_positions = new Texture2D(mGBuffer->GetPositions());
 
   m_SRB_DR->GetVariableByName(SHADER_TYPE_PIXEL, "g_Textures")->Set(tex_textures->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
    m_SRB_DR->GetVariableByName(SHADER_TYPE_PIXEL, "g_Normals")->Set(tex_normals->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
    //m_SRB_DR->GetVariableByName(SHADER_TYPE_PIXEL, "g_Textures")->Set(tex_textures->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
   // m_SRB_DR->GetVariableByName(SHADER_TYPE_PIXEL, "g_Depth")->Set(tex_depth->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
   m_SRB_DR->GetVariableByName(SHADER_TYPE_PIXEL, "g_Positions")->Set(tex_positions->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
   auto cont = Application::GetApp()->GetContext();
   float4x4 proj = float4x4::OrthoOffCenter(0, Application::GetApp()->GetWidth(), Application::GetApp()->GetHeight(), 0, 0, 100.0f, false);

   DRConstants dr_const;

   float4x4 viewProjInv = mGraph->GetCamera()->GetWorldMatrix().Inverse() * mGraph->GetCamera()->GetProjectionMatrix();

   dr_const.g_WorldViewProj = proj.Transpose();
   dr_const.g_ViewProjInv = viewProjInv;
   dr_const.camPos = float4(mGraph->GetCamera()->GetPosition(), 1.0);
   

   for (int i = 0;i < mGraph->LightCount();i++) {

       auto light = mGraph->GetLight(i);
       auto shadow = new TextureCube(light->GetShadowCube());


       m_SRB_DR->GetVariableByName(SHADER_TYPE_PIXEL, "g_Shadow")->Set(shadow->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);



       dr_const.lightPos = float4(light->GetPosition(), 0);
       dr_const.lightDiff = float4(light->GetDiffuse(), 1);
       dr_const.lightProps = float4(light->GetRange(), 0, 0, 0);
       dr_const.g_ViewProjInv = viewProjInv;
       MapHelper<DRConstants> CBConstants(cont, m_DRConstants, MAP_WRITE, MAP_FLAG_DISCARD);
       *CBConstants = dr_const;

       mDraw->End(m_PSO_DR, m_DRConstants, m_SRB_DR);
   //    return;
   }


    int b = 0;

}

