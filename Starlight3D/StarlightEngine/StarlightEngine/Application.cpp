#include "pch.h"
#include "Application.h"

#include "ScriptHost.h"
#include "FSPayload.h"
#include "VString.h"
#include "Maths.h"
#include "Texture2D.h"
#include "kFont.h"
#include "SmartDraw.h"
//#include "ZContextVar.h"

//#include "ImGuiImplWin32.hpp"


//RefCntAutoPtr<IRenderDevice> Application::s_pDevice

Application* Application::s_pThis = nullptr;



Application::Application() {

    s_pThis = this;
    mInput = new UserInput();

    //ZSource* source = new ZSource("script/test.zs");
    //ZTokenizer* toker = new ZTokenizer(source);
    //auto stream = toker->Tokenize();
    //ZParser* parser = new ZParser(stream);
    //ZMainNode* main1 = parser->Parse();

    //ZScriptContext* context1 = new ZScriptContext;

    //context1->AddNode(main1);

    //auto cls_inst = context1->CreateInstance("testClass", "test");

    //auto par_a = new ZContextVar("a", VarType::VarInt);
    //auto par_b = new ZContextVar("b", VarType::VarInt);
    //par_a->SetInt(5);
    //par_b->SetInt(10);

   // cls_inst->CallMethod("testMethod", { par_a,par_b });
    auto host = new ScriptHost;
    int aaa = 5;

}

bool Application::Initialize(HWND hWnd) {

    curWin = hWnd;

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
        m_pEngFac = pFactoryD3D11;
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
        EngineCI.Features.RayTracing = DEVICE_FEATURE_STATE_ENABLED;
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
        EngineCI.Features.RayTracing = DEVICE_FEATURE_STATE_ENABLED;
        auto* pFactoryVk = GetEngineFactoryVk();
        pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_pDevice, &m_pImmediateContext);

        if (!m_pSwapChain && hWnd != nullptr)
        {
            Win32NativeWindow Window{ hWnd };
            pFactoryVk->CreateSwapChainVk(m_pDevice, m_pImmediateContext, SCDesc, Window, &m_pSwapChain);
        }
        m_pEngFac = pFactoryVk;
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

 //  m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

   

  


}

void Application::Present() {
    try {
        m_pSwapChain->Present();
    }
    catch (...) {

    }
}
bool mouse1_in = false;
bool mouse2_in = false;
bool mouse3_in = false;
void Application::Render() {
    auto SC = m_pSwapChain->GetDesc();

    UINT message = 0;
    WPARAM wParam = 0;
    LPARAM lParam = 0;

    //if(this->mInput)

    if (Application::GetApp()->GetInput()->IsMouseDown(0))
    {
        if (mouse1_in == false) {

            message = WM_LBUTTONDOWN;
            mouse1_in = true;

        }
    }
    else {
        if (mouse1_in) {

            mouse1_in = false;
            message = WM_LBUTTONUP;


        }
    }

    
    if (Application::GetApp()->GetInput()->IsMouseDown(1))
    {
        if (mouse2_in == false) {

            message = WM_RBUTTONDOWN;
            mouse2_in = true;

        }
    }
    else {
        if (mouse2_in) {

            mouse2_in = false;
            message = WM_RBUTTONUP;


        }

    }
  
    int m1 = 0;
    int m2 = 0;

    if (mouse1_in)
    {
        m1 = 1;
    }
    if (mouse2_in) {
        m2 = 1;
    }
   // printf("M1:%d M2:%d\n", m1, m2);
 
    
   // UpdateApp();


    //RenderApp();


    
    // Set render targets before issuing any draw command.
        // Note that Present() unbinds the back buffer if it is set as render target.
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
    m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Clear the back buffer
    const float ClearColor[] = { 0,0,0, 0.0f };
    // Let the engine perform required state transitions
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
   
 // RenderApp();

    m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    
    auto Handled = static_cast<ImGuiImplWin32*>(m_pImGui.get())->Win32_ProcHandler(curWin, message, wParam, lParam);

    message = WM_KEYDOWN;
    wParam = 0x41;
    lParam = 0x41;

    //auto Handled2 = static_cast<ImGuiImplWin32*>(m_pImGui.get())->Win32_ProcHandler(curWin, message, wParam, lParam);


 
    m_pImGui->NewFrame(SC.Width, SC.Height, SC.PreTransform);
    RenderApp();
   
    m_pImGui->EndFrame();
    m_pImGui->Render(m_pImmediateContext);
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
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);


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
    //glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetWindowSizeLimits(m_Window, 320, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(m_Window, (mode->width - width) / 2, (mode->height - height) / 2);
    
    return;

}


void Application::InitEngine() {

    //Win32NativeWindow Window{ glfwGetWin32Window(m_Window) };
    Initialize(glfwGetWin32Window(m_Window));


}


