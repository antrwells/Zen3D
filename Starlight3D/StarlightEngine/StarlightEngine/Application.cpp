#include "pch.h"
#include "Application.h"
#include "UI.h"
//RefCntAutoPtr<IRenderDevice> Application::s_pDevice

Application* Application::s_pThis = nullptr;



Application::Application() {

    s_pThis = this;
    mInput = new UserInput();
     
}

bool Application::Initialize(HWND hWnd) {

    SwapChainDesc SCDesc;
    SCDesc.ColorBufferFormat = TEX_FORMAT_RGBA8_UNORM;
    SCDesc.DepthBufferFormat = TEX_FORMAT_D16_UNORM;
    switch (m_DeviceType)
    {
#if D3D11_SUPPORTED
    case RENDER_DEVICE_TYPE_D3D11:
    {
        EngineD3D11CreateInfo EngineCI;
#    if ENGINE_DLL
        // Load the dll and import GetEngineFactoryD3D11() function
        auto* GetEngineFactoryD3D11 = LoadGraphicsEngineD3D11();
#    endif
        auto* pFactoryD3D11 = GetEngineFactoryD3D11();
        pFactoryD3D11->CreateDeviceAndContextsD3D11(EngineCI, &m_pDevice, &m_pImmediateContext);
        Win32NativeWindow Window{ hWnd };
        pFactoryD3D11->CreateSwapChainD3D11(m_pDevice, m_pImmediateContext, SCDesc, FullScreenModeDesc{}, Window, &m_pSwapChain);
    }
    break;
#endif


#if D3D12_SUPPORTED
    case RENDER_DEVICE_TYPE_D3D12:
    {
#    if ENGINE_DLL
        // Load the dll and import GetEngineFactoryD3D12() function
        auto GetEngineFactoryD3D12 = LoadGraphicsEngineD3D12();
#    endif
        EngineD3D12CreateInfo EngineCI;

        auto* pFactoryD3D12 = GetEngineFactoryD3D12();
        pFactoryD3D12->CreateDeviceAndContextsD3D12(EngineCI, &m_pDevice, &m_pImmediateContext);
        Win32NativeWindow Window{ hWnd };
        pFactoryD3D12->CreateSwapChainD3D12(m_pDevice, m_pImmediateContext, SCDesc, FullScreenModeDesc{}, Window, &m_pSwapChain);
        m_pEngFac = pFactoryD3D12;
    }
    break;
#endif


#if GL_SUPPORTED
    case RENDER_DEVICE_TYPE_GL:
    {
#    if EXPLICITLY_LOAD_ENGINE_GL_DLL
        // Load the dll and import GetEngineFactoryOpenGL() function
        auto GetEngineFactoryOpenGL = LoadGraphicsEngineOpenGL();
#    endif
        auto* pFactoryOpenGL = GetEngineFactoryOpenGL();

        EngineGLCreateInfo EngineCI;
        EngineCI.Window.hWnd = hWnd;

        pFactoryOpenGL->CreateDeviceAndSwapChainGL(EngineCI, &m_pDevice, &m_pImmediateContext, SCDesc, &m_pSwapChain);
    }
    break;
#endif


#if VULKAN_SUPPORTED
    case RENDER_DEVICE_TYPE_VULKAN:
    {
#    if EXPLICITLY_LOAD_ENGINE_VK_DLL
        // Load the dll and import GetEngineFactoryVk() function
        auto GetEngineFactoryVk = LoadGraphicsEngineVk();
#    endif
        EngineVkCreateInfo EngineCI;

        auto* pFactoryVk = GetEngineFactoryVk();
        pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_pDevice, &m_pImmediateContext);

        if (!m_pSwapChain && hWnd != nullptr)
        {
            Win32NativeWindow Window{ hWnd };
            pFactoryVk->CreateSwapChainVk(m_pDevice, m_pImmediateContext, SCDesc, Window, &m_pSwapChain);
        }
    }
    break;
#endif


    default:
        std::cerr << "Unknown/unsupported device type";
        return false;
        break;
    }

    return true;

}


static const char* VSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};
void main(in  uint    VertId : SV_VertexID,
          out PSInput PSIn) 
{
    float4 Pos[3];
    Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
    Pos[1] = float4( 0.0, +0.5, 0.0, 1.0);
    Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);
    float3 Col[3];
    Col[0] = float3(1.0, 0.0, 0.0); // red
    Col[1] = float3(0.0, 1.0, 0.0); // green
    Col[2] = float3(0.0, 0.0, 1.0); // blue
    PSIn.Pos   = Pos[VertId];
    PSIn.Color = Col[VertId];
}
)";

