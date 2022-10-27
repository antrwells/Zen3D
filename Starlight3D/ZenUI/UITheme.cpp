#include "pch.h"
#include "UITheme.h"
#include "Texture2D.h"
#include "kFont.h"

UITheme::UITheme(std::string path) {

	Theme = this;

	std::string rpath = "UI/Theme/" + path + "/";

	std::string frame = rpath + "FrameSquare.png";
	std::string button = rpath + "ButtonFrame.png";
	std::string win_frame = rpath + "WindowFrame.png";
	std::string win_title = rpath + "WindowTitle.png";
	std::string win_main = rpath + "WindowMain.png";
	std::string scrollbar = rpath + "ScrollBar.png";

	mFrame = new Texture2D(frame.c_str());
	mButtonFrame = new Texture2D(button.c_str());
	mWindowFrame = new Texture2D(win_frame.c_str());
	mWindowTitle = new Texture2D(win_title.c_str());
	mWindowMain = new Texture2D(win_main.c_str());
	mScrollBar = new Texture2D(scrollbar.c_str());
	mWhite = new Texture2D("UI/White.png");

	std::string font = rpath + "mainfont.pf";

	mMainFont = new kFont(font.c_str());

}

UITheme* UITheme::Theme = nullptr;

