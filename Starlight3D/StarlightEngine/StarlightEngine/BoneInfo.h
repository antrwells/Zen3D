#pragma once
#include "Common/interface/BasicMath.hpp"
#include "VFile.h"


using namespace Diligent;

	class BoneInfo
	{
	public:
		int id = 0;
		float4x4 offset;
		void Write(VFile* file) {
			file->WriteInt(id);
			file->WriteMatrix(offset);
		}
		void Read(VFile* file) {
			id = file->ReadInt();
			offset = file->ReadMatrix();
		}
	};

