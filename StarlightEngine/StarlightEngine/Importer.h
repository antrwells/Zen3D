#pragma once
#include "NodeEntity.h"


/// <summary>
/// The import namespace covers the engine's functionality to import 3D models. 
/// This includes a general purpose AssImp importer that can import many different types of Model;
/// </summary>

	/// <summary>
	/// The importer class can import static and animated entities.
	/// </summary>
	class Importer
	{
	public:

		NodeEntity* ImportAI(const char* path, bool optimize = true);
		//Kinetic::Graph::Nodes::NodeActor* ImportActor(const char* path);


	private:


	};



