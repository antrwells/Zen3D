#include "pch.h"
#include "NodeProperty.h"


	NodeProperty::NodeProperty(std::string name) {

		mName = name;

	}

	


	std::string NodeProperty::GetName() {

		return mName;

	}

	PropertyType NodeProperty::GetType() {

		return mType;

	}

	int NodeProperty::GetInt() {
		return mInt;
	}

	float NodeProperty::GetFloat() {

		return mFloat;

	}

	std::string NodeProperty::GetString() {

		return mString;

	}

	void NodeProperty::SetType(PropertyType type) {
		mType = type;
	}

	void NodeProperty::SetInt(int v) {

		mInt = v;

	}

	void NodeProperty::SetFloat(float v) {
		mFloat = v;
	}
	void NodeProperty::SetString(std::string v) {
		mString = v;
	}