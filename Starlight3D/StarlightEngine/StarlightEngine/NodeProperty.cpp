#include "pch.h"
#include "NodeProperty.h"

namespace Kinetic::Graph::Data {

	NodeProperty::NodeProperty(std::string name) {

		mName = name;

	}

	void NodeProperty::SetData(void* data) {

		mData = data;

	}

	void* NodeProperty::GetData() {

		return mData;

	}

	std::string NodeProperty::GetName() {

		return mName;

	}

}