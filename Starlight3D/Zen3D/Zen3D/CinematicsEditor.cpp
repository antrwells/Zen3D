#include "ZenUI.h"
#include "Cinematic.h"
#include "CineTrack.h"

struct NodeRef {

	Node3D* node = nullptr;

};

int trackx, tracky;

float time_pick = 0;
float cur_time = 0;
bool mPreviewCine = false;
float preview_time = 0.0f;

void ZenUI::CinematicsEditorWindow() {

	if (mCinematicsFirst) {

		trackx = tracky = 0;
		ImGui::SetNextWindowPos(mCinematicsPos);
		ImGui::SetNextWindowSize(mCinematicsSize);
		mCinematicsFirst = false;
	}



	if (ImGui::Begin("Cinematics Editor", &mCinematicsOpen,ImGuiWindowFlags_MenuBar))
	{

		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("Load Cinematic"))
				{


				}

				if (ImGui::MenuItem("Save Cinematic"))
				{

					saveCineBrowseOpen = true;

				}

				ImGui::EndMenu();

			}
			ImGui::EndMenuBar();
		}

		if (ImGui::Button("New Cinematic"))
		{
			mCurrentCine = new Cinematic;

		}

		ImGui::SameLine();

		if (mCurrentCine != nullptr) {
			if (ImGui::Button("Record"))
			{

				auto tracks = mCurrentCine->GetTracks();

				for (int i = 0; i < tracks.size(); i++) {

					auto track = tracks[i];

					auto node = track->GetNode();

					TrackKeyFrame* frame = new TrackKeyFrame;
					frame->SetTime(cur_time);
					frame->SetPosition(node->GetPosition());
					frame->SetScale(node->GetScale());
					frame->SetRotation(node->GetRotation());
					track->InsertKeyFrame(frame);

				}

			}

			if (ImGui::Button("Preview"))
			{

				if (mPreviewCine == false) {
					mPreviewCine = true;
				}
				else {
					mPreviewCine = false;
				}

			}

			ImGui::SameLine();

			float length = mCurrentCine->GetLength();

			ImGui::PushItemWidth(128);

			if (ImGui::DragFloat("Total Time", &length, 0.1f, 0.1f, 512.0f))
			{
				mCurrentCine->SetLength(length);
			}

			ImGui::SameLine();

			float spd = mCurrentCine->GetSpeed();

			if (ImGui::DragFloat("Speed", &spd, 0.01f, 0.001f, 1.0f))
			{
				mCurrentCine->SetSpeed(spd);
			}

			ImGui::PopItemWidth();
			if (mPreviewCine)
			{
				preview_time += 0.05f;
				if (preview_time > 5.0f)
				{
					preview_time = 0.0f;
				}

				mCurrentCine->SetTime(preview_time);

			}

		}

		if (mCurrentCine != nullptr) {
			char* buf = (char*)malloc(512);
			auto name = mCurrentCine->GetName();

			for (int i = 0; i < 512; i++) {

				int ch = name[i];
				if (ch == "\0"[0])
				{
					buf[i] = "\0"[0];
					break;
				}
				buf[i] = ch;

			}
			ImGui::SameLine();
			ImGui::PushItemWidth(256);
			if (ImGui::InputText("Cinematic Name", buf, 512))
			{
				mCurrentCine->SetName(std::string(buf));
			}
			ImGui::PopItemWidth();
			ImGui::BeginChild(255);

			auto tracks = mCurrentCine->GetTracks();

			int tx, ty;

			tx = 134;
			ty = 23;

			int mx, my;
			mx = Application::GetApp()->GetInput()->GetMouseDX();
			my = Application::GetApp()->GetInput()->GetMouseDY();

			if (Application::GetApp()->GetInput()->IsMouseDown(1))
			{
				trackx += mx;
				tracky += my;
				if(trackx<0)
				{
					trackx = 0;
				}
				if (tracky < 0)
				{
					tracky = 0;
				}
			}

			int tt = tracks.size();
			int mh = tt * 34 - 16;

			if (tracky > mh) {
				tracky = mh;
			}

			int vx, vy;
			vx = tx - trackx;
			vy = ty - tracky;
			ImGui::SetCursorPos(ImVec2(vx, vy));
			ImGui::Image(mTrackImage->GetView(), ImVec2(mCinematicsSize.x, 4));

			tx = tx - trackx;
			ty = ty - 14 - tracky;

			float time = 0.0f;

			int ox = tx - 134;

			if (Application::GetApp()->GetInput()->IsMouseDown(0))
			{

				if ((Application::GetApp()->GetInput()->GetMouseY() - ImGui::GetWindowPos().y)>62)
				{

					time_pick = Application::GetApp()->GetInput()->GetMouseX() - ImGui::GetWindowPos().x;
					if (time_pick < vx) {
						time_pick = vx;
					}
					time_pick -= ox;
					//time_pick + vx;
				}
			
			
			//	time_pick = time_pick / 100.0f;

				//time_pick += 1.0f;
			}

		
			printf("TimePick:%d\n", (int)time_pick);
			//time_pick = 0;
			float rtime = (time_pick - vx+ox) / 100.0f;

			printf("Time:%f\n", rtime);
			cur_time = rtime;


			ImGui::SetCursorPos(ImVec2(time_pick+ox, vy));
			ImGui::Image(mTrackImage->GetView(), ImVec2(4, mCinematicsSize.y));


			while (true) {

				ImGui::SetCursorPos(ImVec2(tx, ty));
				std::string text = std::string(std::to_string(time));

				text = text.substr(0, 4);

				ImGui::Text(text.c_str());
				tx = tx + 100;
				time = time + 1.0f;
				if (tx > ImGui::GetWindowSize().x)
				{
					break;
				}
			}



			int dy = 26 - tracky;
			int dx = 5 - trackx;



			for (int i = 0; i < tracks.size(); i++) {
				ImGui::SetCursorPos(ImVec2(dx, dy));
				ImGui::Image(mTrackImage->GetView(), ImVec2(128, 32));
				ImGui::SetCursorPos(ImVec2(dx+7, dy + 10));
				ImGui::Text(tracks[i]->GetNode()->GetName());
				ImGui::SetCursorPos(ImVec2(dx, dy + 32));
				ImGui::Image(mTrackImage->GetView(), ImVec2(mCinematicsSize.x, 2));
				dy = dy + 34;

				auto keys = tracks[i]->GetKeys();

				for (int k = 0; k < keys.size(); k++)
				{
					auto key = keys[k];
					float k_time = key->GetTime();

					float adx = 134 + k_time * 100.0f;

					ImGui::SetCursorPos(ImVec2(adx-2+ox, vy));
					ImGui::Image(mTrackImage->GetView(), ImVec2(8,128));




				}

			}


			ImGui::EndChild();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Node"))
				{
					auto nr = (NodeRef*)payload->Data;
					auto node2 = nr->node;
					
					CineTrack* new_track = new CineTrack;
					new_track->SetNode(node2);

					mCurrentCine->InsertTrack(new_track);
					//mSelectedNode = node2;
					//SaveNode(node2->GetName());
					//ScanContent(mContentPath->GetConst());

					//node2->GetRootNode()->RemoveNode(node2);
					//node->AddNode(node2);


				}
				ImGui::EndDragDropTarget();
			}
		}

		
		ImGui::End();
	}
}