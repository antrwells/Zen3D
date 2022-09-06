#include "pch.h"
#include "Helper.h"


float3 Helper::Lerp(float3 src, float3 dst, float am) {

	float3 v = src + (dst - src) * am;
	return v;
}