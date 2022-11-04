#pragma once
#include "Common/interface/RefCntAutoPtr.hpp"

#include "Common/interface/BasicMath.hpp"


using namespace Diligent;


inline Quaternion fromMatrix(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
	// Use the Graphics Gems code, from 
	// ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z
	float x, y, z, w;
	float t = m00 + m11 + m22;
	// we protect the division by s by ensuring that s>=1
	if (t >= 0) { // by w
		float s = sqrt(t + 1);
		w = 0.5f * s;
		s = 0.5f / s;
		x = (m21 - m12) * s;
		y = (m02 - m20) * s;
		z = (m10 - m01) * s;
	}
	else if ((m00 > m11) && (m00 > m22)) { // by x
		float s = sqrt(1 + m00 - m11 - m22);
		x = s * 0.5f;
		s = 0.5f / s;
		y = (m10 + m01) * s;
		z = (m02 + m20) * s;
		w = (m21 - m12) * s;
	}
	else if (m11 > m22) { // by y
		float s = sqrt(1 + m11 - m00 - m22);
		y = s * 0.5f;
		s = 0.5f / s;
		x = (m10 + m01) * s;
		z = (m21 + m12) * s;
		w = (m02 - m20) * s;
	}
	else { // by z
		float s = sqrt(1 + m22 - m00 - m11);
		z = s * 0.5f;
		s = 0.5f / s;
		x = (m02 + m20) * s;
		y = (m21 + m12) * s;
		w = (m10 - m01) * s;
	}
	Quaternion q(x, y, z, w);

	return q;
}


class Helper
{
public:

	static float3 Lerp(float3 src, float3 dst, float am);
	static Quaternion MatrixToQuat(float3x3 mat)
	{

		return fromMatrix(mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2]);


	}

};

