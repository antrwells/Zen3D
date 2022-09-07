#pragma once
#include <string>
#include <vector>

/// <summary>
/// The data namespace is for using app specific data within nodes.
/// </summary>

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
		void SetData(void* data);
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
		void* mData;

	};

