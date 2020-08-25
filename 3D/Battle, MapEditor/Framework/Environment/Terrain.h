#pragma once

class Terrain : public Renderer
{
public:
	typedef VertexTextureNormalTangent TerrainVertex;

public:
	Terrain(Shader* shader, wstring heightFile);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void LayerMap(wstring file, wstring alphaFile);

	float GetHeight(Vector3& position);
	float GetHeightPick(Vector3& position);
	Vector3 GetPickedPosition();

	void RaiseHeight(Vector3& position, UINT type, UINT type2, UINT range, float power);
	void MiddleHeight(Vector3& position, UINT type, UINT range, bool &fixed, float power);
	void SlopeHeight(Vector3& position, UINT type, UINT range, UINT range2, UINT direction, float power);

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void CreateTriangleData();

private:
	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);
		Vector3 Location;
		UINT Type = 0;
		UINT HeightType = 0;
		UINT SlopeDirection = 0;
		UINT Range = 1;
		UINT Range2 = 1;
	} brushDesc;

	struct LineDesc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		UINT Visible = 0;
		float Thickness = 0.01f;
		float Size = 5.0f;
		float Padding;
	} lineDesc;


	//=======================csPicking==================================
	struct InputDesc	//쉐이더의 input
	{
		UINT Index;
		Vector3 V0, V1, V2;
	};

	struct OutputDesc	//쉐이더의 Output
	{
		UINT Picked;
		float U;
		float V;
		float Distance;
	};

	struct Ray
	{
		Vector3 Position;
		float Padding;

		Vector3 Direction;
		float Padding2;
	} ray;
	//=========================================================

private:
	Texture* heightMap;

	UINT width;
	UINT height;
	
	TerrainVertex* vertices;	
	UINT* indices;

	Texture* baseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	Texture* layerMap = NULL;
	ID3DX11EffectShaderResourceVariable* sLayerMap;

	Texture* alphaMap = NULL;
	ID3DX11EffectShaderResourceVariable* sAlphaMap;

	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;

	ConstantBuffer* lineBuffer;
	ID3DX11EffectConstantBuffer* sLineBuffer;

private:	//csPicking
	InputDesc* input;
	OutputDesc* output;
	Shader* csShader;

	UINT size;

	StructuredBuffer* csbuffer;
private:
	bool fixed = false;	//평탄화의 Y값의 고정을 위해

};