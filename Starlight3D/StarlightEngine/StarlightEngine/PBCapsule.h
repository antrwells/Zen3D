#pragma once
#include "PhysicsBody.h"

    class PBCapsule :
        public PhysicsBody
    {
    public:
        PBCapsule(float height, float radius);
        void InitBody();
    private:
        float Height, Radius;
    };
