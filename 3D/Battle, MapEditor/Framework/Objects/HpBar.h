#pragma once

class HpBar
{
public:
	HpBar(float hp);
	~HpBar();

	void Update();
	void Render();

	void HpTransform(Transform* transform);
	void Dameged(float damege);
	float HpState() { return currHp; }

private:
	void CreateHp();

private:
	Shader* shader;

	MeshRender* hpBackground;
	MeshRender* hpBar;

	float fullHp;
	float currHp;
	float damegedHpPos;
};