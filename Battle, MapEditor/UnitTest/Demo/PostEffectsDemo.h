#pragma once
#include "Systems/IExecute.h"

class PostEffectsDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

private:
	void OnGUI();
	void OnValueGUI();

private:
	void Mesh();
	void Airplane();
	void Kachujin();

	void Pass(UINT mesh, UINT model, UINT anim);

	void AddPointLights();
	void AddSpotLights();

private:
	Shader* shader;	
	GBuffer* gBuffer;

	RenderTarget* sourceRTV;
	DepthStencil* depthStencil;
	Viewport* viewport;

	vector<wstring> effectList;

	UINT selectIndex = 0;
	typedef pair<wstring, PostEffect *> EffectPair;
	vector<EffectPair> selectList;
	char* selectItems[30];

	Render2D* final2D;
	Render2D* source2D;

	CubeSky* sky;
	Snow* snow;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRender* sphere;
	MeshRender* cylinder;
	MeshRender* cube;
	MeshRender* grid;


	ModelRender* airplane = NULL;

	Model* weapon;
	ModelAnimator* kachujin = NULL;

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};