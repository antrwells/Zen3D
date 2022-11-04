#include "pch.h"
#include "CineTrack.h"

void sort_key(std::vector<TrackKeyFrame*>& keys)
{

	bool swap = true;
	while (swap) {
		swap = false;
		for (int i = 0; i < keys.size() - 1; i++)
		{
			if (keys[i]->GetTime() > keys[i + 1]->GetTime()) {

				swap = true;
				TrackKeyFrame* f1 = keys[i];
				keys[i] = keys[i + 1];
				keys[i + 1] = f1;

			}
		}

	}
}

void CineTrack::InsertKeyFrame(TrackKeyFrame* frame)
{

//	std::vector<TrackKeyFrame*> newKeys;


	mKeyFrames.push_back(frame);

	if (mKeyFrames.size() > 1)
	{

		sort_key(mKeyFrames);
	}

	//mKeyFrames = newKeys;
}

TrackKeyFrame* CineTrack::GetCurrentKey(float time) {

	int f1=-1, f2;

	for (int i = mKeyFrames.size()-1; i>=0; i--) {

		auto key = mKeyFrames[i];
		if (key->GetTime() < time) {
			f1 = i;
			break;
		}

	}

	if (f1 == -1) {
		return mKeyFrames[mKeyFrames.size() - 1];
	}
	f2 = f1 + 1;
	if (f2 >= mKeyFrames.size()) {
		return mKeyFrames[f1];
	}

	return mKeyFrames[f1]->GetCurrentKey(mKeyFrames[f2], time, false);


	return nullptr;
	if (time >= mKeyFrames[mKeyFrames.size() - 1]->GetTime())
	{
		return mKeyFrames[mKeyFrames.size() - 1];
	}

	bool found = false;
	for (int i = 0; i < mKeyFrames.size(); i++)
	{

		if (time <= mKeyFrames[i]->GetTime())
		{
			if (i == 0) {
				return mKeyFrames[0];
			}
			return mKeyFrames[i-1]->GetCurrentKey(mKeyFrames[i], time,false);

		}

	}
	return mKeyFrames[mKeyFrames.size() - 1];

}

void CineTrack::UpdateTrack(float time) {

	auto key = GetCurrentKey(time);

	mNode->SetPosition(key->GetPosition());
	mNode->SetScale(key->GetScale());
	mNode->SetRotation3x3(key->GetRotation());

}

void CineTrack::SetNode(Node3D* node) {

	mNode = node;

}

Node3D* CineTrack::GetNode() {

	return mNode;

}

std::vector<TrackKeyFrame*> CineTrack::GetKeys() {

	return mKeyFrames;

}


void CineTrack::Write(VFile* file)
{
	file->WriteString(mNode->GetName());
	file->WriteInt(mKeyFrames.size());
	for (int i = 0; i < mKeyFrames.size(); i++)
	{
		mKeyFrames[i]->Write(file);
	}

}

void CineTrack::Read(VFile* file) {

	mNodeName = std::string(file->ReadString());
	int frames = file->ReadInt();
	for (int i = 0; i < frames; i++) {

		TrackKeyFrame* frame = new TrackKeyFrame;
		frame->Read(file);
		mKeyFrames.push_back(frame);

	}

}