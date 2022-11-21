#include "ZenUI.h"
#include "Cinematic.h"
#include "CineTrack.h"
#include "ActorAnim.h"
struct NodeRef {

	Node3D* node = nullptr;

};

int trackx, tracky;

float time_pick = 0;
float cur_time = 0;
bool mPreviewCine = false;
float preview_time = 0.0f;
CineTrack* soloTrack = nullptr;
TrackKeyFrame* DragKey = nullptr;

void ZenUI::CinematicsEditorWindow() {

	if (mCinematicsFirst) {

		trackx = tracky = 0;
		ImGui::SetNextWindowPos(mCinematicsPos);
		ImGui::SetNextWindowSize(mCinematicsSize);
		mCinematicsFirst = false;
	}



	if (ImGui::Begin("Cinematics Editor", &mCinematicsOpen,ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar))
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

				if (soloTrack != nullptr) {

					auto node = soloTrack->GetNode();

					TrackKeyFrame* frame = new TrackKeyFrame;
					frame->SetTime(cur_time);
					frame->SetPosition(node->GetPosition());
					frame->SetScale(node->GetScale());
					frame->SetRotation(node->GetRotation());
					soloTrack->InsertKeyFrame(frame);

				}
				else {

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
			}
			ImGui::SameLine();
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
				if (preview_time > mCurrentCine->GetLength())
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
			bool time_changed = false;
			if (Application::GetApp()->GetInput()->IsMouseDown(0))
			{

				if ((Application::GetApp()->GetInput()->GetMouseY() - ImGui::GetWindowPos().y)>32)
				{

					time_pick = Application::GetApp()->GetInput()->GetMouseX() - ImGui::GetWindowPos().x;
					if (time_pick < vx) {
						time_pick = vx;
					}
					time_pick -= ox;
					time_changed = true;
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

			if (time_changed) {
				if (mCurrentCine != nullptr) {

					mCurrentCine->SetTime(cur_time);
				}
			}
	

			ImGui::SetCursorPos(ImVec2(time_pick+ox, vy));
			ImGui::Image(mTrackImage->GetView(), ImVec2(4, mCinematicsSize.y+256));


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


			int tn = 0;
			for (int i = 0; i < tracks.size(); i++) {
				tn = i;
				ImGui::SetCursorPos(ImVec2(dx, dy));
				ImGui::Image(mTrackImage->GetView(), ImVec2(128, 42));
				ImGui::SetCursorPos(ImVec2(dx + 98, dy + 5));
				bool tSolo = tracks[i]->GetSolo();
				ImGui::PushID(299 + i);
				if (ImGui::Checkbox(" ", &tSolo))
				{
					tracks[i]->SetSolo(tSolo);
					if (tSolo) {
						soloTrack = tracks[i];
						for (int k = 0; k < tracks.size(); k++)
						{
							if (k != i) {
								tracks[k]->SetSolo(false);
							}
						}
					}
				}
				ImGui::PopID();
				ImGui::SetCursorPos(ImVec2(dx+7, dy + 10));
				ImGui::Text(tracks[i]->GetNode()->GetName());
				ImGui::SetCursorPos(ImVec2(dx, dy + 42));
				ImGui::Image(mTrackImage->GetView(), ImVec2(mCinematicsSize.x, 2));
				dy = dy + 44;

				auto keys = tracks[i]->GetKeys();

				for (int k = 0; k < keys.size(); k++)
				{
					auto key = keys[k];
					float k_time = key->GetTime();

					float adx = 134 + k_time * 100.0f;

					int mmx, mmy;
					mmx = Application::GetApp()->GetInput()->GetMouseX() - ImGui::GetWindowPos().x;
					mmy = Application::GetApp()->GetInput()->GetMouseY() - ImGui::GetWindowPos().y;

					int by, by2;

					by = vy + 4 + (42 * tn);

					if (mmx > adx - 2 + ox && mmx < adx - 2 + ox + 8) {
						if (mmy > by && mmy < by +42)
						{
							ImGui::SetCursorPos(ImVec2(adx - 2 + ox - 3,by));
							ImGui::Image(mTrackImage->GetView(), ImVec2(14, 42));
							if (Application::GetApp()->GetInput()->IsMouseDown(0))
							{
								if (DragKey == nullptr) {
									DragKey = key;
									if (ImGui::IsMouseDoubleClicked(0)) {
										
										mKeyFramePars = true;
										mSelectedKey = key;
										mSelectedTrack = tracks[i];

									}

								}
							}
						}
					}
					if (!Application::GetApp()->GetInput()->IsMouseDown(0)) {
						DragKey = nullptr;
					}
					else {
						if (DragKey != nullptr) {
							DragKey->SetTime(cur_time);
						}
					}

					ImGui::SetCursorPos(ImVec2(adx-2+ox, vy+4+(42*tn)));
					ImGui::Image(mTrackImage->GetView(), ImVec2(8,42));

					

			



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

void ZenUI::KeyFrameParsWindow() {

	if (mKeyFrameParsFirst) {

		ImGui::SetNextWindowPos(ImVec2(150, 300));
		ImGui::SetNextWindowSize(ImVec2(400, 500));

		mKeyFrameParsFirst = false;
	}

	auto node = mSelectedTrack->GetNode();

	if (ImGui::Begin("Key Parameters", &mKeyFramePars))
	{

		ImGui::Text("Transformation");



		float pos[3];
		pos[0] = mSelectedKey->GetPosition().x;
		pos[1] = mSelectedKey->GetPosition().y;
		pos[2] = mSelectedKey->GetPosition().z;
		if (ImGui::DragFloat3("Position", pos, 0.1f))
		{
			mSelectedKey->SetPosition(float3(pos[0], pos[1], pos[2]));
		}

		float scal[3];
		scal[0] = mSelectedKey->GetScale().x;// node->GetScale().x;
		scal[1] = mSelectedKey->GetScale().y;
		scal[2] = mSelectedKey->GetScale().z;

		if (ImGui::DragFloat3("Scale",scal))
		{
			mSelectedKey->SetScale(float3(scal[0], scal[1], scal[2]));

		}

		if (node->GetType() == NodeType::Actor)
		{
			auto act = (NodeActor*)node;
			bool play_anim = mSelectedKey->GetPlayAnim();
			if (ImGui::Checkbox("Start Anim", &play_anim))
			{
				mSelectedKey->SetPlayAnim(play_anim);
			}

			//const char* items[] = { "Point", "Spot","Directional" };
			if (play_anim) {
				std::vector<const char*> items;
				for (int i = 0; i < act->GetAnims().size(); i++)
				{
					auto anim = act->GetAnims()[i];
					items.push_back(anim->mName.c_str());
				}
				int anum = 0;
				std::string cur = mSelectedKey->GetAnimName();
				for (int a = 0; a < items.size(); a++) {
					if (items[a] == cur)
					{
						anum = a;
						break;
					}
				}

				if (ImGui::Combo("Anim", &anum, items.data(), items.size())) {
				}
					mSelectedKey->SetAnimName(items[anum]);

				
			}
		}

	}
	ImGui::End();
}