void Application::Run() {

    bool is_splash = true;
    int splash_start = clock();
    
    auto splash_tex = new Texture2D("edit/splash1.png");
    auto white_tex = new Texture2D("edit/white.png");
    auto draw = new SmartDraw(this);
    auto font = new kFont("data/fonts/aqua.pf");

    SetPayload();

    float cl = 1.0f;

    while (true) {


        int cur_time = clock();

        if (is_splash) {
            if (mCurrentPayload == nullptr)
            {
                glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
                glfwSetWindowShouldClose(m_Window,GL_TRUE);
                glfwDestroyWindow(m_Window);

                glfwPollEvents();
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                winWidth = mode->width - 40;
                    winHeight = mode->height - 60;

                m_Window = glfwCreateWindow(winWidth, winHeight, "Zen3D", nullptr, nullptr);
                if (m_Window == nullptr)
                {
                    LOG_ERROR_MESSAGE("Failed to create GLFW window");
                    return;
                }
                glfwSetWindowPos(m_Window,5,10);

                //winWidth = ;
                //winHeight = 450;
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
                InitEngine();
                CreateResources();
                Init();

                mPhysics = new Physics();

                auto SC = m_pSwapChain->GetDesc();
                m_pImGui.reset(new ImGuiImplWin32(curWin, m_pDevice, SC.ColorBufferFormat, SC.DepthBufferFormat));
                ImGuiIO& io = ImGui::GetIO(); (void)io;


                //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
                //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

             

              //  io.WantCaptureKeyboard = true;

                //glfwSetWindowPos(m_Window, 128, 128);
                //glfwSetWindowSize(m_Window, 1400, 860);
                

                WindowResize(winWidth,winHeight);
                is_splash = false;
                mResized = false;
                InitApp();


            }
        }

        double xp, yp;
        glfwGetCursorPos(m_Window, &xp, &yp);

        if (mMouseFirst) {
            mMouseX = (float)xp;
            mMouseY = (float)yp;
            mMouseFirst = false;
        }

        mDeltaX = (int)xp - mMouseX;
        mDeltaY = (int)yp - mMouseY;

        mMouseX = (int)xp;
        mMouseY = (int)yp;

        if (is_splash == false) {
            mInput->SetMouse(mMouseX, mMouseY, mDeltaX, mDeltaY);
            UpdateApp();
            Render();
        }
        else {
            auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
            auto* pDSV = m_pSwapChain->GetDepthBufferDSV();
            m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            // Clear the back buffer
            const float ClearColor[] = { 0,0,0, 0.0f };
            // Let the engine perform required state transitions
            m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
            m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

            // RenderApp();

            m_pImmediateContext->SetRenderTargets(1, &pRTV, pDSV, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
           
            draw->Begin();

            draw->DrawTexture(0, 0, winWidth, winHeight, splash_tex, 1, 1, 1, 1, false);

            float lp2 = mCurrentPayload->LoadedPercent() * 100.0f;

            printf("lp:%f", lp2);
            printf("\n");

            std::string load = "Loading Zen Resources..." + std::to_string((int)lp2) + std::string("/100");

            font->drawText(load.c_str(), 20, 20, 1, 1, 1, 1, draw);
            
            float cx, cy;
            float com = mCurrentPayload->LoadedPercent();
            
            cx = 565;
            cy = 315;

            int size = 35;

            cl = cl - 0.01f;
            if (cl < 0.f)
            {
                cl = 1.0f+cl;
            }

            for (int a = 0; a < 360; a+=20) {

                float dx, dy;
                dx = cx +  cos(Maths::Deg2Rad((float)a)) * size;
                dy = cy + sin(Maths::Deg2Rad((float)a)) * size;

                float cv = 1.0f;

                cv = ((float)a) / 360.0f;
                cv = 1.0f - cv;

                cv = cv - cl;

                if (cv < 0.0f) {
                    cv = 1.0f + cv;
                }
                //cv = 0.4;
             
                cv = 1.0f - cv;
                draw->DrawTexture((int)dx-5, (int)dy-5,11,11, white_tex, 0, cv, cv, cv, false);
                //printf("A:%d\n", a);
                //printf("CV:%f\n", cv);
            }

            int bb = 5;
            auto cp = mCurrentPayload;

            float lp = mCurrentPayload->LoadedPercent();

            VString load_string((int)(lp * 100));
         

       //     font->drawText(load_string.GetConst(), 20, 50, 1, 0.2, 0.2, 1);

            //std::string loads = std::string((lp * 100));



            if (cp->Loaded())
            {
                mCurrentPayload = nullptr;
      

                int aa = 5;

            }

            std::string res = "";


            draw->End();

        }
        Present();

        glfwPollEvents();

        //std::cout << "MX:" << mDeltaX << " MY:" << mDeltaY << std::endl;

    }

}



void Application::GLFW_ResizeCallback(GLFWwindow* wnd, int w, int h)
{
    //auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
    Application::GetApp()->WindowResize(w, h);

    //if (pSelf->m_pSwapChain != nullptr)
      //  pSelf->m_pSwapChain->Resize(static_cast<Uint32>(w), static_cast<Uint32>(h))

}

void Application::GLFW_KeyCallback(GLFWwindow* wnd, int key, int scancode, int state, int mod)
{

    KeyID id = KeyID::None;

    switch (key) {
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:

        id = KeyID::Shift;
        break;

    case GLFW_KEY_A:
        id = KeyID::KeyA;
        break;
    case GLFW_KEY_B:
        id = KeyID::KeyB;
        break;
    case GLFW_KEY_C:
        id = KeyID::KeyC;
        break;
    case GLFW_KEY_D:
        id = KeyID::KeyD;
        break;
    case GLFW_KEY_E:
        id = KeyID::KeyE;
        break;
    case GLFW_KEY_F:
        id = KeyID::KeyF;
        break;
    case GLFW_KEY_G:
        id = KeyID::KeyG;
        break;
    case GLFW_KEY_H:
        id = KeyID::KeyH;
        break;
    case GLFW_KEY_I:
        id = KeyID::KeyI;
        break;
    case GLFW_KEY_J:
        id = KeyID::KeyJ;
        break;
    case GLFW_KEY_K:
        id = KeyID::KeyK;
        break;
    case GLFW_KEY_L:
        id = KeyID::KeyL;
        break;
    case GLFW_KEY_M:
        id = KeyID::KeyM;
        break;
    case GLFW_KEY_N:
        id = KeyID::KeyN;
        break;
     case GLFW_KEY_O:
        id = KeyID::KeyO;
        break;
     case GLFW_KEY_P:
         id = KeyID::KeyP;
         break;
     case GLFW_KEY_Q:
         id = KeyID::KeyQ;
         break;
     case GLFW_KEY_R:
         id = KeyID::KeyR;
         break;
     case GLFW_KEY_S:
         id = KeyID::KeyS;
         break;
     case GLFW_KEY_T:
         id = KeyID::KeyT;
         break;
     case GLFW_KEY_U:
         id = KeyID::KeyU;
         break;
     case GLFW_KEY_V:
         id = KeyID::KeyV;
         break;
     case GLFW_KEY_W:
         id = KeyID::KeyW;
         break;
     case GLFW_KEY_X:
         id = KeyID::KeyX;
         break;
     case GLFW_KEY_Y:
         id = KeyID::KeyY;
         break;
     case GLFW_KEY_Z:
         id = KeyID::KeyZ;
         break;
     case GLFW_KEY_SPACE:
         id = KeyID::Space;
         break;
     case GLFW_KEY_ENTER:
         id = KeyID::Return;
         break;
    }


    if (state == GLFW_PRESS) {
        Application::GetApp()->GetInput()->SetKey(id, true);
    }
    else if(state == GLFW_RELEASE){
       Application::GetApp()->GetInput()->SetKey(id, false);
    }
    
   

    if (key >= 'A' && key <= 'Z' && !(mod & GLFW_MOD_SHIFT)) {
        key += 'a' - 'A';
    }
    int action = state;
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
       // io.KeysDown[key] = true;
        if (key != GLFW_KEY_LEFT_SHIFT && key != GLFW_KEY_RIGHT_SHIFT && key != GLFW_KEY_BACKSPACE && key != GLFW_KEY_DELETE && key!= GLFW_KEY_ENTER)
        {

            io.AddInputCharacter(key);
        }
        else {
            if (mod == GLFW_MOD_SHIFT) {
                int nkey = 0;
                if (key == GLFW_KEY_1) {
                 
                }
                //io.AddInputCharacter(nkey);
            }
        }

    }
    //if (action == GLFW_RELEASE)
     //   io.KeysDown[key] = false;

   
   // printf("KEy down!!!!!!!!!!!!!!!\n");

    //auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
    //pSelf->OnKeyEvent(static_cast<Key>(key), static_cast<KeyState>(state));
}

