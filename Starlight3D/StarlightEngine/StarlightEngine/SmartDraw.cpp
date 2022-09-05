#include "pch.h"
#include "SmartDraw.h"


SmartDraw::SmartDraw(Application* app) {


    gApp = app;

    //cZ = 1.00f;

    BlendStateDesc BlendState;
    BlendState.RenderTargets[0].BlendEnable = true;
    BlendState.RenderTargets[0].SrcBlend = BLEND_FACTOR_SRC_ALPHA;
    BlendState.RenderTargets[0].DestBlend = BLEND_FACTOR_INV_SRC_ALPHA;

     vertices = new Vertex[2048];
     indices = new Uint32[4096];

    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "SmartDraw PSO";

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
        ShaderCI.Desc.Name = "Cube VS";
        ShaderCI.FilePath = "data/draw.vsh";
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
        ShaderCI.Desc.Name = "Cube PS";
        ShaderCI.FilePath = "data/draw.psh";
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

    app->GetDevice()->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

    // Since we did not explcitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly through the pipeline state object.
    m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);

    // Create a shader resource binding object and bind all static resources in it
     m_pPSO->CreateShaderResourceBinding(&m_pSRB,true);

}

void SmartDraw::CreateVertexBuffer(DrawList* list) {

    


    int size = list->Draws.size();






    int vi = 0;

    


    for (int i = 0;i < size;i++) {

        auto dr = list->Draws[i];
        float4 col = float4(dr->r, dr->g, dr->b, dr->a);

        vertices[vi].pos = float3(dr->x[0], dr->y[0], dr->z);
        vertices[vi].color = col;
        vertices[vi].uv = float2(0, 0);
        vi++;
        vertices[vi].pos = float3(dr->x[1], dr->y[1], dr->z);
        vertices[vi].color = col;
        vertices[vi].uv = float2(1, 0);
        vi++;
        vertices[vi].pos = float3(dr->x[2], dr->y[2], dr->z);
        vertices[vi].color = col;
        vertices[vi].uv = float2(1, 1);
        vi++;
        vertices[vi].pos = float3(dr->x[3], dr->y[3], dr->z);
        vertices[vi].color = col;
        vertices[vi].uv = float2(0, 1);
        vi++;
    }

    int ds = ((3 * 4) + (4 * 4) + (2*4)) * (size * 4);

    if (made) {
        m_CubeVertexBuffer.Detach();
        m_CubeVertexBuffer.Release();
    }

    BufferDesc VertBuffDesc;
    VertBuffDesc.Name = "SmartDraw vertex buffer";
    VertBuffDesc.Usage = USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
    VertBuffDesc.Size = ds;
    BufferData VBData;
    VBData.pData = &vertices[0];
    VBData.DataSize = ds;
    gApp->GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &m_CubeVertexBuffer);
    
   // for (int i = 0;i < size * 4;i++) {
  //      delete &vertices[i];
//    }

  //  delete vertices;


}

void SmartDraw::CreateIndexBuffer(DrawList* list) {

    

    int size = list->Draws.size();

  

    int vi = 0;
    int cv = 0;


    for (int i = 0;i < size;i++) {

        indices[vi] = cv;
        indices[vi + 1] = cv + 1;
        indices[vi + 2] = cv + 2;
        indices[vi + 3] = cv + 2;
        indices[vi + 4] = cv + 3;
        indices[vi + 5] = cv;
        cv = cv + 4;
        vi = vi + 6;



    }

    int ds = 4 * size * 6;

    if (made) {
        m_CubeIndexBuffer.Detach();
        m_CubeIndexBuffer.Release();
    }

    BufferDesc IndBuffDesc;
    IndBuffDesc.Name = "SmartDraw index buffer";
    IndBuffDesc.Usage = USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags = BIND_INDEX_BUFFER;
    IndBuffDesc.Size = ds;
    BufferData IBData;
    IBData.pData = &indices[0];
    IBData.DataSize = ds;
    gApp->GetDevice()->CreateBuffer(IndBuffDesc, &IBData, &m_CubeIndexBuffer);

  //  delete indices;
  

}

void SmartDraw::Begin() {

    for (int i = 0; i < Draws.size(); i++) {

        auto dd = Draws[i];

        for (int d = 0; d < dd->Draws.size(); d++) {

            auto d2 = dd->Draws[d];

            delete d2;

        }

        delete dd;

    }
    Draws.resize(0);
    drawZ = 1.0f;

    

}

void SmartDraw::DrawTexture(int x, int y, int w, int h, Texture2D* tex, float r, float g, float b, float a) {

    DrawList* list = GetDrawList(tex);

    DrawInfo* info = new DrawInfo();

    info->x[0] = x;
		info->x[1] = x + w;
		info->x[2] = x + w;
		info->x[3] = x;

		info->y[0] = y;
		info->y[1] = y;
		info->y[2] = y + h;
		info->y[3] = y + h;

		info->Tex = tex;

		info->z = drawZ;
		info->r = r;
		info->g = g;
		info->b = b;
		info->a = a;

        drawZ -= 0.0001f;

		list->Draws.push_back(info);


}

void SmartDraw::DrawQuad(int x, int y, int w, int h, float r, float g, float b, float a) {

    DrawList* list = Draws[0];

    DrawInfo* info = new DrawInfo();
    info->x[0] = x;
    info->x[1] = x + w;
    info->x[2] = x + w;
    info->x[3] = x;

    info->y[0] = y;
    info->y[1] = y;
    info->y[2] = y + h;
    info->y[3] = y + h;

  //  info->Tex = tex;

    info->z = drawZ;
    info->r = r;
    info->g = g;
    info->b = b;
    info->a = a;

    drawZ -= 0.01f;

    list->Draws.push_back(info);


}

void SmartDraw::End() {

    //if (!made) {
   
    for (int i = 0;i < Draws.size();i++)
    {
        CreateVertexBuffer(Draws[i]);
        CreateIndexBuffer(Draws[i]);
        made = true;

        auto dd = Draws[i];

        m_pSRB->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(dd->Tex->GetView(), SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);


        auto m_pImmediateContext = gApp->GetContext();

        float4x4 proj = float4x4::OrthoOffCenter(0, gApp->GetWidth(), gApp->GetHeight(), 0, 0, 100.0f, false);

        auto cont = gApp->GetContext();

        MapHelper<float4x4> CBConstants(cont, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = proj.Transpose();
        m_pImmediateContext->SetPipelineState(m_pPSO);

        const Uint64 offset = 0;
        IBuffer* pBuffs[] = { m_CubeVertexBuffer };

     

        m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, &offset, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
        m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        // Set the pipeline state
       
        // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
        // makes sure that resources are transitioned to required states.
        m_pImmediateContext->CommitShaderResources(m_pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

        DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
        DrawAttrs.IndexType = VT_UINT32; // Index type
        DrawAttrs.NumIndices = Draws[i]->Draws.size() * 6;
        // Verify the state of vertex and index buffers
        DrawAttrs.Flags = DRAW_FLAG_NONE;
        m_pImmediateContext->DrawIndexed(DrawAttrs);
        //m_pImmediateContext->Flush();
  //      m_pImmediateContext->Flush();
     

    }
  //  auto m_pImmediateContext = gApp->GetContext();
   
    

}