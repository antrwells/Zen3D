#pragma once
#include "Application.h"
#include "ProjectsUI.h"
class AppProjects :
    public Application
{
public:

    AppProjects();
    void InitApp();
    void UpdateApp();
    void RenderApp();
    void SetPayload();

private:
    ProjectsUI* mUI;
};