void Application::GLFW_MouseButtonCallback(GLFWwindow* wnd, int button, int state, int)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (state == GLFW_PRESS) {
           // UI::SetMouseBut(0, true);
            Application::GetApp()->GetInput()->SetMouseDown(0, true);
        }
        else {
          //  UI::SetMouseBut(0, false);
            Application::GetApp()->GetInput()->SetMouseDown(0, false);
        }


    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (state == GLFW_PRESS) {
          //  UI::SetMouseBut(1, true);
            Application::GetApp()->GetInput()->SetMouseDown(1, true);
        }
        else {
         //   UI::SetMouseBut(1, false);
            Application::GetApp()->GetInput()->SetMouseDown(1, false);
        }
    }
    // auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
    // pSelf->OnKeyEvent(static_cast<Key>(button), static_cast<KeyState>(state));
}

void Application::GLFW_CursorPosCallback(GLFWwindow* wnd, double xpos, double ypos)
{
    float xscale = 1;
    float yscale = 1;
    s_pThis->SetMousePos((int)xpos, (int)ypos);
    
    //glfwGetWindowContentScale(wnd, &xscale, &yscale);
    //auto* pSelf = static_cast<GLFWDemo*>(glfwGetWindowUserPointer(wnd));
   // pSelf->MouseEvent(float2(static_cast<float>(xpos * xscale), static_cast<float>(ypos * yscale)));

}

void Application::GLFW_MouseWheelCallback(GLFWwindow* wnd, double dx, double dy)
{
}
