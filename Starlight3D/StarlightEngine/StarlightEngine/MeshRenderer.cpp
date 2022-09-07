#include "pch.h"
#include "MeshRenderer.h"
#include "Application.h"
#include "Maths.h"
#include "RenderTarget2D.h"


struct LitConstants {

    float4x4 g_MVP;
    float4x4 g_Proj;
    float4x4 g_Model;
    float4x4 g_View;
    float4x4 g_ModelInv;
    float4 viewPos;
    float4 lightPos;
    float4 lightProp;
    float4 lightDiff;
    float4 lightSpec;


};

MeshRenderer::MeshRenderer() {

	CreateSimpleGP();
    CreateLitGP();

}

void MeshRenderer::CreateSimpleGP() {

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = true;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_SRC_ALPHA;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_INV_SRC_ALPHA;

    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    Application* app = Application::GetApp();

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Mesh3D - SimpleGP PSO";

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
        ShaderCI.Desc.Name = "Mesh Simple - VS";
        ShaderCI.FilePath = "data/mesh_simple.vsh";
        app->GetDevice()->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(float4x4);
        CBDesc.Usage = USAGE_DYNAMIC;
        CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        app->GetDevice()->CreateBuffer(CBDesc, nullptr, &m_VSConstants);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Mesh Simple - PS";
        ShaderCI.FilePath = "data/mesh_simple.psh";
        app->GetDevice()->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False}, //pos
        // Attribute 1 - vertex color
        LayoutElement{1, 0, 4, VT_FLOAT32, False}, //col
        LayoutElement{2,0,3,VT_FLOAT32,False}, //tex-coord
        LayoutElement{3,0,3,VT_FLOAT32,False }, //norm
        LayoutElement{4,0,3,VT_FLOAT32,False}, //bi-norm
        LayoutElement{5,0,3,VT_FLOAT32,False} //tangent
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

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO_Basic);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_PSO_Basic->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);

    // Create a shader resource binding object and bind all static resources in it
    m_PSO_Basic->CreateShaderResourceBinding(&m_SRB_Basic, true);

}


void MeshRenderer::CreateLitGP() {

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = true;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_ONE;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_ZERO;
    //BlendState.RenderTargets[0].BlendEnable



    GraphicsPipelineStateCreateInfo PSOCreateInfo;
    Application* app = Application::GetApp();

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Mesh3D - LitGP PSO";

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
        ShaderCI.Desc.Name = "Mesh Lit - VS";
        ShaderCI.FilePath = "data/mesh_lit.vsh";
        app->GetDevice()->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        BufferDesc CBDesc;
        CBDesc.Name = "VS constants CB";
        CBDesc.Size = sizeof(LitConstants);
        CBDesc.Usage = USAGE_DYNAMIC;
        CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
        CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        app->GetDevice()->CreateBuffer(CBDesc, nullptr, &m_LitConstants);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Mesh Lit - PS";
        ShaderCI.FilePath = "data/mesh_lit.psh";
        app->GetDevice()->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False}, //pos
        // Attribute 1 - vertex color
        LayoutElement{1, 0, 4, VT_FLOAT32, False}, //col
        LayoutElement{2,0,3,VT_FLOAT32,False}, //tex-coord
        LayoutElement{3,0,3,VT_FLOAT32,False }, //norm
        LayoutElement{4,0,3,VT_FLOAT32,False}, //bi-norm
        LayoutElement{5,0,3,VT_FLOAT32,False} //tangent
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
        {SHADER_TYPE_PIXEL,"g_TextureNorm",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
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
        {SHADER_TYPE_PIXEL,"g_TextureNorm",SamLinearClampDesc}
    };

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO_Lit_FP);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_PSO_Lit_FP->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_LitConstants);

    // Create a shader resource binding object and bind all static resources in it
    m_PSO_Lit_FP->CreateShaderResourceBinding(&m_SRB_Lit, true);

    //Second pass GP

   
    BlendState.RenderTargets[0].BlendEnable = true;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_ONE;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_ONE;
    //BlendState.RenderTargets[0].BlendEnable



    //GraphicsPipelineStateCreateInfo PSOCreateInfo;
    //Application* app = Application::GetApp();

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Mesh3D - LitGP SP PSO";

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

    //ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;

    // In this tutorial, we will load shaders from file. To be able to do that,
    // we need to create a shader source stream factory
    //RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    //app->GetFactory()->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    //ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;

    // Create a vertex shader


    // Create a pixel shader

    // clang-format off
    // Define vertex shader input layout
 
    // clang-format on
    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

   // ShaderResourceVariableDesc Vars[] =
    //{
     //   {SHADER_TYPE_PIXEL, "g_Texture",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC},
      //  {SHADER_TYPE_PIXEL,"g_TextureNorm",SHADER_RESOURCE_VARIABLE_TYPE_DYNAMIC}
    //};

    PSOCreateInfo.PSODesc.ResourceLayout.Variables = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

 

    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_PSO_Lit_SP);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_PSO_Lit_SP->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_LitConstants);

    // Create a shader resource binding object and bind all static resources in it
    m_PSO_Lit_SP->CreateShaderResourceBinding(&m_SRB_Lit, true);
}

