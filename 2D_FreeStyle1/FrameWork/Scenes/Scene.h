#pragma once

class Scene
{
public:
	Scene();
	~Scene();
	
	virtual void Ready() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void Scene_Ready();
	void Scene_Update();
	void Scene_Render();

protected:
	static ID3D11BlendState * alphBlend;
	static ID3D11RasterizerState* cullmode;

	static UINT refCount;
	class GameObject* root;
	class Transform* rootTransform;

	class CameraObj* mainCameraObj;
	class Camera* mainCamera;
};
