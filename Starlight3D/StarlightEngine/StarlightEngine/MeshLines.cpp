#include "pch.h"
#include "MeshLines.h"
#include "Application.h"

MeshLines::MeshLines() {


}

void MeshLines::CreateBuffers() {

	int ds = sizeof(LinesVertex) * (int)mVertices.size();

	m_VertexBuffer.Detach();
	m_VertexBuffer.Release();
	m_VertexBufferView.Detach();
	m_VertexBufferView.Release();

	BufferDesc VertBuffDesc;
	VertBuffDesc.Name = "MeshLines - vertex buffer";
	VertBuffDesc.Usage = USAGE_IMMUTABLE;
	VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER | BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	VertBuffDesc.Size = ds;
	VertBuffDesc.ElementByteStride = sizeof(LinesVertex);
	VertBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
	BufferData VBData;
	VBData.pData = mVertices.data();
	VBData.DataSize = ds;

	Application::GetApp()->GetDevice()->CreateBuffer(VertBuffDesc, &VBData, &m_VertexBuffer);

	BufferViewDesc ViewDesc;
	ViewDesc.ViewType = BUFFER_VIEW_SHADER_RESOURCE;
	ViewDesc.ByteOffset = 0;
	ViewDesc.ByteWidth = sizeof(LinesVertex) * mVertices.size();
	m_VertexBuffer->CreateView(ViewDesc, &m_VertexBufferView);
	//ASSERT_NE(pVertexBufferView, nullptr);



	m_IndexBuffer.Detach();
	m_IndexBuffer.Release();
	int b = 5;

	ds = sizeof(Line) * (int)mLines.size();

	BufferDesc IndBuffDesc;
	IndBuffDesc.Name = "MeshLines - index buffer";
	IndBuffDesc.Usage = USAGE_IMMUTABLE;
	IndBuffDesc.BindFlags = BIND_INDEX_BUFFER | BIND_SHADER_RESOURCE | BIND_RAY_TRACING;
	IndBuffDesc.Size = ds;
	IndBuffDesc.ElementByteStride = sizeof(uint);
	IndBuffDesc.Mode = BUFFER_MODE_STRUCTURED;
	BufferData IBData;

	IBData.pData = mLines.data();
	IBData.DataSize = ds;
	Application::GetApp()->GetDevice()->CreateBuffer(IndBuffDesc, &IBData, &m_IndexBuffer);



}