float angX = 0;


void MeshRenderer::RenderLit(NodeEntity* entity, NodeCamera* cam, NodeLight* light,bool firstPass) {

    Application* gApp = Application::GetApp();

    auto m_pImmediateContext = gApp->GetContext();
  


    if (firstPass) {

        for (int i = 0;i < entity->GetMeshes().size();i++)
        {
            auto mesh = entity->GetMesh(i);

           

            auto tex_view = mesh->GetMaterial()->GetColorMap()->GetView();
            auto norm_view = mesh->GetMaterial()->GetNormalMap()->GetView();
         
            m_SRB_Lit->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(tex_view, SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
            m_SRB_Lit->GetVariableByName(SHADER_TYPE_PIXEL, "g_TextureNorm")->Set(norm_view, SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);

         

            float4x4 proj = float4x4::OrthoOffCenter(0, gApp->GetWidth(), gApp->GetHeight(), 0, 0, 100.0f, false);

            //float4x4 model = float4x4::RotationY(Maths::Deg2Rad(angX));// *float4x4::RotationX(-PI_F * 0.1f);

            float4x4 model = entity->GetWorldMatrix();
            angX = angX + 0.1f;
            // Camera is at (0, 0, -5) looking along the Z axis
            float4x4 View = cam->GetWorldMatrix().Inverse();;// float4x4::Translation(0.f, 0.0f, 5.0f);

            // Get pretransform matrix that rotates the scene according the surface orientation
            //auto SrfPreTransform = GetSurfacePretransformMatrix(float3{ 0, 0, 1 });

            // Get projection matrix adjusted to the current screen orientation
            auto Proj = cam->GetProjectionMatrix();  //float4x4::Projection( Maths::Deg2Rad(70.0f), 1024.0f / 760.0f, 0.001f, 1000.0f, false);

            // Compute world-view-projection matrix
            float4x4 m_WorldViewProjMatrix = View * Proj;

            auto cont = gApp->GetContext();

            //model = float4x4::Identity();


            LitConstants lc;
            lc.g_MVP = m_WorldViewProjMatrix.Transpose();

            lc.g_Model = model.Transpose();
            lc.g_ModelInv = model.Transpose().Inverse();
            lc.g_View = View;
            lc.g_Proj = Proj.Transpose();
            lc.viewPos = float4(cam->GetPosition(), 1.0);
            lc.lightPos = float4(light->GetPosition(), 1.0);
            lc.lightProp = float4(light->GetRange(), 0, 0, 0);
            lc.lightDiff = float4(light->GetDiffuse(), 0);
            lc.lightSpec = float4(light->GetSpecular(), 0);


            MapHelper<LitConstants> CBConstants(cont, m_LitConstants, MAP_WRITE, MAP_FLAG_DISCARD);
            *CBConstants = lc;
            m_pImmediateContext->SetPipelineState(m_PSO_Lit_FP);

            const Uint64 offset = 0;
            IBuffer* pBuffs[] = { mesh->GetVertexBuffer() };



            m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
            m_pImmediateContext->SetIndexBuffer(mesh->GetIndexBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            // Set the pipeline state

            // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
            // makes sure that resources are transitioned to required states.
            m_pImmediateContext->CommitShaderResources(m_SRB_Lit, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
            DrawAttrs.IndexType = VT_UINT32; // Index type
            DrawAttrs.NumIndices = mesh->GetTris().size() * 3;
            // Verify the state of vertex and index buffers
            DrawAttrs.Flags = DRAW_FLAG_NONE;
            m_pImmediateContext->DrawIndexed(DrawAttrs);
            //ereturn;

       
          
        }

    }
    else {

        for (int i = 0;i < entity->GetMeshes().size();i++)
        {
            auto mesh = entity->GetMesh(i);

            Application* gApp = Application::GetApp();

            auto tex_view = mesh->GetMaterial()->GetColorMap()->GetView();
            auto norm_view = mesh->GetMaterial()->GetNormalMap()->GetView();

            m_SRB_Lit->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(tex_view, SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
            m_SRB_Lit->GetVariableByName(SHADER_TYPE_PIXEL, "g_TextureNorm")->Set(norm_view, SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);



            auto m_pImmediateContext = gApp->GetContext();

            float4x4 proj = float4x4::OrthoOffCenter(0, gApp->GetWidth(), gApp->GetHeight(), 0, 0, 100.0f, false);

            //float4x4 model = float4x4::RotationY(Maths::Deg2Rad(angX));// *float4x4::RotationX(-PI_F * 0.1f);

            float4x4 model = entity->GetWorldMatrix();
            angX = angX + 0.1f;
            // Camera is at (0, 0, -5) looking along the Z axis
            float4x4 View = cam->GetWorldMatrix().Inverse();;// float4x4::Translation(0.f, 0.0f, 5.0f);

            // Get pretransform matrix that rotates the scene according the surface orientation
            //auto SrfPreTransform = GetSurfacePretransformMatrix(float3{ 0, 0, 1 });

            // Get projection matrix adjusted to the current screen orientation
            auto Proj = cam->GetProjectionMatrix();  //float4x4::Projection( Maths::Deg2Rad(70.0f), 1024.0f / 760.0f, 0.001f, 1000.0f, false);

            // Compute world-view-projection matrix
            float4x4 m_WorldViewProjMatrix = View * Proj;

            auto cont = gApp->GetContext();

            //model = float4x4::Identity();


            LitConstants lc;
            lc.g_MVP = m_WorldViewProjMatrix.Transpose();

            lc.g_Model = model.Transpose();
            lc.g_ModelInv = model.Transpose().Inverse();
            lc.g_View = View;
            lc.g_Proj = Proj.Transpose();
            lc.viewPos = float4(cam->GetPosition(), 1.0);
            lc.lightPos = float4(light->GetPosition(), 1.0);
            lc.lightProp = float4(light->GetRange(), 0, 0, 0);
            lc.lightDiff = float4(light->GetDiffuse(), 0);
            lc.lightSpec = float4(light->GetSpecular(), 0);


            MapHelper<LitConstants> CBConstants(cont, m_LitConstants, MAP_WRITE, MAP_FLAG_DISCARD);
            *CBConstants = lc;
            m_pImmediateContext->SetPipelineState(m_PSO_Lit_SP);

            const Uint64 offset = 0;
            IBuffer* pBuffs[] = { mesh->GetVertexBuffer() };



            m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
            m_pImmediateContext->SetIndexBuffer(mesh->GetIndexBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            // Set the pipeline state

            // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
            // makes sure that resources are transitioned to required states.
            m_pImmediateContext->CommitShaderResources(m_SRB_Lit, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
            DrawAttrs.IndexType = VT_UINT32; // Index type
            DrawAttrs.NumIndices = mesh->GetTris().size() * 3;
            // Verify the state of vertex and index buffers
            DrawAttrs.Flags = DRAW_FLAG_NONE;
            m_pImmediateContext->DrawIndexed(DrawAttrs);
            //ereturn;

        }

    }
     
}

void MeshRenderer::RenderSimple(NodeEntity* entity, NodeCamera* cam) {

    int a = 5;


    for (int i = 0;i < entity->GetMeshes().size();i++)
    {
        auto mesh = entity->GetMesh(i);

        Application* gApp = Application::GetApp();

        auto tex_view = mesh->GetMaterial()->GetColorMap()->GetView();

        m_SRB_Basic->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(tex_view, SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);



        auto m_pImmediateContext = gApp->GetContext();

        float4x4 proj = float4x4::OrthoOffCenter(0, gApp->GetWidth(), gApp->GetHeight(), 0, 0, 100.0f, false);

        //float4x4 model = float4x4::RotationY(Maths::Deg2Rad(angX));// *float4x4::RotationX(-PI_F * 0.1f);

        float4x4 model = entity->GetWorldMatrix();
        angX = angX + 0.1f;
        // Camera is at (0, 0, -5) looking along the Z axis
        float4x4 View = cam->GetWorldMatrix();// float4x4::Translation(0.f, 0.0f, 5.0f);

        // Get pretransform matrix that rotates the scene according the surface orientation
        //auto SrfPreTransform = GetSurfacePretransformMatrix(float3{ 0, 0, 1 });

        // Get projection matrix adjusted to the current screen orientation
        auto Proj = cam->GetProjectionMatrix();  //float4x4::Projection( Maths::Deg2Rad(70.0f), 1024.0f / 760.0f, 0.001f, 1000.0f, false);

        // Compute world-view-projection matrix
        float4x4 m_WorldViewProjMatrix = model * View * Proj;

        auto cont = gApp->GetContext();

        MapHelper<float4x4> CBConstants(cont, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = m_WorldViewProjMatrix.Transpose();
        m_pImmediateContext->SetPipelineState(m_PSO_Basic);

        const Uint64 offset = 0;
        IBuffer* pBuffs[] = { mesh->GetVertexBuffer() };



        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(mesh->GetIndexBuffer(), 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state

        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_SRB_Basic, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
        DrawAttrs.IndexType = VT_UINT32; // Index type
        DrawAttrs.NumIndices = mesh->GetTris().size() * 3;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_NONE;
        m_pImmediateContext->DrawIndexed(DrawAttrs);
    }

}