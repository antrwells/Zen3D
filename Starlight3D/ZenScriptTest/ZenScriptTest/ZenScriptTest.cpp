// ZenScriptTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ZTokenizer.h"
#include "ZParser.h"
#include "ZScriptContext.h"
#include "VarTypes.h"
#include "ZClassNode.h"
#include "ZSystemFunctions.h"
#include "ZSystemFunction.h"
struct t1 {
    int vv = 5;
    int bb = 10;
};


ZContextVar* sysfunc_getc(const std::vector<ZContextVar*>& args)
{

    t1* n1 = new t1;
    n1->bb = 22;
    n1->vv = 44;

    ZContextVar* rv = new ZContextVar("", VarType::VarCObj);
    rv->SetCObj(n1);

    return rv;
}
ZContextVar* sysfunc_test(const std::vector<ZContextVar*>& args)
{

    t1* obj = (t1*)args[0]->GetCObj();
   
    int aa = 5;

    return nullptr;


}

int main()
{
    std::cout << "Hello World!\n";
    ZScriptContext* context1 = new ZScriptContext;
//    context1->LoadLib("math");

    ZSource* source = new ZSource("script/test.zs");
   ZTokenizer* toker = new ZTokenizer(source);
   auto stream = toker->Tokenize();
   ZParser* parser = new ZParser(stream);
   ZMainNode* main1 = parser->Parse();
   ZSystemFunctions* tmp = context1->GetSysFuncs();

   ZSystemFunction test("test", sysfunc_test);
   ZSystemFunction getc("getc", sysfunc_getc);
   tmp->RegisterFunction(test);
   tmp->RegisterFunction(getc);
  

   context1->AddNode(main1);



   std::vector<ZContextVar*> pars = {new ZContextVar("v1",VarType::VarCObj)};

   t1 check;
   check.bb = 5;
   check.vv = 25;

   pars[0]->SetCObj(&check);

   auto cls_inst = context1->CreateInstance("testClass", "test",pars);

   cls_inst->CallMethod("test", {});
    
   //int aa = 5;
   auto par_a = new ZContextVar("a", VarType::VarInt);
   auto par_b = new ZContextVar("b", VarType::VarInt);
   par_a->SetInt(67);
   par_b->SetInt(10);

  

   //for (int i = 0; i < 10; i++) {                                                                                                              
 //      cls_inst->CallMethod("check", { par_a});d
   //}
   int b = 5;

   //context1->RunLine("printf(\"Hey it's working!\",20+30+5)");

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
