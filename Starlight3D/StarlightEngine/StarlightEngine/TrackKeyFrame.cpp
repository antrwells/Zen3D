#include "pch.h"
#include "TrackKeyFrame.h"
#include "Helper.h"
#include "VFile.h"
#include "NodeActor.h"
TrackKeyFrame::TrackKeyFrame() {

	mTime =0;
	mPosition = float3(0, 0, 0);
	mScale = float3(0, 0, 0);
	mRotation = float3x3::Identity();

}

void TrackKeyFrame::SetPosition(float3 position) {

	mPosition = position;

}

void TrackKeyFrame::SetScale(float3 scale) {

	mScale = scale;

}

void TrackKeyFrame::SetRotation(float3x3 rotation) {

	mRotation = rotation;

}

float3 TrackKeyFrame::GetPosition() {

	return mPosition;

}

float3 TrackKeyFrame::GetScale() {

	return mScale;

}

float3x3 TrackKeyFrame::GetRotation() {

	return mRotation;

}

void TrackKeyFrame::SetTime(float time) {

	mTime = time;

}

float TrackKeyFrame::GetTime() {

	return mTime;

}


TrackKeyFrame* TrackKeyFrame::GetCurrentKey(TrackKeyFrame* right, float time,bool oneToZero)
{
	float3 rPos;
	float3 rScale;
	float4x4 rRot;

	if (oneToZero)
	{
	
		rPos = Diligent::lerp(mPosition, right->GetPosition(), time);
		rScale = Diligent::lerp(mScale, right->GetScale(), time);
		
		Quaternion q1 = Helper::MatrixToQuat(mRotation);
		Quaternion q2 = Helper::MatrixToQuat(right->GetRotation());

		Quaternion q = Diligent::slerp(q1, q2, time);
		
		rRot = q.ToMatrix();

		float3x3 mat;

		mat[0][0] = rRot[0][0];
		mat[0][1] = rRot[0][1];
		mat[0][2] = rRot[0][2];

		mat[1][0] = rRot[1][0];
		mat[1][1] = rRot[1][1];
		mat[1][2] = rRot[1][2];

		mat[2][0] = rRot[2][0];
		mat[2][1] = rRot[2][1];
		mat[2][2] = rRot[2][2];

		auto new_key = new TrackKeyFrame;

		new_key->SetPosition(rPos);
		new_key->SetScale(rScale);
		new_key->SetRotation(mat);

		return new_key;


	}
	else {


		float atime = time - mTime;
		float btime = right->GetTime() - mTime;

		float ctime = atime / btime;

		if (ctime > 1.0) {

			printf("Time Error:%f\n", ctime);
			ctime = 1.0;

		}
		if (ctime < 0.0) {
			ctime = 0.0;
		}

		rPos = Diligent::lerp(mPosition, right->GetPosition(), ctime);
		rScale = Diligent::lerp(mScale, right->GetScale(), ctime);

		Quaternion q1 = Helper::MatrixToQuat(mRotation);
		Quaternion q2 = Helper::MatrixToQuat(right->GetRotation());

		Quaternion q = Diligent::slerp(q1, q2, ctime);

		rRot = q.ToMatrix();

		float3x3 mat;

		mat[0][0] = rRot[0][0];
		mat[0][1] = rRot[0][1];
		mat[0][2] = rRot[0][2];

		mat[1][0] = rRot[1][0];
		mat[1][1] = rRot[1][1];
		mat[1][2] = rRot[1][2];

		mat[2][0] = rRot[2][0];
		mat[2][1] = rRot[2][1];
		mat[2][2] = rRot[2][2];

		auto new_key = new TrackKeyFrame;

		new_key->SetPosition(rPos);
		new_key->SetScale(rScale);
		new_key->SetRotation(mat);

		return new_key;

	}

	return nullptr;

}


void TrackKeyFrame::Write(VFile* file) {

	file->WriteFloat(mTime);
	file->WriteVec3(mPosition);
	file->WriteVec3(mScale);
	file->WriteMatrix3x3(mRotation);
	file->WriteBool(mPlayAnim);
	file->WriteString(mAnimName.c_str());
}

void TrackKeyFrame::Read(VFile* file) {

	mTime = file->ReadFloat();
	mPosition = file->ReadVec3();
	mScale = file->ReadVec3();
	mRotation = file->ReadMatrix3x3();
	mPlayAnim = file->ReadBool();
	mAnimName = std::string(file->ReadString());
}

bool TrackKeyFrame::GetPlayAnim() {

	return mPlayAnim;

}

void TrackKeyFrame::SetPlayAnim(bool play) {

	mPlayAnim = play;

}

std::string TrackKeyFrame::GetAnimName() {

	return mAnimName;

}

void TrackKeyFrame::SetAnimName(std::string name) {

	mAnimName = name;

}

void TrackKeyFrame::Activate()
{

	if (mNode->GetType() == NodeType::Actor) {

		auto act = (NodeActor*)mNode;

		if (mPlayAnim)
		{

			act->PlayAnim(mAnimName);
			printf("Playing Animation!\n");

		}

	}

}

void TrackKeyFrame::Deactivate() {


}

void TrackKeyFrame::SetNode(Node3D* node)
{

	mNode = node;

}