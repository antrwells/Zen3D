#pragma once
#include <vector>
#include "DataTypes.h"

#ifndef PLATFORM_WIN32
#    define PLATFORM_WIN32 1
#endif

#include "Graphics/GraphicsEngineD3D11/interface/EngineFactoryD3D11.h"
#include "Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h"
#include "Graphics/GraphicsEngineOpenGL/interface/EngineFactoryOpenGL.h"
#include "Graphics/GraphicsEngineVulkan/interface/EngineFactoryVk.h"

#include "Graphics/GraphicsEngine/interface/RenderDevice.h"
#include "Graphics/GraphicsEngine/interface/DeviceContext.h"
#include "Graphics/GraphicsEngine/interface/SwapChain.h"
#include "Graphics/GraphicsTools/interface/MapHelper.hpp"
#include "Common/interface/RefCntAutoPtr.hpp"
#include "Graphics/GraphicsTools/interface/GraphicsUtilities.h"
//#include "glad/glad.h"
#include "Material.h"


class Node3D;
	/// <summary>
	/// A mesh3D is used to define a three dimensional mesh, often used by NodeEntity's to create the overall 3D scene.
	/// </summary>
	class Mesh3D
	{
	public:

		/// <summary>
		/// Creates an empty mesh. Meshes are used as part of a scenegraph/nodes to achieve a real time 3D scene, with lighting and shadows.
		/// </summary>
		Mesh3D();


		void SetOwner(Node3D* owner) {

			mOwner = owner;

		}

		Node3D* GetOwner()
		{
			return mOwner;
		}

		void MakeDoubleSided() {

			std::vector<Tri> new_tris;

			for (int i = 0; i < mTris.size(); i++) {

				auto old_tri = mTris[i];

				Tri new_tri;

				new_tri.v0 = old_tri.v0;
				new_tri.v1 = old_tri.v2;
				new_tri.v2 = old_tri.v1;

				new_tris.push_back(new_tri);
				

			}

			for (int i = 0; i < new_tris.size(); i++) {
				mTris.push_back(new_tris[i]);
			}

		}

		Mesh3D* Unwind() {

			int v = 0;

			std::vector<Vertex> verts;
			std::vector<Tri> tris;

			for (int i = 0; i < mTris.size(); i++) {

				Tri tri;

				tri = mTris[i];

				Tri new_tri;


				verts.push_back(mVertices[tri.v0]);
				verts.push_back(mVertices[tri.v1]);
				verts.push_back(mVertices[tri.v2]);
				new_tri.v0 = v;
				new_tri.v1 = v + 1;
				new_tri.v2 = v + 2;
				tris.push_back(new_tri);
				v = v + 3;


			}

			Mesh3D* new_mesh = new Mesh3D;
			new_mesh->SetVertices(verts);
			new_mesh->SetTris(tris);
			new_mesh->SetMaterial(mMaterial);
			new_mesh->GenerateNormals();
			new_mesh->CreateBuffers();
			return new_mesh;


		}

		void AddVertex(Vertex vertex,bool reset);
		std::vector<Vertex> GetVertices();

		void AddTri(Tri tri);
		void SetTris(std::vector<Tri> tris) {
			mTris = tris;
		}

		std::vector<Tri> GetTris();

		void SetBoneData(int index, int boneID, float weight) {

			/// <summary>
			/// ***
			/// </summary>
			/// <param name="index"></param>
			/// <param name="boneID"></param>
			/// <param name="weight"></param>
			int MAX_BONE_WEIGHTS = 4;

			Vertex vertex = mVertices[index];
			for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
			{
				//if (vertex.m_BoneIDS[i] < 0)
				{
					//vertex.m_Weights[i] = weight;
					//vertex.m_BoneIDS[i] = boneID;
					break;
				}
			}
			mVertices[index] = vertex;

		}

		//VertexBuffer
		void CreateBuffers();
		void CreateRTBuffers();
		void BindVBO();
		void ReleaseVBO();
		void DrawVBO();
		void UpdateVBO();

		//normals
		void GenerateNormals();
		void GenerateTangents();

		Tri GetTri(int id) {
			return mTris[id];
		}

		int NumTris() {
			return mTris.size();
		}

		int NumVertices() {
			return mVertices.size();
		}

		Vertex GetVertex(int id) {
			return mVertices[id];
		}

		void SetMaterial(Material* material);

		//Material
		void BindMaterial();
		void ReleaseMaterial();

		Material* GetMaterial() {
			return mMaterial;
		}

		void SetVertices(std::vector<Vertex> vertices) {

			mVertices = vertices;

		}

		int GetVertexArray() {
			return 0;
			//return VertexArray;
		}

		RefCntAutoPtr<IBuffer>  GetVertexBuffer() {
			
			return m_VertexBuffer;
			//	return VertexBuffer;
		}

		void SetVB(int va, int vb) {
			//VertexArray = va;
			//VertexBuffer = vb;
		}

		RefCntAutoPtr<IBuffer>  GetIndexBuffer() {

			return m_IndexBuffer;

		//	if (IndexBuffer == -1) {
	//			glGenBuffers(1, &IndexBuffer);
		//	}
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
		//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Kinetic::DataTypes::Tri) * mTris.size(), mTris.data(), GL_STATIC_DRAW);


		}

		RefCntAutoPtr<IBottomLevelAS> GetBlas()
		{

			return mBLAS;
		}

	private:
		
		/// <summary>
		/// Data
		/// </summary>
		std::vector<Vertex> mVertices;
		std::vector<Tri> mTris;
		Node3D* mOwner;

		//Material
		Material* mMaterial;
		RefCntAutoPtr<IBuffer>                m_VertexBuffer;
		RefCntAutoPtr<IBuffer>                m_IndexBuffer;
		RefCntAutoPtr<IBottomLevelAS> mBLAS;
		const char* mGeoName;
		//OpenGL
		//GLuint VertexArray = -1;
		//GLuint VertexBuffer = -1;
		//GLuint IndexBuffer = -1;


	};



