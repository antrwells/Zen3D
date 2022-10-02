#include "ZenUI.h"
#include "Importer.h"
#include "RayPicker.h"
#include "VString.h"
#include "DirCollection.h"
#include "VString.h"
#include "VFile.h"


void ZenUI::MainNodeEditor() {

	if (!mNEF)
	{
		ImGui::SetNextWindowPos(mNodeEditPos);
		ImGui::SetNextWindowSize(mNodeEditSize);
		mNEF = true;
	}

	int flags = ImGuiWindowFlags_MenuBar;

	if (ImGui::Begin("Node Editor"))
	{

		if (ImGui::Button("Add Script"))
		{
			exit(1);
		}

	}

	ImGui::End();

}
