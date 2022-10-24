#include "pch.h"
#include "UIRect.h"


UIRect::UIRect(int rx, int ry, int rw, int rh) {

	x = rx;
	y = ry;
	w = rw;
	h = rh;

}

UIRect::UIRect() {

	x = y = w = h = 0;

}