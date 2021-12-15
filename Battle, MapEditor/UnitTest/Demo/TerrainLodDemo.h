#pragma once
#include "Systems/IExecute.h"
#define ModelNumCount 3

class TerrainLodDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

public:
	void Billboarding();

	void Models();
	void ModelCreate();
	void ModelSave();
	void ModelLoad();

	void Waters();
	void WaterCreate();
	void WaterSave(wstring savePath);
	void WaterLoad(wstring savePath);

private:
	Shader* shader;

	TerrainLod* terrain;


private:	//Model
	Shader* modelShader;
	vector<ModelRender *> models;	//¸ðµ¨³Ö±â
	UINT modelCount[ModelNumCount]{0};
	UINT ModelSaveLoadNum = 0;

private:	//Biilboard
	Shader* billboardShader;
	Billboard* billboard;
	bool startBillboard;
	bool billOn = false;

	UINT treeType;
	UINT count;
	bool erase;

private:	//Water
	Shader* waterShader;
	vector<Water* > water;
	UINT waterCount = 0;
	UINT WaterSaveLoadNum = 0;
};