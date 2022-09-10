#pragma once
#include "PhysicsBody.h"
#include "Mesh3D.h"
#include <vector>


    class NodeEntity;

    class PBTriangles :
        public PhysicsBody
    {
    public:
        //PBTriangles(WorldEntity* entity);
        PBTriangles();
        PBTriangles(std::vector<Mesh3D*> meshes, int index);
    private:
        physx::PxTriangleMesh* triMesh = NULL;
    };


