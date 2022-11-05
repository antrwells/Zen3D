#pragma once
#include <string>
#include <vector>
#include "NodeActor.h"
#include "TrackKeyFrame.h"

class CineTrack
{
public:

	void InsertKeyFrame(TrackKeyFrame* frame);
	TrackKeyFrame* GetCurrentKey(float time);
	void UpdateTrack(float time);
	void SetNode(Node3D* node);
	Node3D* GetNode();
	std::vector<TrackKeyFrame*> GetKeys();
	void Write(VFile* file);
	void Read(VFile* file);
	std::string GetNodeName() {
		return mNodeName;
	}
	bool GetSolo();
	void SetSolo(bool solo);
private:

	Node3D* mNode;
	std::string mNodeName;
	std::vector<TrackKeyFrame*> mKeyFrames;
	TrackKeyFrame* mCurrentKey = nullptr;
	bool mRecordSolo = false;
};

