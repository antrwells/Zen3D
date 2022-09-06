#pragma once
#include "Node3D.h"
#include "Mesh3D.h"


    /// <summary>
    /// A billboard is a quad with a texture map that can be facing the camera or elsewhere.
    /// Useful for things like say a starry background.
    /// </summary>
    class NodeBillboard :
        public Node3D
    {
    public:

        /// <summary>
        /// creates an empty billboard.
        /// </summary>
        NodeBillboard()
        {
            mMesh = nullptr;
        };
        /// <summary>
        /// Turns a texture map into a billboard, that you can add and render to a scene graph.
        /// </summary>
        /// <param name="map"></param>
        /// <param name="size"></param>
        /// <param name="useZ"></param>
        NodeBillboard(Texture2D* map, float size,bool useZ=false);
        /// <summary>
         /// Internal use.
         /// </summary>
        void Render();
        /// <summary>
        /// sets the color of the billboard. This color is modulated with the texture map.
        /// </summary>
        /// <param name="col"></param>
        void SetColor(float3 col);
        void SetColors(float4 col);
        /// <summary>
        /// Internal use.
        /// </summary>
        void SetMesh(Mesh3D* mesh)
        {
            mMesh = mesh;
        }

        /// <summary>
        /// Clones the billboard, including mesh/texture data.
        /// </summary>
        /// <returns></returns>
        Node3D* Clone() {

            NodeBillboard* clone = new NodeBillboard();
            clone->SetMesh(mMesh);
            return clone;

        }

    private:

        Mesh3D* mMesh = nullptr;


    };

