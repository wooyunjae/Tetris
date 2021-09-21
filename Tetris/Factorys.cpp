#include <assert.h>

#include "Factorys.h"
#include "Macro.h"
Factorys* Factorys::mpInstance = nullptr;

Factorys::Factorys()
	: mpD2DFactory(nullptr)
	, mpWICFactory(nullptr)
	, mpDWriteFactory(nullptr)
{
}

Factorys::~Factorys()
{
	SAFE_RELEASE(mpD2DFactory);
	SAFE_RELEASE(mpWICFactory);
	SAFE_RELEASE(mpDWriteFactory);
}

Factorys* Factorys::GetInstance()
{
	if (!mpInstance)
	{
		mpInstance = new Factorys();
	}
	return mpInstance;
}

void Factorys::DeleteInstance()
{
	if (mpInstance)
	{
		delete mpInstance;
		mpInstance = nullptr;
	}
}

HRESULT Factorys::Init()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpD2DFactory);
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mpWICFactory));
	}
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(mpDWriteFactory),
			reinterpret_cast<IUnknown**>(&mpDWriteFactory)
		);
	}
	return hr;
}

ID2D1Factory* Factorys::GetFactory()
{
	return mpD2DFactory;
}

IWICImagingFactory* Factorys::GetWICFactory()
{
	return mpWICFactory;
}

IDWriteFactory* Factorys::GetDWriteFactory()
{
	return mpDWriteFactory;
}
