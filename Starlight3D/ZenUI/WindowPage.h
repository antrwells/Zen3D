#pragma once

class UIBase;

struct WindowPage {
	std::string title;
	int id = 0;
	UIBase* content = nullptr;
};
