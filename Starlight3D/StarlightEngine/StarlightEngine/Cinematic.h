#pragma once
#include <string>
#include <vector>
#include "CineTrack.h"

class SceneGraph;

class Cinematic
{
public:

	Cinematic();
	void InsertTrack(CineTrack* track);
	void SetTime(float t1, float t2);
	std::string GetName();
	void SetName(std::string name);
	std::vector<CineTrack*> GetTracks();
	void SetTime(float time);
	void Write(VFile* file);
	void Read(VFile* file,SceneGraph* graph);
	float GetLength();
	void SetLength(float time);
	void Play();
	void SetSpeed(float spd);
	float GetSpeed();
	void Update();
private:

	float mTime = 0.0f;
	float mEnd = 0.0f;
	bool mPlaying = false;
	float mSpeed = 0.01f;
	
	std::string mName = "New Anim";
	std::vector<CineTrack*> mTracks;

};

