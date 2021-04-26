#pragma once
class Buffer
{
private:
	friend class Buffers;
public:
	Buffer(void* data, UINT byteWidth);
	virtual ~Buffer();

	virtual void Update();
	virtual void Render();

	void Change();

	void Slot(UINT n) { slot = n; }
	void SetData(void* data);
private:
	void CreateBuffer();
protected:
	ID3D11Buffer * buffer;
	D3D11_BUFFER_DESC desc;

	void* data; 
	UINT byteWidth; //Å©±â 

	UINT slot;
	bool bChange;
};


class Buffers
{
public:
	friend class Buffer;
public:
	static void Add(Buffer* buffer);
	static void Update();
	static void Delete();
private:
	static vector<class Buffer*> buffers;
};