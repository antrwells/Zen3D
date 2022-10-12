#pragma once
#include "PostProcess.h"

class PPBloom : public PostProcess
{
public:

	PPBloom();
	void PP();
	void Render();
	void Set(float limit, float blur);
	float GetLimit() { return mLimit; }
	float GetBlur() { return mBlur; }
private:
	float mLimit = 0.65f;
	float mBlur = 0.0032f;
};

