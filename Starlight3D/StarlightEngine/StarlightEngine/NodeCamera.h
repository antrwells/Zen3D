#pragma once
#include "Node3D.h"



    /// <summary>
    /// The NodeCamera class is how you manipulate the applications view of the scene.
    /// SceneGraph creates this automatically for you.
    /// </summary>
    class NodeCamera :
        public Node3D
    {
    public:

        NodeCamera();

        /// <summary>
        /// Creates the projection matrix based on these metrics
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        void SetViewport(int x, int y, int w, int h);

        /// <summary>
           /// returns the world matrix, which is the position, rotation and scale of a node, and is modulated by it's root nodes.
           /// And it is inverted, as it is overriden for the camera class.
           /// </summary>
           /// <returns></returns>
        float4x4 GetWorldMatrix();
        float4x4 GetProjectionMatrix();
          

        /// <summary>
        /// How close to 0 a point in the render can happen.
        /// </summary>
        /// <returns></returns>
        float GetMinZ() {
            return mMinZ;
        }

        /// <summary>
        /// How far into the distance the camera can see.
        /// </summary>
        /// <returns></returns>
        float GetMaxZ() {
            return mMaxZ;
        }

        /// <summary>
        /// Set the minimum distance of the render.
        /// </summary>
        /// <param name="z"></param>
        void SetMinZ(float z) {
            mMinZ = z;
        }

        /// <summary>
        /// Set the maximum distance of the render.
        /// </summary>
        /// <param name="z"></param>
        void SetMaxZ(float z) {
            mMaxZ = z;
            SetViewport(0, 0, mWidth, mHeight);
        }

        /// <summary>
        /// The field of view in degrees.
        /// </summary>
        /// <param name="fov"></param>
        void SetFOV(float fov)
        {
            mFOV = fov;
            SetViewport(0, 0, mWidth, mHeight);
        }

    private:

        float4x4 mProjectionMatrix;

        float mFOV = 45.0f;
        float mMinZ = 0.05f;
        float mMaxZ = 60.0f;
        float mWidth, mHeight;

    };


    