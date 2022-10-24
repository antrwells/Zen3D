// Zen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AppZen.h"
#include "VFile.h"


int appW, appH;


struct testObj {
    int x = 5;
};

void testFunc(testObj* node) {

    printf("testFunc:%d\n", node->x);

}

int main()
{

    testObj n;
    n.x = 25;

    appW = 1424;
    appH = 860;
    std::cout << "Hello Zen!\n";

    VFile* load_proj = new VFile("project.load", FileMode::Read);

    std::string proj_path = std::string(load_proj->ReadString());

    load_proj->Close();

    AppZen* zen = new AppZen;
    int APIHint = GLFW_NO_API;
    zen->CrWindow("Zen", 1400,800, APIHint);
    zen->InitEngine();
    zen->CreateResources();
    zen->SetProjectPath(proj_path);
    zen->Run();



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
