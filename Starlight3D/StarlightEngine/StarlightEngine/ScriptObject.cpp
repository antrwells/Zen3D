#include "pch.h"
#include "ScriptObject.h"
#include <ZClassNode.h>
#include "ZContextVar.h"

void ScriptObject::CallInit()
{

	mMainClass->CallMethod("Init", {});

}

void ScriptObject::CallUpdate() {

	mMainClass->CallMethod("Update", {});

}

void ScriptObject::CallUI() {

	mMainClass->CallMethod("UI", {});

}

void ScriptObject::CallRender() {

	mMainClass->CallMethod("Render", {});

}


std::vector<ZContextVar*> ScriptObject::GetVars() {

	return mMainClass->GetVars();


}

void ScriptObject::PushVars() {

	auto vars = mMainClass->GetVars();

	for (int i = 0; i < vars.size(); i++)
	{

		auto v = vars[i];
		
		v->Push();

		

	}



}

void ScriptObject::PopVars() {

	auto vars = mMainClass->GetVars();

	for (int i = 0; i < vars.size(); i++)
	{

		auto v = vars[i];

		v->Pop();



	}


}