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

ZContextVar* ZSystemFunctions::CallFunction(std::string name, const std::vector<ZContextVar*>& params)
{
	for (int i = 0; i < mFunctions.size(); i++) {

		auto func = mFunctions[i];

		if (func.GetName() == name) {

			return func.CallFunction(params);


		}

	}
	assert(false);
	return nullptr;

}