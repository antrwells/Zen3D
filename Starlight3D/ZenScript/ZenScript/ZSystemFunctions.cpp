#include "ZSystemFunctions.h"
#include "ZSystemFunction.h"
#include "ZContextVar.h"
#include <assert.h>


ZSystemFunctions* ZSystemFunctions::sFuncs = nullptr;

ZSystemFunctions::ZSystemFunctions() {

	sFuncs = this;
	mFunctions.resize(0);

}

void ZSystemFunctions::RegisterFunction(ZSystemFunction function) {

	mFunctions.push_back(function);

}

ZSystemFunction ZSystemFunctions::GetFunction(size_t name) {

	for (int i = 0; i < mFunctions.size(); i++) {

		auto func = mFunctions[i];

		if (func.GetHashName() == name) {

			return func;// func.CallFunction(params);


		}

	}
	
}

ZContextVar* ZSystemFunctions::CallFunction(size_t name, const std::vector<ZContextVar*>& params)
{
	for (int i = 0; i < mFunctions.size(); i++) {

		auto func = mFunctions[i];

		if (func.GetHashName() == name) {

			return func.CallFunction(params);


		}

	}
	assert(false);
	return nullptr;

}