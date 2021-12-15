#pragma once

class Shadow
{
public:
	Shadow(Shader* shader, Vector3& position, float radius, UINT width = 1024, UINT height = 1024);
	~Shadow();

	void PreRender();

	ID3D11ShaderResourceView* SRV() { return renderTarget->SRV(); }	//���Ϲ޾Ƽ� 2d�� ���� ��Ŵ

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
	UINT width, height;	//�׸��� �ؽ����� ũ��

	//�� �ΰ��� ������ü�ȿ� �׸��ڸ� �����ش�.
	Vector3 position;	//�׸��ڸ� ���������� ���� �߽���
	float radius;	//�׸��ڰ� ������ �ݰ�

	RenderTarget* renderTarget;	//�þ߸� �������ؼ� ������
	DepthStencil* depthStencil;
	Viewport* viewport;	//����Ʈ ������ ����Ʈ

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sShadowMap;

	ID3D11SamplerState* ss;
	ID3DX11EffectSamplerVariable* sSS;
};