#pragma once

class Component
{
public:
	Component(wstring name);
	~Component();

	virtual void Ready();
	virtual void Update();
	virtual void Render();
	virtual void Insfactor();

	class GameObject* gameObject;
	void SetGameObject(class GameObject* obj) { gameObject = obj; }
private:
	wstring name;
protected:
	bool bActive;
	bool bChange;
};
