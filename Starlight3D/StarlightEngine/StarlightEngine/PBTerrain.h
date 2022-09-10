#pragma once
#include "PhysicsBody.h"
#include <vector>
class PBTerrain :
    public PhysicsBody
{
public:
	PBTerrain(std::vector<float> highfield,float width,float height);
	void InitBody();

private:
	std::vector<float> mHeights;
	float mWidth, mHeight;

};

