#include "pch.h"
#include "Cinematic.h"
#include "SceneGraph.h"

void Cinematic::InsertTrack(CineTrack* track)
{

	mTracks.push_back(track);

}

void Cinematic::SetTime(float t1, float t2) {

	mTime = t1;
	mEnd = t2;

}

std::string Cinematic::GetName() {

	return mName;

}

void Cinematic::SetName(std::string name) {

	mName = name;

}

std::vector<CineTrack*> Cinematic::GetTracks() {

	return mTracks;

}

void Cinematic::SetTime(float time) {
	
	mTime = time;
	for (int i = 0; i < mTracks.size(); i++)
	{
		mTracks[i]->UpdateTrack(time);
	}

}


void Cinematic::Write(VFile* file) {

	file->WriteFloat(mTime);
	file->WriteFloat(mEnd);
	file->WriteFloat(mSpeed);
	file->WriteString(mName.c_str());
	file->WriteInt(mTracks.size());

	for (int i = 0; i < mTracks.size(); i++)
	{

		mTracks[i]->Write(file);

	}


}

void Cinematic::Read(VFile* file,SceneGraph* graph) {

	mTime = file->ReadFloat();
	mEnd = file->ReadFloat();
	mSpeed = file->ReadFloat();
	mName = std::string(file->ReadString());
	int tracks = file->ReadInt();
	for (int i = 0; i < tracks; i++) {

		CineTrack* track = new CineTrack;

		track->Read(file);

		std::string node_name = track->GetNodeName();

		auto node = graph->FindNode(node_name);

		track->SetNode(node);

		mTracks.push_back(track);

	}

}

Cinematic::Cinematic() {

	mEnd = 5.0f;

}

float Cinematic::GetLength() {
	return mEnd;

}

void Cinematic::SetLength(float time) {

	mEnd = time;

}

float Cinematic::GetSpeed() {

	return mSpeed;

}

void Cinematic::SetSpeed(float spd)
{

	mSpeed = spd;

}

void Cinematic::Play() {

	mTime = 0;
	mPlaying = true;

}

void Cinematic::Update() {

	mTime += mSpeed;
	SetTime(mTime);
	if (mTime >= mEnd) {
		mTime = 0.0f;
	}

}