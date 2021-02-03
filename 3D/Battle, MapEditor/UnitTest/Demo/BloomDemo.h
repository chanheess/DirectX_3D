#pragma once
#include "Systems/IExecute.h"

class BloomDemo : public IExecute
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
	void SetBlur();

	void GetBlurParameter(vector<float>& weights, vector<Vector2>& offsets, float x, float y);
	float GetGaussFunction(float val);

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

	RenderTarget* renderTarget[8];
	DepthStencil* depthStencil;
	Viewport* viewport;

	float threshold = 0.9f;


	UINT blurCount = 15;
	float intensity = 0.5f;

	vector<float> weightX;
	vector<Vector2> offsetX;

	vector<float> weightY;
	vector<Vector2> offsetY;

	PostEffect* postEffect;
	Render2D* render2D;

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