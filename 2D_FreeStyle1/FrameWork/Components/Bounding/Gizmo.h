#pragma once
//ΩÃ±€≈Ê
class Gizmo
{
public:
	static Gizmo & Get();
	static void Delete();

	void Update();
	void Render();
	void DrawLine(ILine* line, D3DXMATRIX world);
private:
	Gizmo();
	~Gizmo();

	static Gizmo* instance;
private:
	vector<Vertex> vertices;
	Shader* shader;
	class VBuffer* vertexBuffer;
	class CBuffer* colorBuffer;
	class CBuffer* worldBuffer;
};

