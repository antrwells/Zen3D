#include "pch.h"
#include "PBTerrain.h"

PBTerrain::PBTerrain(std::vector<float> height, float width, float depth) {

	mHeights = height;
	mWidth = width;
	mHeight = depth;
	InitBody();

}


void PBTerrain::InitBody() {



}