// Pixel shader simply outputs interpolated vertex color
static const char* PSSource = R"(
struct PSInput 
{ 
    float4 Pos   : SV_POSITION; 
    float3 Color : COLOR; 
};
struct PSOutput
{ 
    float4 Color : SV_TARGET; 
};
void main(in  PSInput  PSIn,
          out PSOutput PSOut)
{
    PSOut.Color = float4(PSIn.Color.rgb, 1.0);
}
)";


void Application::CreateResources() {

    // Pipeline state object encompasses configuration of all GPU stages
    //return;
    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Simple triangle PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_pSwapChain->GetDesc().ColorBufferFormat;
    // Use the depth buffer format from the swap chain
    PSOCreateInfo.GraphicsPipeline.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // No back face culling for this tutorial
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    // Disable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = False;
    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;
    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.UseCombinedTextureSamplers = true;
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Triangle vertex shader";
        ShaderCI.Source = VSSource;
        m_pDevice->CreateShader(ShaderCI, &pVS);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint = "main";
        ShaderCI.Desc.Name = "Triangle pixel shader";
        ShaderCI.Source = PSSource;
        m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // Finally, create the pipeline state
    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;
    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

    Init();
    InitApp();


}

void Application::Present() {

    m_pSwapChain->Present();

}

void Application::Render() {

    UpdateApp();


    //RenderApp();



    // Set render targets before issuing any draw command.
        // Note that Present() unbinds the back buffer if it is set as render target.
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Clear the back buffer
    const float ClearColor[] = { 0.5f,0.0f,1.0f, 0.0f };
    // Let the engine perform required state transitions
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    RenderApp();

    return;

    // Set the pipeline state in the immediate context
    m_pImmediateContext->SetPipelineState(m_pPSO);

    // Typically we should now call CommitShaderResources(), however shaders in this example don't
    // use any resources.

    DrawAttribs drawAttrs;
    drawAttrs.NumVertices = 3; // Render 3 vertices
    m_pImmediateContext->Draw(drawAttrs);


}


void Application::CrWindow(const char* title, int width, int height, int hint) {

    if (glfwInit() != GLFW_TRUE)
        return;

    glfwWindowHint(GLFW_CLIENT_API, hint);
    if (hint == GLFW_OPENGL_API)
    {
        // We need compute shaders, so request OpenGL 4.2 at least
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    }

    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_Window == nullptr)
    {
        LOG_ERROR_MESSAGE("Failed to create GLFW window");
        return;
    }
    winWidth = width;
    winHeight = height;
    glfwSetWindowUserPointer(m_Window, this);
    //glfwSetFramebufferSizeCallback(m_Window, &GLFW_ResizeCallback);
   // glfwSetKeyCallback(m_Window, &GLFW_KeyCallback);
   // glfwSetMouseButtonCallback(m_Window, &GLFW_MouseButtonCallback);
  //  glfwSetCursorPosCallback(m_Window, &GLFW_CursorPosCallback);
 //   glfwSetScrollCallback(m_Window, &GLFW_MouseWheelCallback);
    glfwSetFramebufferSizeCallback(m_Window, &GLFW_ResizeCallback);
    glfwSetKeyCallback(m_Window, &GLFW_KeyCallback);
    glfwSetMouseButtonCallback(m_Window, &GLFW_MouseButtonCallback);
    //glfwSetCursorPosCallback(m_Window, &GLFW_CursorPosCallback);
    glfwSetScrollCallback(m_Window, &GLFW_MouseWheelCallback);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetWindowSizeLimits(m_Window, 320, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);
    return;

}


void Application::InitEngine() {

    //Win32NativeWindow Window{ glfwGetWin32Window(m_Window) };
    Initialize(glfwGetWin32Window(m_Window));


}


void Application::Run() {

   

    while (true) {

        double xp, yp;
        glfwGetCursorPos(m_Window, &xp, &yp);

        if (mMouseFirst) {
            mMouseX = xp;
            mMouseY = yp;
            mMouseFirst = false;
        }

        mDeltaX = xp - mMouseX;
        mDeltaY = yp - mMouseY;

        mMouseX = xp;
        mMouseY = yp;

        mInput->SetMouse(mMouseX, mMouseY, mDeltaX, mDeltaY);

        Render();
        Present();

        glfwPollEvents();

        //std::cout << "MX:" << mDeltaX << " MY:" << mDeltaY << std::endl;

    }

}



