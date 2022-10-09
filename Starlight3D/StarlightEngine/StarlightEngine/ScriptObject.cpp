#include "pch.h"
#include "ScriptObject.h"
#include <ZClassNode.h>

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