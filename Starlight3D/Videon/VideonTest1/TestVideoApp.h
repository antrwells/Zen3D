#pragma once
#include "Application.h"

class VideoDecoder;
class SmartDraw;
class Texture2D;

class TestVideoApp : public Application
{
public:

	void InitApp();
	void UpdateApp();
	void RenderApp();

private:

	VideoDecoder* mDecoder;
	SmartDraw* mDraw;
	Texture2D* mTex1;

};

