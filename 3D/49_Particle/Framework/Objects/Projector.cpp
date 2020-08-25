#include "Framework.h"
#include "Projector.h"
#include "../Viewer/Fixity.h"

Projector::Projector(Shader * shader, wstring mapFile, UINT width, UINT height, UINT index)
	: shader(shader), width(width), height(height), index(index)
{
	camera = new Fixity();
	//camera->Position(0, 0, -20.0f);
	//projection = new Perspective((float)width, (float)height, 1, 100, Math::PI * 0.25f);	//원근투영 100far 45도

	camera->RotationDegree(90, 0, 0);
	camera->Position(0, 30, 0);
	projection = new Orthographic((float)width, (float)height);

	map = new Texture(mapFile);
	if (index == 0)
	{
		buffer[0] = new ConstantBuffer(&desc, sizeof(Desc));
		sBuffer[0] = shader->AsConstantBuffer("CB_Projector");
	}
	else if (index == 1)
	{
		buffer[1] = new ConstantBuffer(&desc, sizeof(Desc));
		sBuffer[1] = shader->AsConstantBuffer("CB_Projector2");
	}


	sMap = shader->AsSRV("ProjectorMap");
	sMap->SetResource(map->SRV());

}

Projector::~Projector()
{
	SafeDelete(camera);
	SafeDelete(projection);

	SafeDelete(map);
	if (index == 0)
	{
		SafeDelete(buffer[0]);
	}
	else if (index == 1)
	{
		SafeDelete(buffer[1]);
	}
}

void Projector::Update()
{
	Vector3 position;
	camera->Position(&position);

	ImGui::SliderFloat3("Position", position, -100, 100);
	camera->Position(position);

	ImGui::ColorEdit3("Color", desc.Color);

	//Perspective
	{
		/*static float width = this->width, height = this->height;
		static float n = 1.0f, f = 100.0f;
		static float fov = 0.25f;

		ImGui::SliderFloat("Width", &width, 0, 100);
		ImGui::SliderFloat("Height", &height, 0, 100);
		ImGui::SliderFloat("Near", &n, 0, 200);
		ImGui::SliderFloat("Far", &f, 0, 200);
		ImGui::SliderFloat("Fov", &fov, 0, Math::PI * 2.0f);

		((Perspective *)projection)->Set(width, height, n, f, Math::PI * fov);*/
	}

	//Orthographic
	{
		static float width = this->width, height = this->height;
		static float n = 1.0f, f = 100.0f;

		ImGui::SliderFloat("Width", &width, 0, 100);
		ImGui::SliderFloat("Height", &height, 0, 100);
		ImGui::SliderFloat("Near", &n, 0, 200);
		ImGui::SliderFloat("Far", &f, 0, 200);
		n = position.y - 0.1f;
		((Orthographic *)projection)->Set(width, height, n, f);
	}

	camera->GetMatrix(&desc.View);
	projection->GetMatrix(&desc.Projection);
}

void Projector::Render()
{
	if (index == 0)
	{
		buffer[0]->Apply();
		sBuffer[0]->SetConstantBuffer(buffer[0]->Buffer());
	}
	else if (index == 1)
	{
		buffer[1]->Apply();
		sBuffer[1]->SetConstantBuffer(buffer[1]->Buffer());
	}
}

