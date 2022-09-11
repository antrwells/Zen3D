#include "pch.h"
#include "Mesh3D.h"
#include <cmath>
#include "VString.h"
//#include "glm/geometric.hpp"
//#include "glm/vec2.hpp"
//#include "glm/vec3.hpp"
//#include "glm/vec4.hpp"


	Mesh3D::Mesh3D() {

		mVertices.resize(0);
		mMaterial = new Material();
		mMaterial->SetType(MaterialType::PBR);

	}




	void makeTangents(uint32_t nIndices, uint16_t* indices,
		float3* positions, float3* normals,
		float2* texCoords, float4* tangents) {
		uint32_t inconsistentUvs = 0;
		for (uint32_t l = 0; l < nIndices; ++l) tangents[indices[l]] = float4(0);
		for (uint32_t l = 0; l < nIndices; ++l) {
			uint32_t i = indices[l];
			uint32_t j = indices[(l + 1) % 3 + l / 3 * 3];
			uint32_t k = indices[(l + 2) % 3 + l / 3 * 3];
			float3 n = normals[i];
			float3 v1 = positions[j] - positions[i], v2 = positions[k] - positions[i];
			float2 t1 = texCoords[j] - texCoords[i], t2 = texCoords[k] - texCoords[i];


			// Is the texture flipped?
			float uv2xArea = t1.x * t2.y - t1.y * t2.x;
			if (std::abs(uv2xArea) < 0x1p-20)
				continue;  // Smaller than 1/2 pixel at 1024x1024
			float flip = uv2xArea > 0 ? 1 : -1;
			// 'flip' or '-flip'; depends on the handedness of the space.
			if (tangents[i].w != 0 && tangents[i].w != -flip) ++inconsistentUvs;
			tangents[i].w = -flip;

			// Project triangle onto tangent plane
			v1 -= n * dot(v1, n);
			v2 -= n * dot(v2, n);
			// Tangent is object space direction of texture coordinates
			float3 s = normalize((t2.y * v1 - t1.y * v2) * flip);

			// Use angle between projected v1 and v2 as weight
			float angle = std::acos(dot(v1, v2) / (length(v1) * length(v2)));
			tangents[i] += float4(s * angle, 0);
		}
		for (uint32_t l = 0; l < nIndices; ++l) {
			float4& t = tangents[indices[l]];
			t = float4(normalize(float3(t.x, t.y, t.z)), t.w);
		}
		// std::cerr << inconsistentUvs << " inconsistent UVs\n";
	}
	float3 computeFaceNormal(float3 p1, float3 p2, float3 p3) {
		// Uses p2 as a new origin for p1,p3
		auto a = p3 - p2;
		auto b = p1 - p2;
		// Compute the cross product a X b to get the face normal
		return normalize(cross(a, b));

		//return normalize(glm::cross(a, b));

	}

	void Mesh3D::GenerateTangents() {

		uint32_t nIndices = mTris.size() * 3;

		uint16_t* indices = (uint16_t*)malloc(nIndices * 4);
		float3* positions = new float3[mVertices.size() * 3];
		float3* normals = new float3[mVertices.size() * 3];
		float2* uvs = new float2[mVertices.size() * 3];
		float4* tan = new float4[mVertices.size() * 3];

		int ic = 0;
		for (int i = 0; i < mTris.size(); i++) {

			indices[ic] = mTris[i].v0;
			indices[ic + 1] = mTris[i].v1;
			indices[ic + 2] = mTris[i].v2;

			ic = ic + 3;



		}

		ic = 0;

		for (int i = 0; i < mVertices.size(); i++) {


			positions[i] = mVertices[i].position;

			normals[i] = mVertices[i].normal;

			uvs[i] = float2(mVertices[i].texture_coord.x, mVertices[i].texture_coord.y);



			//uvs[ic] = glm::vec2(mVertices[mTris[i].v0].texture_coord.x, mVertices[mTris[i].v0].texture_coord.y);
			//uvs[ic+1] = glm::vec2(mVertices[mTris[i].v1].texture_coord.x, mVertices[mTris[i].v1].texture_coord.y);
			//uvs[ic+2] = glm::vec2(mVertices[mTris[i].v2].texture_coord.x, mVertices[mTris[i].v2].texture_coord.y);


		}
		makeTangents(nIndices, indices, positions, normals, uvs, tan);

		for (int i = 0; i < mVertices.size(); i++) {

			mVertices[i].tangent = float3(tan[i].x, tan[i].y, tan[i].z);


		}

		int b = 5;

	}

	void Mesh3D::GenerateNormals() {


		std::vector<float3> normals2(mVertices.size());

		for (int i = 0; i < mTris.size(); i++) {

			auto tri = mTris[i];

			float3 A = mVertices[tri.v0].position;
			float3 B = mVertices[tri.v1].position;
			float3 C = mVertices[tri.v2].position;
			float3 norm = computeFaceNormal(A, B, C);
			//norm.z = -norm.z;

			normals2[tri.v0] += norm;
			normals2[tri.v1] += norm;
			normals2[tri.v2] += norm;

		}

		for (int i = 0; i < normals2.size(); i++) {

			normals2[i] = normalize(normals2[i]);
			mVertices[i].normal = -normals2[i];

		}

		for (int i = 0; i < mTris.size(); i++) {

			float3 pos1, pos2, pos3;
			float2 uv1, uv2, uv3;

			float3 tangent1;
			float3 bitangent1;

			pos1 = mVertices[mTris[i].v0].position;
			pos2 = mVertices[mTris[i].v1].position;
			pos3 = mVertices[mTris[i].v2].position;

			uv1 = mVertices[mTris[i].v0].texture_coord;
			uv2 = mVertices[mTris[i].v1].texture_coord;
			uv3 = mVertices[mTris[i].v2].texture_coord;

			float3 norm = computeFaceNormal(pos1, pos2, pos3);

			float3 edge1 = pos2 - pos1;
			float3 edge2 = pos3 - pos1;

			float2 deltaUV1 = uv2 - uv1;
			float2 deltaUV2 = uv3 - uv1;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			//tangent1 = -tangent1;
			//bitangent1 = bitangent1;

			mVertices[mTris[i].v0].tangent = tangent1;
			mVertices[mTris[i].v1].tangent = tangent1;
			mVertices[mTris[i].v2].tangent = tangent1;

			mVertices[mTris[i].v0].bi_normal = bitangent1;
			mVertices[mTris[i].v1].bi_normal = bitangent1;
			mVertices[mTris[i].v2].bi_normal = bitangent1;

		}

		//return;
		/*
		for (int i = 0; i < mTris.size(); i++) {

			auto tri = mTris[i];

			auto p1 = mVertices[tri.v0].position;
			auto p2 = mVertices[tri.v1].position;
			auto p3 = mVertices[tri.v2].position;

			glm::vec3 u = p2 - p1;
			glm::vec3 v = p3 - p1;

			glm::vec3 normal(0, 0, 0);

			normal.x = (u.y * v.z) - (u.z * v.y);
			normal.y = (u.z * v.x) - (u.x * v.z);
			normal.z = (u.x * v.y) - (u.y * v.x);

			//mVertices[tri.v0].normal = -normal;
			//mVertices[tri.v1].normal = -normal;
			//mVertices[tri.v2].normal = -normal;

		
		}
		*/

	//	GenerateTangents();




	}

	void Mesh3D::AddVertex(Vertex vertex,bool reset=true) {

		//***
		if (reset) {
			for (int i = 0; i < 4; i++)
			{
				//vertex.m_BoneIDS[i] = -1;
				//vertex.m_Weights[i] = 0.0f;
			}
		}
		mVertices.push_back(vertex);

	}

	std::vector<Vertex> Mesh3D::GetVertices() {

		return mVertices;

	}

	void Mesh3D::AddTri(Tri tri) {

		mTris.push_back(tri);

	}

	std::vector<Tri> Mesh3D::GetTris()
	{

		return mTris;

	}

	//Material


	void Mesh3D::SetMaterial(Material* material) {

		mMaterial = material;

	}

	void Mesh3D::BindMaterial()
	{

		mMaterial->Bind();

	}

	void Mesh3D::ReleaseMaterial() {

		mMaterial->Release();

	}


	//OpenGL


	void Mesh3D::CreateBuffers() {


		int ds = sizeof(Vertex) * mVertices.size();


	
		BufferDesc VertBuffDesc;
		VertBuffDesc.Name = "Mesh3D - vertex buffer";
		VertBuffDesc.Usage = USAGE_IMMUTABLE;
		VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER | BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
		VertBuffDesc.Size = ds;
		VertBuffDesc.ElementByteStride = sizeof(Vertex);
		VertBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
		BufferData VBData;
		VBData.pData = mVertices.data();
		VBData.DataSize = ds;
		
		Application::GetApp()->GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &m_VertexBuffer);

		int b = 5;

		ds = sizeof(Tri) * mTris.size();

		BufferDesc IndBuffDesc;
		IndBuffDesc.Name = "Mesh3D - index buffer";
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

		CreateRTBuffers();

		int a = 5;

	}

	//RT Vertex

	struct RTVertex {

		float3 pos;

	};

	int RTBufferIndex = 0;

	void Mesh3D::CreateRTBuffers() {


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
		geo_name.Add(VString(RTBufferIndex++));

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
			b_name.Add(VString(RTBufferIndex-1));

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
		TriangleData.VertexStride =   m_VertexBuffer->GetDesc().ElementByteStride;
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

	void Mesh3D::UpdateVBO() {

		

	}



	void Mesh3D::BindVBO() {

	

	}


	void Mesh3D::ReleaseVBO() {

		/*
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
		glDisableVertexAttribArray(7);
		*/

	



	}

	void Mesh3D::DrawVBO() {


		
	}

