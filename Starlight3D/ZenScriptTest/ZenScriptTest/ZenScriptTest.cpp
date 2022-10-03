// ZenScriptTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZTokenizer.h"
#include "ZParser.h"
#include "ZScriptContext.h"
#include "VarTypes.h"
#include "ZClassNode.h"

int main()
{
    std::cout << "Hello World!\n";

    ZSource* source = new ZSource("script/test.zs");
   ZTokenizer* toker = new ZTokenizer(source);
   auto stream = toker->Tokenize();
   ZParser* parser = new ZParser(stream);
   ZMainNode* main1 = parser->Parse();

   ZScriptContext* context1 = new ZScriptContext;

   context1->AddNode(main1);

   auto cls_inst = context1->CreateInstance("testClass","test");

 
   //int aa = 5;
   auto par_a = new ZContextVar("a", VarType::VarInt);
   auto par_b = new ZContextVar("b", VarType::VarInt);
   par_a->SetInt(67);
   par_b->SetInt(10);

   for (int i = 0; i < 10; i++) {                                                                                                              
       cls_inst->CallMethod("check", { par_a});
   }
   int b = 5;
 //  context1->CallMethod("test", "testMethod",{par_a,par_b});
   


   //context1->PushClass(cls_inst);e

   //ZContextVar* res = cls_inst->CallMethod("testMethod", { par_a,par_b });q

   //context1->PopClass();


   int aa = 5;

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
