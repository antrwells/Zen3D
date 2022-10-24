#pragma once
#include "Common/interface/BasicMath.hpp"
using namespace Diligent;
class UIRect
{
public:

	int x, y, w, h;

	UIRect(int rx, int ry, int rw, int rh);
	UIRect();
	bool InBounds(float2 pos) {

		if (pos.x > x && pos.x < x + w) {
			if (pos.y > y && pos.y < y + h) {
				return true;
			}
		}
		return false;

	}
};

