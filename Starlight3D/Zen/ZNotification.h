#pragma once
#include <string>
#include <vector>
class ZNotification
{
public:

	ZNotification(std::string title, std::string msg);
	bool Update();
	std::string GetTitle() { return mTitle; }
	std::string GetMsg() { return mMsg; }
	int GetX() { return mX; }
	int GetY() { return mY; }
private:

	std::string mTitle;
	std::string mMsg;
	bool mUp = true;
	bool mWait = false;
	bool mDown = false;
	//ImVec2 Pos;/
	float mX, mY;
};

