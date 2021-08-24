#pragma once
#include "GraphicsAPIHeader.h"
namespace Bitmap
{
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);
}