#pragma once
#include "Common/interface/BasicMath.hpp"

using namespace Diligent;

class VFile;

class TrackKeyFrame
{
public:

	TrackKeyFrame();
	void SetTime(float time);
	float GetTime();
	void SetPosition(float3 position);
	void SetScale(float3 scale);
	void SetRotation(float3x3 rotation);
	TrackKeyFrame* GetCurrentKey(TrackKeyFrame* right,float time,bool zeroToOne);
	float3 GetPosition();
	float3 GetScale();
	float3x3 GetRotation();
	void Write(VFile* file);
	void Read(VFile* file);
private:

	float mTime;
	float3 mPosition;
	float3 mScale;
	float3x3 mRotation;
	

};

