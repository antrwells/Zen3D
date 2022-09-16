#pragma once
#include "Application.h"
#include "SmartDraw.h"
#include "UI.h"
#include "kFont.h"
#include "Importer.h"
#include "SceneGraph.h"
#include "RenderTarget2D.h"
#include "RenderTargetCube.h"
#include "CubeRenderer.h"
#include "GBuffer.h"
#include "SceneRenderer.h"
#include "SceneRayTracer.h"
#include "PostProcessing.h"

class AppZen : public Application
{
public:

    AppZen();
    void InitApp();
    void UpdateApp();
    void RenderApp();

private:
};

