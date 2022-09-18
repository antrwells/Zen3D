#pragma once
class WindowContent
{
public:
	WindowContent(const char* title) {
		mTitle = title;
	}
	const char* GetTitle() {
		return mTitle;
	}

private:
	const char* mTitle;

};

