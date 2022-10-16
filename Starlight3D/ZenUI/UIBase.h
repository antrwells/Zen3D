#pragma once
class UIBase
{
public:

	virtual void InitControl();
	virtual void UpdateControl();
	virtual void PreRenderControl();
	virtual void RenderControl();
	virtual void PostRenderControl();

private:

};

