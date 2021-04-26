#pragma once
#include <iostream>
#include <windows.h>
template <class T, int ALLOC_BLOCK_SIZE = 50 >
class CMemeoryPool
{
public:
	//생성한 주소를 반환해주는 오퍼레이터 함수
	static VOID* operator new(std::size_t allocLength)
	{
		//다음 주소가 없다면
		if (!mFreePointer)
			//지정해놓은 크기만큼 풀 잡아주기
			AllocBlock();
		UCHAR* returnPointer = mFreePointer;
		//다음 주소로 넘어가기
		mFreePointer = *reinterpret_cast<UCHAR**>(returnPointer);
		return returnPointer;
	}
	//삭제 오퍼레이터 함수
	static VOID operator delete(VOID* deletePointer)
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		mFreePointer = reinterpret_cast<UCHAR*>(deletePointer);
	}
private:
	//블록만큼 다시 크기를 잡는다
	static VOID AllocBlock()
	{
		//생성할 주소는 (새로 크기를 다시 잡는다)
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		UCHAR** current = reinterpret_cast<UCHAR**>(mFreePointer);
		UCHAR* next = mFreePointer;
		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			next += sizeof(T);
			*current = next;
			current = reinterpret_cast<UCHAR**>(next);
		}
		*current = 0;
	}

private:
	static UCHAR* mFreePointer;
protected:
	~CMemeoryPool()

	{

	}

};

template <class T, int ALLOC_BLOCK_SIZE>
//삭제할 주소를 담고있는 포인터
UCHAR* CMemeoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;