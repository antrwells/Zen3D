#pragma once
class UIColor
{
public:

	float r, g, b, a;
	UIColor() {
		r = g = b = 1.0;
		a = 1.0;
	}

	UIColor(float cr, float cg, float cb, float ca = 1.0)
	{
		r = cr;
		g = cg;
		b = cb;
		a = ca;

	}

	void Mix(UIColor right)
	{
		r = right.r * r;
		g = right.g * g;
		b = right.b * b;
		a = right.a * a;

	}

	void Lerp(UIColor right,float i) {

		r = r + (right.r - r) * i;
		g = g + (right.g - g) * i;
		b = b + (right.b - b) * i;
		a = a + (right.a - a) * i;

	}

};

