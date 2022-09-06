#pragma once
#include "Node3D.h"
#include <vector>

class NoRenderList
{
public:
	static void AddNoRender(Node3D* node) {

		mNoRender.push_back(node);

	}

	static void ResetNoRender() {

		mNoRender.clear();

	}

	static std::vector<Node3D*> mNoRender;
	
};

