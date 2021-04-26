#pragma once
#include "Systems/Widget.h"
class WMenuBar : public Widget
{
public:
	WMenuBar();
	~WMenuBar();

	void Begin() override;
	void End() override;

	void Render() override;

private:
	bool bMetricWindow;
	bool bDemoWindow;
};

