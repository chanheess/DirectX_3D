#pragma once
class Widget
{
public:
	Widget();
	virtual ~Widget();

	virtual void Render() = 0;
	virtual void Begin();
	virtual void End();

	bool Visible() { return bVisible; }
	void Visible(bool val) { bVisible = val; }

protected:
	string title;

	int windowFlags;
	bool bVisible;
};

