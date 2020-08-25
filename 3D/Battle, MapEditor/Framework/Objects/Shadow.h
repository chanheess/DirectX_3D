#pragma once

class Shadow
{
public:
	Shadow(Shader* shader, Vector3& position, float radius, UINT width = 1024, UINT height = 1024);
	~Shadow();

	void PreRender();

	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }	//리턴받아서 2d로 랜더 시킴

private:
	void CalcViewProjection();

private:
	struct Desc
	{
		Matrix View;
		Matrix Projection;

		Vector2 MapSize;
		float Bias = -0.0006f;

		UINT Quality = 2;
	} desc;

private:
	Shader* shader;
	UINT width, height;	//그림자 텍스쳐의 크기

	//이 두개로 직육면체안에 그림자를 비춰준다.
	Vector3 position;	//그림자를 최종적으로 받을 중심점
	float radius;	//그림자가 비춰질 반경

	RenderTarget* renderTarget;	//시야를 보기위해서 넣은것
	DepthStencil* depthStencil;
	Viewport* viewport;	//라이트 영역의 뷰포트

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sShadowMap;

	ID3D11SamplerState* ss;
	ID3DX11EffectSamplerVariable* sSS;
};