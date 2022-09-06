#pragma once
#include <string>

enum AnimType {
	Forward,Backward,PingPong,Once
};

class ActorAnim
{
public:
	
	float mStartTime;
	float mEndTime;
	float mCurTime = 0.0f;
	float mSpeed = 0.0;
	std::string mName;
	AnimType mType;

	ActorAnim(std::string name, float start, float end,float speed,AnimType type)
	{
		mType = type;
		mStartTime = start;
		mEndTime = end;
		mName = name;
		mSpeed = speed;
	}

	void Begin() {
		mCurTime = mStartTime;
	}

	void Update() {

		mCurTime = mCurTime + mSpeed;
		if (mCurTime >= mEndTime) {

			switch (mType) {

			case AnimType::Forward:

				mCurTime = mStartTime;
			
				break;
			case AnimType::Once:

				mCurTime = mEndTime;

				break;
			}

		}

	}

};