void Application::GLFW_ResizeCallback(GLFWwindow* wnd, int w, int h)
{
    //auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
    //if (pSelf->m_pSwapChain != nullptr)
      //  pSelf->m_pSwapChain->Resize(static_cast<Uint32>(w), static_cast<Uint32>(h));
}

void Application::GLFW_KeyCallback(GLFWwindow* wnd, int key, int, int state, int)
{

    KeyID id = KeyID::None;

    switch (key) {
    case GLFW_KEY_A:
        id = KeyID::A;
        break;
    case GLFW_KEY_B:
        id = KeyID::B;
        break;
    case GLFW_KEY_C:
        id = KeyID::C;
        break;
    case GLFW_KEY_D:
        id = KeyID::D;
        break;
    case GLFW_KEY_E:
        id = KeyID::E;
        break;
    case GLFW_KEY_F:
        id = KeyID::F;
        break;
    case GLFW_KEY_G:
        id = KeyID::G;
        break;
    case GLFW_KEY_H:
        id = KeyID::H;
        break;
    case GLFW_KEY_I:
        id = KeyID::I;
        break;
    case GLFW_KEY_J:
        id = KeyID::J;
        break;
    case GLFW_KEY_K:
        id = KeyID::K;
        break;
    case GLFW_KEY_L:
        id = KeyID::L;
        break;
    case GLFW_KEY_M:
        id = KeyID::M;
        break;
    case GLFW_KEY_N:
        id = KeyID::N;
        break;
     case GLFW_KEY_O:
        id = KeyID::O;
        break;
     case GLFW_KEY_P:
         id = KeyID::P;
         break;
     case GLFW_KEY_Q:
         id = KeyID::Q;
         break;
     case GLFW_KEY_R:
         id = KeyID::R;
         break;
     case GLFW_KEY_S:
         id = KeyID::S;
         break;
     case GLFW_KEY_T:
         id = KeyID::T;
         break;
     case GLFW_KEY_U:
         id = KeyID::U;
         break;
     case GLFW_KEY_V:
         id = KeyID::V;
         break;
     case GLFW_KEY_W:
         id = KeyID::W;
         break;
     case GLFW_KEY_X:
         id = KeyID::X;
         break;
     case GLFW_KEY_Y:
         id = KeyID::Y;
         break;
     case GLFW_KEY_Z:
         id = KeyID::Z;
         break;
     case GLFW_KEY_SPACE:
         id = KeyID::Space;
         break;
     case GLFW_KEY_ENTER:
         id = KeyID::Return;
         break;
    }

    if (id == KeyID::None)
    {
        return;
    }
    if (state == GLFW_PRESS) {
        Application::GetApp()->GetInput()->SetKey(id, true);
    }
    else if(state == GLFW_RELEASE){
       Application::GetApp()->GetInput()->SetKey(id, false);
    }

    //auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
    //pSelf->OnKeyEvent(static_cast<Key>(key), static_cast<KeyState>(state));
}

void Application::GLFW_MouseButtonCallback(GLFWwindow* wnd, int button, int state, int)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (state == GLFW_PRESS) {
            UI::SetMouseBut(0, true);
        }
        else {
            UI::SetMouseBut(0, false);
        }


    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (state == GLFW_PRESS) {
            UI::SetMouseBut(1, true);
        }
        else {
            UI::SetMouseBut(1, false);
        }
    }
    // auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
    // pSelf->OnKeyEvent(static_cast<Key>(button), static_cast<KeyState>(state));
}

void Application::GLFW_CursorPosCallback(GLFWwindow* wnd, double xpos, double ypos)
{
    float xscale = 1;
    float yscale = 1;
    s_pThis->SetMousePos(xpos, ypos);
    
    //glfwGetWindowContentScale(wnd, &xscale, &yscale);
    //auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
   // pSelf->MouseEvent(float2(static_cast<float>(xpos * xscale), static_cast<float>(ypos * yscale)));

}

void Application::GLFW_MouseWheelCallback(GLFWwindow* wnd, double dx, double dy)
{
}
