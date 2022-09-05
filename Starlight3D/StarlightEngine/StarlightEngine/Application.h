#pragma once


#include <memory>
#include <iomanip>
#include <iostream>
#include "UserInput.h"
#ifndef NOMINMAX
#    define NOMINMAX
#endif
#include <Windows.h>

#include <crtdbg.h>

#ifndef PLATFORM_WIN32
#    define PLATFORM_WIN32 1
#endif

#ifndef ENGINE_DLL
#    define ENGINE_DLL 1
#endif

#ifndef D3D11_SUPPORTED
#    define D3D11_SUPPORTED 1
#endif

#ifndef D3D12_SUPPORTED
#    define D3D12_SUPPORTED 1
#endif

#ifndef GL_SUPPORTED
#    define GL_SUPPORTED 1
#endif

#ifndef VULKAN_SUPPORTED
#    define VULKAN_SUPPORTED 1
#endif


#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"

#include "Common/interface/RefCntAutoPtr.hpp"

#include "Common/interface/BasicMath.hpp"
#if PLATFORM_WIN32
#    define GLFW_EXPOSE_NATIVE_WIN32 1
#endif
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

using namespace Diligent;


#define MAX_LOADSTRING 100




class Application
{
public:

	Application();
	bool Initialize(HWND hWnd);
    void InitEngine();
    void Init() {};
    void Update() {  };
    void Render();

    void CrWindow(const char* title, int width, int height, int hint);

    virtual void InitApp() {};
    virtual void UpdateApp() {};
    virtual void RenderApp() {};

    bool ProcessCommandLine(const char* CmdLine)
    {
        const char* mode = nullptr;

        const char* Keys[] = { "--mode ", "--mode=", "-m " };
        for (size_t i = 0; i < _countof(Keys); ++i)
        {
            const auto* Key = Keys[i];
            if ((mode = strstr(CmdLine, Key)) != nullptr)
            {
                mode += strlen(Key);
                break;
            }
        }

        if (mode != nullptr)
        {
            while (*mode == ' ')
                ++mode;

            if (_stricmp(mode, "D3D11") == 0)
            {
#if D3D11_SUPPORTED
                m_DeviceType = RENDER_DEVICE_TYPE_D3D11;
#else
                std::cerr << "Direct3D11 is not supported. Please select another device type";
                return false;
#endif
            }
            else if (_stricmp(mode, "D3D12") == 0)
            {
#if D3D12_SUPPORTED
                m_DeviceType = RENDER_DEVICE_TYPE_D3D12;
#else
                std::cerr << "Direct3D12 is not supported. Please select another device type";
                return false;
#endif
            }
            else if (_stricmp(mode, "GL") == 0)
            {
#if GL_SUPPORTED
                m_DeviceType = RENDER_DEVICE_TYPE_GL;
#else
                std::cerr << "OpenGL is not supported. Please select another device type";
                return false;
#endif
            }
            else if (_stricmp(mode, "VK") == 0)
            {
#if VULKAN_SUPPORTED
                m_DeviceType = RENDER_DEVICE_TYPE_VULKAN;
#else
                std::cerr << "Vulkan is not supported. Please select another device type";
                return false;
#endif
            }
            else
            {
                std::cerr << mode << " is not a valid device type. Only the following types are supported: D3D11, D3D12, GL, VK";
                return false;
            }
        }
        else
        {
#if D3D12_SUPPORTED
            //m_DeviceType = RENDER_DEVICE_TYPE_D3D12;
#elif VULKAN_SUPPORTED
            m_DeviceType = RENDER_DEVICE_TYPE_VULKAN;
#elif D3D11_SUPPORTED
            m_DeviceType = RENDER_DEVICE_TYPE_D3D11;
#elif GL_SUPPORTED
            m_DeviceType = RENDER_DEVICE_TYPE_GL;
#endif
        }
        return true;
    }

    void CreateResources();
    void Present();
    RENDER_DEVICE_TYPE GetDeviceType() const { return m_DeviceType; }
    void WindowResize(Uint32 width, Uint32 height)
    {
 
        if (m_pSwapChain)
            m_pSwapChain->Resize(width, height);
        winWidth = width;
        winHeight = height;
    
    }

    RefCntAutoPtr<ISwapChain> GetSwap() {

        return m_pSwapChain;

    }
    
    RefCntAutoPtr<IEngineFactory> GetFactory() {

        return m_pEngFac;

    }

    RefCntAutoPtr<IRenderDevice> GetDevice() {

        return m_pDevice;

    }
    RefCntAutoPtr<IDeviceContext> GetContext() {

        return m_pImmediateContext;

    }

    void SetSize(int w, int h) {
        winWidth = w;
        winHeight = h;
    }

    int GetWidth() {
        return winWidth;
    }

    int GetHeight() {
        return winHeight;
    }

    static RefCntAutoPtr<IRenderDevice> GetDev() {
        return s_pThis->GetDevice();
    }
    static Application *s_pThis;
    void Run();

    void SetMousePos(int x, int y) {

        mMouseX = x;
        mMouseY = y;
    }

    int GetMouseX() {
        return mMouseX;
    }

    int GetMouseY() {
        return mMouseY;
    }
    UserInput* GetInput() {
        return mInput;
    }

    static Application* GetApp() {
        return s_pThis;
    }

private:

    UserInput* mInput;

    RefCntAutoPtr<IRenderDevice>  m_pDevice;
    RefCntAutoPtr<IDeviceContext> m_pImmediateContext;
    RefCntAutoPtr<ISwapChain>     m_pSwapChain;
    RefCntAutoPtr<IPipelineState> m_pPSO;
    RefCntAutoPtr<IEngineFactory> m_pEngFac;
    RENDER_DEVICE_TYPE            m_DeviceType = RENDER_DEVICE_TYPE_D3D12;
    int winWidth, winHeight;
    GLFWwindow* m_Window = nullptr;

    int mMouseX, mMouseY;
    int mDeltaX, mDeltaY;
    bool mMouseFirst = true;
    

    static void GLFW_ResizeCallback(GLFWwindow* wnd, int w, int h);
    static void GLFW_KeyCallback(GLFWwindow* wnd, int key, int, int state, int);
    static void GLFW_MouseButtonCallback(GLFWwindow* wnd, int button, int state, int);
    static void GLFW_CursorPosCallback(GLFWwindow* wnd, double xpos, double ypos);
    static void GLFW_MouseWheelCallback(GLFWwindow* wnd, double dx, double dy);
};

