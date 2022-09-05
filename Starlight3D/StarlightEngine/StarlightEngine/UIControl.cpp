#include "pch.h"
#include "UIControl.h"


UIControl::UIControl() {

	Controls.resize(0);
	actionPtr = NULL;
	actOne = NULL;
	actDrag = NULL;
	X = 0;
	Y = 0;
	W = 0;
	H = 0;
	Text = "";
	Root = NULL;
	OffX = 0;
	OffY = 0;
};

UIControl* UIControl::Set(int x, int y, int w, int h) {

	X = x;
	Y = y;
	W = w;
	H = h;
	//exit(0);

	Resized();
	AfterSet();
	return this;
}


UIControl* UIControl::SetText(const char* text) {


	StdText = std::string(text);
	Text = StdText.c_str();

	return this;

}

void UIControl::AddControl(UIControl* control) {

	Controls.push_back(control);
	control->SetRoot(this);

}

int UIControl::ControlsCount() {

	return Controls.size();

};

UIControl* UIControl::GetControl(int index) {

	return Controls[index];


}

void UIControl::SetRoot(UIControl* root) {

	Root = root;

}

int UIControl::GetX() {

	if (OverX != -1) return OverX;
	int rx = 0;

	if (Root != NULL) {

		rx = Root->GetX();
	
	}

	return (rx + X + OffX);

};

int UIControl::GetY() {

	if (OverY != -1) return OverY;
	int ry = 0;

	if (Root != NULL) {

		ry = Root->GetY();

	}

	return (ry + Y + OffY);

};

int UIControl::GetW() {

	return W;

};

int UIControl::GetInternalX() {

	return X;

}

int UIControl::GetInternalY() {

	return Y;

}

int UIControl::GetH() {

	return H;


};

int UIControl::GetContentsW() {

	int cw = 0;
	return 0;
}

int UIControl::GetContentsH() {

	int ch = 0;

	int my = 0;

	for (int i = 0;i < Controls.size();i++) {

		auto c = Controls[i];

		int cy = c->GetInternalY() + c->GetH();
		if (cy >= my)
		{
			my = cy;
		}

	}

	return my;

}

const char* UIControl::GetText() {

	return Text;

};

bool UIControl::InBounds(int x, int y) {

	if (x >=GetX() && y >= GetY() && x <= (GetX()+GetW()) && y <= (GetY()+GetH()))
	{
		return true;
	}
	return false;
};



void UIControl::Action() {

	if (act != NULL) {
		act();
	}

	if (actionPtr != NULL) {
		actionPtr();
	}
	if (actOne != NULL) {
		actOne();
	}
};

	Texture2D* UIControl::GrabBackground() {

	if (BGTex == nullptr) {

	//	BGTex = new Kinetic::Textures::Texture2D(GetW(), GetH());

	}

	//BGTex->Copy(GetX(),VirtualWarApp::rHeight-GetY()-GetH());
	return BGTex;


}