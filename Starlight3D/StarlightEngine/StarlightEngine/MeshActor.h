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

class MeshActor
{
public:

	MeshActor();

	void AddVertex(VertexActor vertex, bool reset);
	std::vector<VertexActor> GetVertices();

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

		VertexActor vertex = mVertices[index];
		for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
		{
			if (vertex.m_BoneIDS[i] < 0)
			{
				vertex.m_Weights[i] = weight;
				vertex.m_BoneIDS[i] = boneID;
				break;
			}
		}
		mVertices[index] = vertex;

	}

	void CreateBuffers();
	void CreateRTBuffers();
	void UpdateVBO();

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

	VertexActor GetVertex(int id) {
		return mVertices[id];
	}

	void SetMaterial(Material* material);

	//Material
	void BindMaterial();
	void ReleaseMaterial();

	Material* GetMaterial() {
		return mMaterial;
	}

	void SetVertices(std::vector<VertexActor> vertices) {

		mVertices = vertices;

	}

	RefCntAutoPtr<IBuffer>  GetVertexBuffer() {

		return m_VertexBuffer;
		//	return VertexBuffer;
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

	RefCntAutoPtr<IBufferView> GetVertexBufferView() {

		return m_VertexBufferView;

	}


private:
	std::vector<VertexActor> mVertices;
	std::vector<Tri> mTris;
	Node3D* mOwner;

	//Material
	Material* mMaterial;
	RefCntAutoPtr<IBuffer>                m_VertexBuffer;
	RefCntAutoPtr<IBufferView> m_VertexBufferView;
	RefCntAutoPtr<IBuffer>                m_IndexBuffer;
	RefCntAutoPtr<IBufferView> m_IndexBufferView;
	RefCntAutoPtr<IBottomLevelAS> mBLAS;
	const char* mGeoName;
};

