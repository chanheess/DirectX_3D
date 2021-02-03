#pragma once

class HpBar
{
public:
	HpBar(float hp, bool player);	//player : true / monster : false
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

	MeshRender* hpBar;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Render2D* hpBar2d;

	float fullHp;
	float currHp;
	float damegedHpPos;

	bool bPlayer;	//player : true / monster : false
};