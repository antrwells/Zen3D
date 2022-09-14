#include "pch.h"
#include "MeshActor.h"
#include <cmath>
#include "VString.h"
//#include "glm/geometric.hpp"
//#include "glm/vec2.hpp"
//#include "glm/vec3.hpp"
//#include "glm/vec4.hpp"


MeshActor::MeshActor() {

	mVertices.resize(0);
	mMaterial = new Material();
	mMaterial->SetType(MaterialType::PBR);

}



void MeshActor::AddVertex(VertexActor vertex, bool reset = true) {

	//***
	if (reset) {
		for (int i = 0; i < 4; i++)
		{
			vertex.m_BoneIDS[i] = -1;
			vertex.m_Weights[i] = 0.0f;
		}
	}
	mVertices.push_back(vertex);

}

std::vector<VertexActor> MeshActor::GetVertices() {

	return mVertices;

}

void MeshActor::AddTri(Tri tri) {

	mTris.push_back(tri);

}

std::vector<Tri> MeshActor::GetTris()
{

	return mTris;

}

//Material


void MeshActor::SetMaterial(Material* material) {

	mMaterial = material;

}

void MeshActor::BindMaterial()
{

	mMaterial->Bind();

}

void MeshActor::ReleaseMaterial() {

	mMaterial->Release();

}


//OpenGL


void MeshActor::CreateBuffers() {


	int ds = sizeof(VertexActor) * mVertices.size();



	BufferDesc VertBuffDesc;
	VertBuffDesc.Name = "MeshActor - vertex buffer";
	VertBuffDesc.Usage = USAGE_IMMUTABLE;
	VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER | BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	VertBuffDesc.Size = ds;
	VertBuffDesc.ElementByteStride = sizeof(VertexActor);
	VertBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
	BufferData VBData;
	VBData.pData = mVertices.data();
	VBData.DataSize = ds;

	Application::GetApp()->GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &m_VertexBuffer);

	BufferViewDesc ViewDesc;
	ViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
	ViewDesc.ByteOffset = 0;
	ViewDesc.ByteWidth = sizeof(VertexActor) * mVertices.size();
	m_VertexBuffer->CreateView(ViewDesc, &m_VertexBufferView);
	//ASSERT_NE(pVertexBufferView, nullptr);


	int b = 5;

	ds = sizeof(Tri) * mTris.size();

	BufferDesc IndBuffDesc;
	IndBuffDesc.Name = "MeshActor - index buffer";
	IndBuffDesc.Usage = USAGE_IMMUTABLE;
	IndBuffDesc.BindFlags = BIND_INDEX_BUFFER | BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	IndBuffDesc.Size = ds;
	IndBuffDesc.ElementByteStride = sizeof(uint);
	IndBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
	BufferData IBData;

	IBData.pData = mTris.data();
	IBData.DataSize = ds;
	Application::GetApp()->GetDevice()->CreateBuffer(IndBuffDesc, &IBData, &m_IndexBuffer);



	b = 5;

	//CreateRTBuffers();

	int a = 5;

}

//RT Vertex

struct RTVertex {

	float3 pos;

};

int RTBufferIndex2 = 0;

void MeshActor::CreateRTBuffers() {


	;
	std::vector<RTVertex> rtVertices;


	for (int i = 0;i < mVertices.size();i++) {

		auto vert = mVertices[i];
		RTVertex rtVert;
		rtVert.pos = vert.position;
		rtVertices.push_back(rtVert);

	}

	std::vector<uint> rtIndices;

	for (int i = 0;i < mTris.size();i++) {

		auto tri = mTris[i];

		rtIndices.push_back((uint)tri.v0);
		rtIndices.push_back((uint)tri.v1);
		rtIndices.push_back((uint)tri.v2);
	}


	RefCntAutoPtr<IBuffer> pScratchBuffer;

	VString geo_name = VString("Geo ");
	geo_name.Add(VString(RTBufferIndex2++));

	mGeoName = geo_name.GetConst();

	BLASTriangleDesc Triangles;
	{
		Triangles.GeometryName = geo_name.GetConst();
		Triangles.MaxVertexCount = rtVertices.size();
		Triangles.VertexValueType = VT_FLOAT32;
		Triangles.VertexComponentCount = 3;
		Triangles.MaxPrimitiveCount = rtIndices.size() / 3;
		Triangles.IndexType = VT_UINT32;

		auto BLASName{ "Mesh RT BLAS" };

		VString b_name = VString("MeshBlas ");
		b_name.Add(VString(RTBufferIndex2 - 1));

		BottomLevelASDesc ASDesc;
		ASDesc.Name = b_name.GetConst();//"Mesh RT BLAS";//const Char*)BLASName.c_str();
		ASDesc.Flags = RAYTRACING_BUILD_AS_PREFER_FAST_TRACE;
		ASDesc.pTriangles = &Triangles;
		ASDesc.TriangleCount = 1;
		Application::GetApp()->GetDevice()->CreateBLAS(ASDesc, &mBLAS);

	}

	if (!pScratchBuffer || pScratchBuffer->GetDesc().Size < mBLAS->GetScratchBufferSizes().Build)
	{
		BufferDesc BuffDesc;
		BuffDesc.Name = "BLAS Scratch Buffer";
		BuffDesc.Usage = USAGE_DEFAULT;
		BuffDesc.BindFlags = BIND_RAY_TRACING;
		BuffDesc.Size = mBLAS->GetScratchBufferSizes().Build;

		pScratchBuffer = nullptr;
		Application::GetApp()->GetDevice()->CreateBuffer(BuffDesc, nullptr, &pScratchBuffer);
	}


	BLASBuildTriangleData TriangleData;
	TriangleData.GeometryName = Triangles.GeometryName;
	TriangleData.pVertexBuffer = m_VertexBuffer;
	TriangleData.VertexStride = m_VertexBuffer->GetDesc().ElementByteStride;
	TriangleData.VertexOffset = 0;//Uint64{ Mesh.FirstVertex } *Uint64{ TriangleData.VertexStride };
	TriangleData.VertexCount = mVertices.size();
	TriangleData.VertexValueType = Triangles.VertexValueType;
	TriangleData.VertexComponentCount = Triangles.VertexComponentCount;
	TriangleData.pIndexBuffer = m_IndexBuffer;
	TriangleData.IndexOffset = 0;//Uint64{ Mesh.FirstIndex } *Uint64{ Mesh.IndexBuffer->GetDesc().ElementByteStride };
	TriangleData.PrimitiveCount = Triangles.MaxPrimitiveCount;
	TriangleData.IndexType = Triangles.IndexType;
	TriangleData.Flags = RAYTRACING_GEOMETRY_FLAG_OPAQUE;

	BuildBLASAttribs Attribs;
	Attribs.pBLAS = mBLAS;
	Attribs.pTriangleData = &TriangleData;
	Attribs.TriangleDataCount = 1;

	Attribs.pScratchBuffer = pScratchBuffer;

	// Allow engine to change resource states.
	Attribs.BLASTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
	Attribs.GeometryTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
	Attribs.ScratchBufferTransitionMode = RESOURCE_STATE_TRANSITION_MODE_TRANSITION;

	Application::GetApp()->GetContext()->BuildBLAS(Attribs);

}

void MeshActor::UpdateVBO() {



}


