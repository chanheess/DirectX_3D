#pragma once
#define LayerCount 4

class TerrainLod : public Renderer
{
public:
	struct InitializeDesc;
	struct BrushDesc;
	
public:
	TerrainLod(InitializeDesc& desc, bool modify);
	~TerrainLod();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void LayerMap(wstring layer, UINT number);
	void NormalMap(wstring file);

	Vector3 GetPickedPosition();
	BrushDesc* GetBrushDesc() { return &brushDesc; }

	float GetHeight(Vector3 &position);
	void RaiseHeight(Vector3 &position, UINT type, UINT range, float power);
	void MiddleHeight(Vector3& position, UINT type, UINT range, bool &fixed, float power);
	void SlopeHeight(Vector3& position, UINT type, UINT range, UINT range2, UINT direction, float power);
	void SmoothHeight(Vector3& position, UINT type, UINT range);
	void HeightSave(wstring savePath);
	void HeightLoad(wstring savePath);

	void TextureApply();
	void TextureApply2();
	void Splatting();
	void SplattingApply(Vector3 &position, UINT type, UINT type2, UINT range);
	void SplattingErase(Vector3 &position, UINT type, UINT range);
	void SplattingSave(wstring savePath);
	void SplattingLoad(wstring savePath);

private:
	bool InBounds(UINT x, UINT z);
	void CalcPatchBounds(UINT x, UINT z);
	void CalcBoundY();

	void CreateVertexData();
	void CreateIndexData();

public:
	struct InitializeDesc
	{
		Shader* shader;

		wstring heightMap;
		wstring alphaMap;
		float CellSpacing;
		UINT CellsPerPatch;	//나누는 개수
		float HeightRatio;
	};

private:
	struct BufferDesc
	{
		float MinDistance = 1.0f;
		float MaxDistance = 500.0f;
		float MinTessellation = 1.0f;
		float MaxTessellation = 64.0f;

		float TexelCellSpaceU;
		float TexelCellSpaceV;
		float WorldCellSpace = 1.0f;
		float HeightRatio = 1.0f;

		Plane WorldFrustumPlanes[6];
	} bufferDesc;

private:
	struct VertexTerrain
	{
		Vector3 Position;
		Vector2 Uv;
		Vector2 BoundsY;
	};

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

private:
	UINT faceCount;	//그릴 면의 개수

	UINT vertexPerPatchX;	//패치당 vertex가 몇개 들어가는지
	UINT vertexPerPatchZ;

private:
	UINT fileNum = 0;	//저장 불러오기 파일넘버
	wstring layerName[LayerCount];	//layer들의 이름저장
	bool setLayer[LayerCount]{ false }; //layer타입on
	bool fixed = false;	//평탄화의 Y값의 고정을 위해

	bool modify;	//작업모드, 게임모드

	float* heightColors;
	float* heightColorsTemp;	//스무딩 저장용
	float colorPower = 1.0f;	//컬러의 색강도

	InitializeDesc initDesc;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	UINT width, height;

	VertexTerrain* vertices;
	//VertexTextureNormalID* vertices; //
	UINT* indices;

	Frustum* frustum;
	Camera* camera;
	Perspective* perspective;

	Texture* heightMap;
	//vector<D3DXCOLOR> heightMapPixel;
	ID3DX11EffectShaderResourceVariable* sHeightMap;


	vector<Vector2> bounds;

	Texture* baseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	struct LayerAlphaMap
	{
		Texture* layerMap = NULL;
		ID3DX11EffectShaderResourceVariable* sLayerMap;
	};

	Texture* alphaMap;
	UINT* alphaMapPixel = NULL;
	ID3DX11EffectShaderResourceVariable* sAlphaMap;

	LayerAlphaMap map[5];

	Texture* normalMap = NULL;
	ID3DX11EffectShaderResourceVariable* sNormalMap;

	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;
};