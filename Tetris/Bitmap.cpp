#include "Bitmap.h"

#include "GraphicsAPIHeader.h"
#include "Macro.h"

namespace Bitmap
{
	// https://docs.microsoft.com/ko-kr/windows/win32/direct2d/how-to-load-a-direct2d-bitmap-from-a-file
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	)
	{
		IWICBitmapDecoder* pDecoder = NULL;
		IWICBitmapFrameDecode* pSource = NULL;
		IWICStream* pStream = NULL;
		IWICFormatConverter* pConverter = NULL;
		IWICBitmapScaler* pScaler = NULL;
		// 비트맵 디코더를 생성
		HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
			uri,							//디코딩 될 이미지
			NULL,							//선호하는 디코더 벤더(?)
			GENERIC_READ,					//읽기 또는 쓰기 (GENERIC_READ,WRITE)
			WICDecodeMetadataCacheOnLoad,	//메타데이터를 언제 캐쉬하느냐?
			&pDecoder						//디코더 변수
		);

		if (SUCCEEDED(hr))
		{
			// Create the initial frame.
			hr = pDecoder->GetFrame(0, &pSource);
		}
		if (SUCCEEDED(hr))
		{
			// Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
			hr = pIWICFactory->CreateFormatConverter(&pConverter);
		}

		if (SUCCEEDED(hr))
		{
			// If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
			if (destinationWidth != 0 || destinationHeight != 0)
			{
				UINT originalWidth, originalHeight;
				hr = pSource->GetSize(&originalWidth, &originalHeight);
				if (SUCCEEDED(hr))
				{
					if (destinationWidth == 0)
					{
						FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
						destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
					}
					else if (destinationHeight == 0)
					{
						FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
						destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
					}

					hr = pIWICFactory->CreateBitmapScaler(&pScaler);
					if (SUCCEEDED(hr))
					{
						hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
					}
					if (SUCCEEDED(hr))
					{
						hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
					}
				}
			}
			else // Don't scale the image.
			{
				hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
			}
		}
		if (SUCCEEDED(hr))
		{
			// Create a Direct2D bitmap from the WIC bitmap.
			hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
		}

		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pSource);
		SAFE_RELEASE(pStream);
		SAFE_RELEASE(pConverter);
		SAFE_RELEASE(pScaler);

		return hr;
	}
}