#include "Framework.h"
#include "HpBar.h"

HpBar::HpBar(float hp)
	: fullHp(hp), currHp(hp)
{
	shader = new Shader(L"P_HpBar.fxo");

	CreateHp();
	damegedHpPos = 0;
}

HpBar::~HpBar()
{
}

void HpBar::Update()
{
	hpBackground->Update();
	hpBackground->UpdateTransforms();

	hpBar->Update();
	hpBar->UpdateTransforms();
}

void HpBar::Render()
{
	hpBackground->Render();
	hpBar->Render();
}

void HpBar::HpTransform(Transform* transform)
{
	Vector3 pos;
	transform->Position(&pos);

	Vector3 scale;
	transform->Scale(&scale);
	Vector3 scale2;
	hpBar->GetTransform(0)->Scale(&scale2);

	pos.y = pos.y + scale.y * 200.0f + scale2.y * 0.5f;
	hpBar->GetTransform(0)->Position(pos.x - damegedHpPos, pos.y, pos.z);

	pos.z = pos.z + 0.03f;
	hpBackground->GetTransform(0)->Position(pos);


	{
		/*int rot = (int)rotation.y;
		rot = abs(rot);
		rot %= 360;
		if (rot >= 90 && rot <= 270)
		{
			pos.z = pos.z - 0.1f;
			ImGui::InputFloat3("sssss", pos);

		}
		else
		{
			pos.z = pos.z + 0.1f;
			ImGui::InputFloat3("sss", pos);

		}*/
		Vector3 rotation;
		Context::Get()->GetCamera()->RotationDegree(&rotation);
		hpBar->GetTransform(0)->RotationDegree(rotation);
		hpBackground->GetTransform(0)->RotationDegree(rotation);
	}
}

void HpBar::Dameged(float damege)
{
	Vector3 scale;
	hpBar->GetTransform(0)->Scale(&scale);

	//damege
	currHp -= damege;

	//scale
	float ratioHpScale = currHp / fullHp;
	scale.x = 5.0f * ratioHpScale;

	//pos
	float totalDamege = fullHp - currHp;
	float ratioHpPos = 5.0f * (totalDamege / fullHp);
	damegedHpPos = ratioHpPos * 0.5f;

	hpBar->GetTransform(0)->Scale(scale);

	hpBar->Update();
	hpBar->UpdateTransforms();
}

void HpBar::CreateHp()
{
	Transform* transform = NULL;

	//hpBar
	{
		hpBar = new MeshRender(shader, new MeshQuad());
		transform = hpBar->AddTransform();
		transform->Position(0, 0, 0);
		transform->Scale(5, 1, 1);
		hpBar->UpdateTransforms();
		hpBar->Pass(0);
	}

	//hpBackground
	{
		hpBackground = new MeshRender(shader, new MeshQuad());
		transform = hpBackground->AddTransform();
		transform->Position(0, 0, 0);
		transform->Scale(5, 1, 1);
		hpBackground->UpdateTransforms();
		hpBackground->Pass(1);
	}
}
