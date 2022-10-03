#pragma once
#include <string>
#include <vector>

/// <summary>
/// The data namespace is for using app specific data within nodes.
/// </summary>

enum PropertyType {
	Int, Float, Double, String, Table
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
		void SetString(std::string v);
		int GetInt();
		float GetFloat();
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
		std::string mString;

	};

