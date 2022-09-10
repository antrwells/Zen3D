#include "pch.h"
#include "GBuffer.h"
#include "SceneGraph.h"
#include "SmartDraw.h"

GBuffer::GBuffer(int width, int height) {

	mTexture = new RenderTarget2D(width, height);
	mNormal = new RenderTarget2D(width, height);
	mDepth = new RenderTarget2D(width, height);
	mPositions = new RenderTarget2D(width, height);
	mDraw = new SmartDraw(Application::GetApp());
	mDepth->SetClearColor(float4(1, 1, 1, 1));

}

void GBuffer::Render(SceneGraph* graph) {

	mTexture->Bind();

	graph->RenderTextures();

	mTexture->Release();

	mNormal->Bind();

	graph->RenderNormals();

	mNormal->Release();

	mDepth->Bind();

	graph->RenderDepth();

	mDepth->Release();

	mPositions->Bind();

	graph->RenderPositions();

	mPositions->Release();

}


void GBuffer::Debug(int id) {

	mDraw->Begin();

	switch (id) {
	case 0:
		mDraw->DrawTexture(0, 0, 512, 512, new Texture2D(mTexture), 1, 1, 1, 1);
		break;
	case 1:
		mDraw->DrawTexture(0, 0, 512, 512, new Texture2D(mNormal), 1, 1, 1, 1);
		break;
	case 2:
		mDraw->DrawTexture(0, 0, 512, 512, new Texture2D(mDepth), 1, 1, 1, 1);
		break;
	case 3:
		mDraw->DrawTexture(0, 0, 512, 512, new Texture2D(mPositions), 1, 1, 1, 1);
	};
	mDraw->End();

}

RenderTarget2D* GBuffer::GetTextures() {

	return mTexture;

}

RenderTarget2D* GBuffer::GetNormals() {

	return mNormal;

}


RenderTarget2D* GBuffer::GetDepth() {

	return mDepth;

}

RenderTarget2D* GBuffer::GetPositions() {

	return mPositions;

}