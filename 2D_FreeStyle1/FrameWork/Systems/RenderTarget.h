#pragma once
//카메라가 촬영한 내용을 -> 화면 출력 
//카메라가 촬영한 내용을 -> 이미지 파일 
//미니맵 만들때 
class RenderTarget
{
public:
	RenderTarget(UINT wdith = 0, UINT height = 0);
	~RenderTarget();

	void Set(D3DXCOLOR clear = 0xFF000000);

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	void SaveRtvTexture(wstring saveFile);

	ID3D11RenderTargetView* GetRTV(){ return rtv; }
private:
	UINT wdith, height;
	ID3D11ShaderResourceView* srv; //셰이더에 데이터를 넘길
	ID3D11Texture2D* rtvTexture; //이미지 파일의 색상정보 
	ID3D11RenderTargetView* rtv; //도형을 촬영한 내용
};

