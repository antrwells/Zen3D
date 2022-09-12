#include "pch.h"
#include "BigBuffer.h"
#include "Application.h"
#include "Node3D.h"
void BigBuffer::AddData(std::vector<Vertex> vertices, std::vector<Tri> tris,Node3D* owner)
{
	int pid = mVertices.size();

	for (int i = 0;i < vertices.size();i++) {

		mVertices.push_back(vertices[i]);

	}

	GeoIndex gindex;

	gindex.start_tri = mTris.size();
	gindex.g_Model = owner->GetWorldMatrix();


	mGeos.push_back(gindex);

	for (int i = 0;i < tris.size();i++) {

		auto tri = tris[i];
		tri.v0 = pid + tri.v0;
		tri.v1 = pid + tri.v1;
		tri.v2 = pid + tri.v2;

		mTris.push_back(tri);

	}

	

}

void BigBuffer::CreateBuffers() {

	int ds = sizeof(Vertex) * mVertices.size();

	BufferDesc VertBuffDesc;
	VertBuffDesc.Name = "BigBuffer - vertex buffer";
	VertBuffDesc.Usage = USAGE_IMMUTABLE;
	VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER | BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	VertBuffDesc.Size = ds;
	VertBuffDesc.ElementByteStride = sizeof(Vertex);
	VertBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
	BufferData VBData;
	VBData.pData = mVertices.data();
	VBData.DataSize = ds;

	Application::GetApp()->GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &mVertexBuffer);

	BufferViewDesc ViewDesc;
	ViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
	ViewDesc.ByteOffset = 0;
	ViewDesc.ByteWidth = sizeof(Vertex) * mVertices.size();
	mVertexBuffer->CreateView(ViewDesc, &mVertexBufferView);
	//ASSERT_NE(pVertexBufferView, nullptr);


	int b = 5;

	ds = sizeof(Tri) * mTris.size();

	BufferDesc IndBuffDesc;
	IndBuffDesc.Name = "BigBuffer - index buffer";
	IndBuffDesc.Usage = USAGE_IMMUTABLE;
	IndBuffDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	IndBuffDesc.Size = ds;
	IndBuffDesc.ElementByteStride = sizeof(uint);
	IndBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
	BufferData IBData;

	IBData.pData = mTris.data();
	IBData.DataSize = ds;
	Application::GetApp()->GetDevice()->CreateBuffer(IndBuffDesc, &IBData, &mIndexBuffer);

	BufferViewDesc TriViewDesc;
	TriViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
	TriViewDesc.ByteOffset = 0;
	TriViewDesc.ByteWidth = sizeof(Tri) * mTris.size();
	mIndexBuffer->CreateView(TriViewDesc, &mIndexBufferView);

	BufferDesc GeoIndexDesc;
	GeoIndexDesc.Name = "BigBuffer - GeoIndex";
	GeoIndexDesc.Usage = USAGE_IMMUTABLE;
	GeoIndexDesc.BindFlags = BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	GeoIndexDesc.Size = sizeof(GeoIndex) * mGeos.size();
	GeoIndexDesc.ElementByteStride = sizeof(GeoIndex);
	GeoIndexDesc.Mode = BUFFER_MODE_STRUCTURED;

	BufferData GIData;
	GIData.pData = mGeos.data();
	GIData.DataSize = sizeof(GeoIndex) * mGeos.size();
	Application::GetApp()->GetDevice()->CreateBuffer(GeoIndexDesc, &GIData, &mGeoBuffer);

	BufferViewDesc GIViewDesc;
	GIViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
	GIViewDesc.ByteOffset = 0;
	GIViewDesc.ByteWidth = sizeof(GeoIndex) * mGeos.size();
	mGeoBuffer->CreateView(GIViewDesc, &mGeoBufferView);




}


RefCntAutoPtr<IBufferView> BigBuffer::GetVertexBufferView() {

	return mVertexBufferView;

}

RefCntAutoPtr<IBufferView> BigBuffer::GetIndexBufferView() {

	return mIndexBufferView;

}

RefCntAutoPtr<IBufferView> BigBuffer::GetGeoIndexBufferView() {

	return mGeoBufferView;

}