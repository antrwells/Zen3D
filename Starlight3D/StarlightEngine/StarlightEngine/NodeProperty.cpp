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
		mType = PropertyType::Int;

	}


	void NodeProperty::SetBool(bool v)
	{
		mBool = v;
	}

	bool NodeProperty::GetBool() {
		return mBool;
	}

	void NodeProperty::SetFloat(float v) {
		mFloat = v;
		mType = PropertyType::Float;
	}
	void NodeProperty::SetFloat2(float2 v)
	{
		mFloat2 = v;
		mType = PropertyType::Float2;

	}

	void NodeProperty::SetFloat3(float3 v) {
		mFloat3 = v;
		mType = PropertyType::Float3;
	}

	float2 NodeProperty::GetFloat2() {
		return mFloat2;
	}

	float3 NodeProperty::GetFloat3() {
		return mFloat3;
	}

	void NodeProperty::SetString(std::string v) {
		mString = v;
		mType = PropertyType::String;
	}