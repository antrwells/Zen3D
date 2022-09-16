#include "pch.h"
#include "FrameTypeColor.h"
#include "SceneGraph.h"

FrameTypeColor::FrameTypeColor() {

	CreateResources();

}

void FrameTypeColor::GenerateFrame() {

	mGraph->Render();

}