#include "stdafx.h"
#include "WMenuBar.h"


WMenuBar::WMenuBar()
	: bMetricWindow(false), bDemoWindow(false)
{
}


WMenuBar::~WMenuBar()
{
}

void WMenuBar::Begin()
{
	ImGui::Begin("Main", 0);
}

void WMenuBar::End()
{
	ImGui::End();
}

void WMenuBar::Render()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Load");
			ImGui::MenuItem("Save");
			ImGui::Separator();
			ImGui::MenuItem("Exit");
		}
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Metric", NULL, &bMetricWindow);
			ImGui::MenuItem("Demo", NULL, &bDemoWindow);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (bMetricWindow == true) ImGui::ShowMetricsWindow();
	if (bDemoWindow == true) ImGui::ShowDemoWindow();

}
