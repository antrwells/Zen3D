#pragma once
#include "PhysicsBody.h"
#include "Mesh3D.h"


    class PBConvex :
        public PhysicsBody
    {
    public:

        PBConvex(Mesh3D* mesh);
        void InitBody();

    private:

        Mesh3D* mesh;

        //    NodeEntity* entity;

    };
