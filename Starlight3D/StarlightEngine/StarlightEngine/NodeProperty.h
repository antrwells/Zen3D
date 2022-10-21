#pragma once
#include <string>
#include <vector>
#include "Common/interface/BasicMath.hpp"

using namespace Diligent;
/// <summary>
/// The data namespace is for using app specific data within nodes.
/// </summary>

enum PropertyType {
	Int, Float, Double, String, Table,Float2,Float3,Bool
};
	/// <summary>
	/// NodeProperty is a named piece of data for a node/component.
	/// </summary>
	class NodeProperty
	{
	public:

		/// <summary>
		/// Creates the node property, with the given name.
		/// </summary>
		/// <param name="name">the name of the property. this is case-sensitive.</param>
		NodeProperty(std::string name);
		/// <summary>
		/// Sets the arbitary date of the node.
		/// </summary>
		/// <param name="data"></param>
		//void SetData(void* data);
		void SetInt(int v);
		void SetFloat(float v);
		void SetFloat2(float2 v);
		void SetFloat3(float3 v);
		void SetBool(bool v);
		bool GetBool();
		void SetString(std::string v);
		int GetInt();
		float GetFloat();
		float2 GetFloat2();
		float3 GetFloat3();
		std::string GetString();
		PropertyType GetType();
		void SetType(PropertyType type);
		/// <summary>
		/// gets the data asscoiated with the property.
		/// </summary>
		/// <returns></returns>
		void* GetData();
		/// <summary>
		/// Returns the name of the property. case-sensitive.
		/// </summary>
		/// <returns></returns>
		std::string GetName();

	private:

		std::string mName;
		//void* mData; 
		PropertyType mType;

		int mInt;
		float mFloat;
		double mDouble;
		float2 mFloat2;
		float3 mFloat3;
		bool mBool;
		std::string mString;

	};

