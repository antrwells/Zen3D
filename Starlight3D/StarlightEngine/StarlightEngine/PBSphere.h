#pragma once
#include "PhysicsBody.h"

	class PBSphere : public PhysicsBody
	{
	public:
		PBSphere(float ssize);
		void InitBody();

	private:
		float size = 0.0f;
	};
