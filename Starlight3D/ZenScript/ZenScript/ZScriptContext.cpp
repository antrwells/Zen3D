#include "ZScriptContext.h"
#include "ZContextScope.h"
#include <string>
#include "ZClassNode.h"
#include <assert.h>
#include "ZSystemFunctions.h"
#include "ZSystemFunction.h"
#include "VarTypes.h"
#include "ZTokenizer.h"
#include "ZParseCodeBody.h"
#include "ZCodeBodyNode.h"
#include "DirCollection.h"
#include "ZParser.h"
#include <time.h>
#include <ctime>

//#include "ZContextVar.h"


ZContextVar* sysfunc_millisecs(const std::vector<ZContextVar*>& args)
{

	auto res = new ZContextVar("ms:", VarType::VarInt,"int",false);
	res->SetInt(clock());
	return res;


}

ZContextVar* sysfunc_printf(const std::vector<ZContextVar*>& args)
{

	for (auto&& arg : args)
	{
		switch (arg->GetType())
		{
		case VarType::VarInt:
			printf("%d\n",arg->GetIntVal());
			break;
		case VarType::VarFloat:
			printf("%f\n", arg->GetFloatVal());
			break;
		case VarType::VarString:
			printf(arg->GetStringVal().c_str());
			printf("\n");
			break;
		case VarType::VarInstance:

		{
			if (arg->GetBaseID() == "Vec3")
			{

				auto cv = arg->GetClassVal();

				float vx, vy, vz;
				vx = cv->FindVar("x")->GetFloatVal();
				vy = cv->FindVar("y")->GetFloatVal();
				vz = cv->FindVar("z")->GetFloatVal();

				printf("X:%f Y:%f Z:%f", vx, vy, vz);
				printf("\n");

			}

			int aa = 5;
		}

			break;
		default:
			assert(false);
			break;
		}
	}
	return nullptr;
}

ZScriptContext::ZScriptContext() {

	SetupSystem();
	CurrentContext = this;
	

}



ZSystemFunctions* ZScriptContext::GetSysFuncs() {
	
		return mSysFuncs;
	
}

void ZScriptContext::SetupSystem() {

	if (mSysInit) return;

	ZSystemFunction printf("printf", sysfunc_printf);

	mSysFuncs = new ZSystemFunctions;
	
	ZSystemFunction millisecs("millisecs", sysfunc_millisecs);
	mSysFuncs->RegisterFunction(printf);
	mSysFuncs->RegisterFunction(millisecs);

	mSysInit = true;

}


void ZScriptContext::AddNode(ZMainNode* node) {

	auto classes = node->GetClasses();

	for (int i = 0; i < classes.size(); i++) {

		mClasses.push_back(classes[i]);

	}

	auto static_classes = node->GetStaticClasses();

	for (int i = 0; i < static_classes.size(); i++) {

		auto sint = static_classes[i]->CreateInstance(static_classes[i]->GetName(), {});

		mStaticClasses.push_back(sint);

	}

}

ZClassNode* ZScriptContext::FindClass(std::string name) {

	for (int i = 0; i < mClasses.size(); i++) {
		if (mClasses[i]->GetName() == name)
		{
			return mClasses[i];
		}
	}

	return nullptr;
}

ZClassNode* ZScriptContext::CreateInstance(std::string name,std::string instance_name, const std::vector<ZContextVar*>& params) {

	for (int i = 0; i < mClasses.size(); i++) {

		auto cls = mClasses[i];

		if (cls->GetName() == name) {

			auto cl =  cls->CreateInstance(name,params);
			cls->Bind();
			//mInstances.push_back(cl);
			return cl;
			return nullptr;

		}

	}
	//*ERROR*
//	assert(false);
	return nullptr;

}

void ZScriptContext::PushClass(ZClassNode* node)
{

	mClassStack.push(node);

}

void ZScriptContext::PopClass() {

	mClassStack.pop();

}

ZClassNode* ZScriptContext::GetClass() {


	return mClassStack.top();

}


ZScriptContext* ZScriptContext::CurrentContext = nullptr;
ZContextScope* ZScriptContext::CurrentScope = nullptr;

ZClassNode* ZScriptContext::FindInstance(std::string name) {

	for (int i = 0; i < mInstances.size(); i++) {

		if (mInstances[i]->GetName() == name)
		{
			return mInstances[i];
		}

	}

}

ZContextVar* ZScriptContext::CallMethod(std::string name, std::string meth, const std::vector<ZContextVar*>& params)
{

	auto cls = FindInstance(name);
	
	PushScope(cls->GetScope());
	
	cls->CallMethod(meth, params);

	PopScope();

	int aa = 5;

	return nullptr;
}

ZContextVar* ZScriptContext::RunLine(std::string name) {

	auto src = new ZSource;
	src->AddLine(name);
	
	auto toker = new ZTokenizer(src);
	auto stream = toker->Tokenize();
	
	auto parse_Code = new ZParseCodeBody(stream);
	auto code_node = (ZCodeBodyNode*)parse_Code->Parse();
	
	return code_node->Exec(std::vector<ZContextVar*>());

	

}

void ZScriptContext::LoadLib(std::string name)
{

	DirCollection* dir = new DirCollection("script/lib/" + name + "/");

	for (int i = 0; i < dir->enteries.size(); i++)
	{

		if (!dir->enteries[i].folder) {
			auto entry = dir->enteries[i].full;
			
			ZSource* source = new ZSource(entry);
			ZTokenizer* toker = new ZTokenizer(source);
			auto stream = toker->Tokenize();
			ZParser* parser = new ZParser(stream);
			ZMainNode* main1 = parser->Parse();

			this->AddNode(main1);

		}

	}


}


bool ZScriptContext::IsStaticClass(size_t name) {

	for (int i = 0; i < mStaticClasses.size(); i++)
	{

		if (mStaticClasses[i]->GetHashName() == name)
		{
			return true;
		}

	}
	return false;
}
bool ZScriptContext::IsStaticClass(std::string name) {

	for (int i = 0; i < mStaticClasses.size(); i++)
	{

		if (mStaticClasses[i]->GetName() == name)
		{
			return true;
		}

	}
	return false;
}


ZClassNode* ZScriptContext::GetStaticClass(size_t name) {

	for (int i = 0; i < mStaticClasses.size(); i++)
	{

		if (mStaticClasses[i]->GetHashName() == name)
		{
			return mStaticClasses[i];
		}

	}
	return nullptr;

}


ZClassNode* ZScriptContext::GetStaticClass(std::string name) {

	for (int i = 0; i < mStaticClasses.size(); i++)
	{

		if (mStaticClasses[i]->GetName() == name)
		{
			return mStaticClasses[i];
		}

	}
	return nullptr;

}

bool ZScriptContext::mSysInit = false;