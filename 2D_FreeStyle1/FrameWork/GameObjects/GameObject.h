#pragma once
//GameObject -> 씬에 배치된 도형
//Object-> 씬에 배치된 도형이 모아진 녀석
//Component-> 기능 
enum class Tag
{
	Empty = 0,
	UserInterface,
};

class GameObject
{
public:
	GameObject(wstring name = L"GameObject");	//이름을 그냥지정해줌
	~GameObject();

	virtual void Ready();
	virtual void Update();
	virtual void Render();
	virtual void Insfactor();

	void Hierarchy();
	void AddComponent(class Component* comp);

	template<typename T>
	T* GetComponent()
	{
		if (components.count(typeid(T).name()) > 0)
			return dynamic_cast<T*>(components[typeid(T).name()]);

		return NULL;
	}
	class Transform* transform;
protected:
	wstring name;
	Tag tag;

	typedef pair<string, class Component*> Pair;
	map<string, class Component*> components;
	class Sprite* sprite;
	bool bActive;
private:
	class Gizmo* gizmo;
};
