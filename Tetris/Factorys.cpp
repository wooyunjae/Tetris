#include <assert.h>

#include "Factorys.h"
#include "Macro.h"
Factorys* Factorys::mInstance = nullptr;

Factorys::Factorys()
	: mpD2DFactory(nullptr)
	, mpWICFactory(nullptr)
{
	assert(SUCCEEDED(Init()));
}

Factorys::~Factorys()
{
	SAFE_RELEASE(mpD2DFactory);
	SAFE_RELEASE(mpWICFactory);
}

Factorys* Factorys::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new Factorys();
	}
	return mInstance;
}

void Factorys::DeleteInstance()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = nullptr;
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
