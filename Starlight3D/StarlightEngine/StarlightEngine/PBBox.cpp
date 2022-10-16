#include "pch.h"
#include "PBBox.h"


	PBBox::PBBox(float bw, float bh, float bd) {

		w = bw / 2;
		h = bh / 2;
		d = bd / 2;
		InitBody();

	}

	void PBBox::InitBody() {

		

		//body = physx::PxCreateDynamic(Physics::GetSDK(), tm, geo, mat, 10.0f);

	}
