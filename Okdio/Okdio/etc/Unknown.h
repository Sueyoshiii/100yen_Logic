#pragma once
#include "../Okdio.h"
#include <unknwn.h>

class CUnknown :
	public IUnknown
{
public:
	// コンストラクタ
	CUnknown() : ref(1) {}
	// デストラクタ
	~CUnknown() {}

	// 参照カウント加算
	unsigned long __stdcall AddRef(void)
	{
		return InterlockedIncrement(&ref);
	}

	// 参照カウント減少
	unsigned long __stdcall Release(void)
	{
		unsigned long hr = InterlockedDecrement(&ref);
		if (hr == 0)
		{
			delete this;
		}

		return hr;
	}

	// サポートチェック
	long __stdcall QueryInterface(const GUID& id, void** obj)
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}

		*obj = nullptr;
		if (id == IID_IUnknown || id == __uuidof(Okdio))
		{
			*obj = (void*)this;
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

private:
	// 参照カウント
	unsigned long ref;
};
