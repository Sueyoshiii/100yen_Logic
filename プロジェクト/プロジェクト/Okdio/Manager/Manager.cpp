#include "Manager.h"
#include "../Okdio.h"

// コンストラクタ
Manager::Manager()
{
}

// デストラクタ
Manager::~Manager()
{
}

// オブジェクト生成
long Manager::CreateObj(const GUID& id, void** obj)
{
	if (obj == nullptr)
	{
		return E_INVALIDARG;
	}

	*obj = nullptr;
	if (id == __uuidof(Okdio))
	{
		*obj = new Okdio();
		return S_OK;
	}

	return E_NOINTERFACE;
}
