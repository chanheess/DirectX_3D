#pragma once
#include "Systems/IExecute.h"

class TerrainEditor : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

public:
	void Billboarding();

private:
	CubeSky* sky;

	Shader* shader;
	class Terrain* terrain;

private:	//Biilboard
	Shader* billboardShader;
	Billboard* billboard;
	bool startBillboard;
	bool billOn = false;

	UINT treeType;
	UINT count;
	bool erase;
};