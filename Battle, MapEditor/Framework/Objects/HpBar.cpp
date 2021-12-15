#include "Framework.h"
#include "HpBar.h"

HpBar::HpBar(float hp, bool player)
	: fullHp(hp), currHp(hp), bPlayer(player)
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
	hpBar->Update();
	hpBar->UpdateTransforms();

	if (bPlayer == true)
		hpBar2d->Update();
}

void HpBar::Render()
{
	hpBar->Render();

	if (bPlayer == true)
		hpBar2d->Render();
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
	hpBar->GetTransform(0)->Position(pos.x, pos.y, pos.z);

	{
		Vector3 rotation;
		Context::Get()->GetCamera()->RotationDegree(&rotation);
		hpBar->GetTransform(0)->RotationDegree(rotation);
	}
}

void HpBar::Dameged(float damege)
{
	//damege
	currHp -= damege;

	//ratioHp
	float ratioHp = currHp / fullHp;
	shader->AsScalar("ratioHp")->SetFloat(ratioHp);

	hpBar->Update();
	hpBar->UpdateTransforms();

	if (bPlayer == true)
	{
		hpBar2d->GetShader()->AsScalar("ratioHp")->SetFloat(ratioHp);

		hpBar2d->Update();
	}
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

		//ratioHp
		float ratioHp = currHp / fullHp;
		shader->AsScalar("ratioHp")->SetFloat(ratioHp);
	}

	//hpBar2D
	if (bPlayer == true)
	{
		renderTarget = new RenderTarget();

		hpBar2d = new Render2D(L"P_HpBar.fxo");
		hpBar2d->GetTransform()->Position(650, 50, 0);
		hpBar2d->GetTransform()->Scale(200, 30, 1);
		hpBar2d->SRV(renderTarget->SRV());
		hpBar2d->Pass(1);

		//ratioHp
		float ratioHp = currHp / fullHp;
		hpBar2d->GetShader()->AsScalar("ratioHp")->SetFloat(ratioHp);
	}
}
