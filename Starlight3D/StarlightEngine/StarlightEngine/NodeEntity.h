#pragma once
#include "Node3D.h"
#include "Mesh3D.h"
// 
//#include "SmartMesh.h"
#include "PhysicsBody.h"
#include "PhysicsBody.h"
#include "PBBox.h"
#include "PBSphere.h"
#include "PBConvex.h"
#include "PBCapsule.h"


    /// <summary>
    /// A NodeEntity is a 3D node, that contains a single or multiple meshes, binded to the node, and rendered
    /// at the nodes position and rotation, with it's scale.
    /// </summary>
    class NodeEntity :
        public Node3D
    {
    public:

        /// <summary>
        /// The base constructor will create an empty entity with no meshes attached.
        /// </summary>
        NodeEntity();


        Node3D* Clone();

        /// <summary>
        /// Adds a mesh to the entity, whenever rendered, this mesh will be rendered also.
        /// </summary>
        /// <param name="mesh"></param>
        void AddMesh(Mesh3D* mesh);

        /// <summary>
        /// Returns a list of this node's meshes. These are the actual meshes, not a copy or reference. 
        /// </summary>
        /// <returns></returns>
        std::vector<Mesh3D*> GetMeshes();

        void RenderDepth();
        /// <summary>
        /// Renders the entity
        /// </summary>
        void Render();
        /// <summary>
        /// Internal use.
        /// </summary>
        void RenderSmartMesh();
        
        //This converts a whole entity into a single draw call, using bindless textures. max of 1024 unique textures per  smart mesh.
        void ConvertToSmartMesh();


        void SetChanged() {
            for (int i = 0; i < mMeshes.size(); i++) {
                mMeshes[i]->SetChanged();
            }
        }
        /// <summary>
        /// Returns the given mesh of the entity.
        /// </summary>
        /// <param name="i"></param>
        /// <returns></returns>
        Mesh3D* GetMesh(int i);
        /// <summary>
        /// Sets the physics collision to a box, wrapped around the entity.
        /// </summary>
        void SetPhysicsBox();
        /// <summary>
        /// Sets the physics collision to a sphere, wrapped around the entity.
        /// </summary>
        void SetPhysicsSphere();
        /// <summary>
        /// Sets the physics collision to a automatically generated convex hull
        /// </summary>
        void SetPhysicsConvex();
        /// <summary>
        /// sets the physics collision to a capsule.
        /// </summary>
        void SetPhysicsCapsule();
        /// <summary>
        /// returns the smallest possible bounding box around the mesh.
        /// </summary>
        /// <returns></returns>
        float3 GetBounds();
        /// <summary>
        /// Constrain physics on any given dimension.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="z"></param>
        void SetConstraint(bool x, bool y, bool z);
        /// <summary>
        /// Internal use.
        /// </summary>
        void UpdatePhysics();
        /// <summary>
        /// Sets the position of a node.
        /// </summary>
        /// <param name="position"></param>
        void SetPosition(float3 position);
        /// <summary>
        /// Sets the euler rotation of the node.
        /// </summary>
        /// <param name="pitch"></param>
        /// <param name="yaw"></param>
        /// <param name="roll"></param>
        void SetRotation(float pitch, float yaw, float roll);
        /// <summary>
        /// Sets the physics collision to a per tri map of the mesh. This is useful for level collisions, as the tri mesh varient can not move.
        /// </summary>
        void SetPhysicsTris();
    
        
        PhysicsBody* GetBody() {
            return nullptr;
        }

        void ReadNode(VFile* file, bool read_type)
        {
            
            if (read_type) {
                mType = (NodeType)file->ReadInt();
            }
            mName = file->ReadString();
            ReadTransform(file);

            int meshes = file->ReadInt();

            for (int i = 0; i < meshes; i++) {

                Mesh3D* new_mesh = new Mesh3D();
                new_mesh->ReadMesh(file);
                mMeshes.push_back(new_mesh);
                new_mesh->SetOwner(this);
            }

            ReadScripts(file);

            int cc = file->ReadInt();
            for (int i = 0; i < cc; i++) {

           //     NodeType mt = (NodeType)file->ReadInt();
                Node3D* new_node = (Node3D*)new NodeEntity;
                new_node->ReadNode(file, true);
                AddNode(new_node);
            }
        }

        void WriteNode(VFile* file) {


            file->WriteInt((int)mType);
            file->WriteString(mName);
            WriteTransform(file);

            file->WriteInt(mMeshes.size());
            for (int i = 0; i < mMeshes.size(); i++) {

                mMeshes[i]->WriteMesh(file);

            }
            WriteScripts(file);

            file->WriteInt(mChildren.size());
            for (int i = 0; i < mChildren.size(); i++) {

                mChildren[i]->WriteNode(file);

            }

        }

    protected:


        /// <summary>
        /// A list containing all of this NodeEntity's attached meshes.
        /// </summary>
        std::vector<Mesh3D*> mMeshes;
       // SmartMesh* mSmartMesh;
        
       // static Kinetic::FX::Effect* FXSmartMesh;

      

    };


