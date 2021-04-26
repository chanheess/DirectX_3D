#pragma once
//1개만 만들겠다.(싱글톤)

struct D2DDesc
{
	wstring AppName; //타이틀
	HWND Handle;	 //창번호
	float Width;	 //창너비
	float Height;	 //창높이
	bool bFullScreen;//전체화면 
	bool bVsync;	 //수직동기화
};
class D2D
{
public:
	static D2D* Get();
	static void Delete();

	void SetGpuInfo();
	//Device, DeviceContext, SwapChain 
	void CreateDeviceAndSwapChain();
	//BackBuffer
	void CreateBackBuffer();
	//viewport(창크기)
	void CreateViewport();
	//후면버퍼 내용삭제 
	void Clear(
		D3DXCOLOR color = 0xFF808080,
		ID3D11RenderTargetView* rtv = NULL);
	//후면 버퍼 내용 -> 전방버퍼 
	void Present();

	static void SetDesc(D2DDesc& val);
	static void GetDesc(D2DDesc* val);
	static ID3D11Device* GetDevice() { return device; };
	static ID3D11DeviceContext* GetDC() { return deviceContext; }
	IDXGISwapChain* GetSwapChain() { return swapChain; }

private:
	D2D();
	~D2D();

	static D2D* instance;
private:
	static D2DDesc desc;
	static ID3D11Device * device; //장치 생성시 필요한 변수
	static ID3D11DeviceContext* deviceContext; //랜더링에 필요한 변수 

	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* RTV; //전방버퍼 

	UINT numerator;
	UINT denominator;

	UINT gpuMemorySize;
	wstring gpuDescription;

};

#define D2DCLASS() D2D::Get()
#define DEVICE() D2DCLASS()->GetDeivce()
#define DEVICECONTEXT() D2DCLASS()->GetDC()