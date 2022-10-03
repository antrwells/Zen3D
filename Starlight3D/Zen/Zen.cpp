// Zen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AppZen.h"
#include "sol.hpp"

int appW, appH;

void init_app(int w, int h) {

    appW = w;
    appH = h;
    printf("Starting application. Resolution W:%d H:%d\n", w, h);

}

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

    sol::state state;
    state.open_libraries(sol::lib::base, sol::lib::package);

    state["init_app"] = init_app;

    auto init = state.load_file("script/init.l");

    init();

    auto node_1 = state.load_file("script/node1.l");

    state["node"] = &n;
    state["testFunc"] = testFunc;

    node_1();

    std::cout << "Hello Zen!\n";

    AppZen* zen = new AppZen;
    int APIHint = GLFW_NO_API;
    zen->CrWindow("Zen", appW,appH, APIHint);
    zen->InitEngine();
    zen->CreateResources();
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
