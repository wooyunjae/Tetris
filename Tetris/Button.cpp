#include <assert.h>

#include "Button.h"
#include "Macro.h"
#include "Factorys.h"
#include "Bitmap.h"

Button::Button(HWND parent, HINSTANCE hInstance, std::wstring name, RECT rect)
	: mParent(parent)
	, mHwnd(nullptr)
	, mName(name)
	, mpImage(nullptr)
	, mpRenderTarget(nullptr)
	, mRect(rect)
	, meState(eState::IDLE)
	, isClicked(false)
	, mAction(nullptr)
{
	WNDCLASSEX wcex = { sizeof(wcex) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hCursor = NULL;
	wcex.lpszClassName = mName.c_str();
	RegisterClassEx(&wcex);

	mHwnd = CreateWindow(
		mName.c_str(), L"Button",
		WS_CHILD,
		mRect.left, mRect.top, mRect.right - mRect.left, mRect.bottom - mRect.top,
		parent, NULL, hInstance, this
	);
	if (SUCCEEDED(mHwnd ? S_OK : E_FAIL))
	{
		ShowWindow(mHwnd, SW_SHOWNORMAL);
		UpdateWindow(mHwnd);

		D2D1_SIZE_U size = D2D1::SizeU(mRect.right - mRect.left, mRect.bottom - mRect.top);
		FACTORY->GetFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&mpRenderTarget
		);
	}
}

Button::~Button()
{
	SAFE_RELEASE(mpImage);
	SAFE_RELEASE(mpRenderTarget);
	DestroyWindow(mHwnd);
}


void Button::Render()
{
	D2D1_SIZE_F size = mpImage->GetSize();
	D2D1_RECT_F dst = { 0, 0, size.width, size.height / 3 };
	D2D1_RECT_F src;
	mpRenderTarget->BeginDraw();
	switch (meState)
	{
	case eState::IDLE:
		src = { 0, 0, size.width, size.height / 3 * 1 };
		mpRenderTarget->DrawBitmap(
			mpImage,
			dst,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			src
		);
		break;
	case eState::ON_MOUSE:
		src = { 0, size.height / 3 * 1, size.width, size.height / 3 * 2 };
		mpRenderTarget->DrawBitmap(
			mpImage,
			dst,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			src
		);
		break;
	case eState::CLICKED:
		src = { 0, size.height / 3 * 2, size.width, size.height / 3 * 3 };
		mpRenderTarget->DrawBitmap(
			mpImage,
			dst,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			src
		);
		break;
	}
	HRESULT hr = mpRenderTarget->EndDraw();

	if (!SUCCEEDED(hr))
	{
		assert(false);
	}
}

void Button::Update()
{
	POINT ps;
	RECT rt;
	GetCursorPos(&ps);
	GetWindowRect(mHwnd, &rt);
	
	if (rt.left > ps.x
		|| rt.right < ps.x
		|| rt.top > ps.y
		|| rt.bottom < ps.y)
	{
		meState = eState::IDLE;
	}
}

void Button::SetArea(RECT rect)
{
	mRect = rect;
	AdjustWindowRect(&mRect, WS_CHILD, false);
	MoveWindow(mHwnd, mRect.left, mRect.top, mRect.right - mRect.left, mRect.bottom - mRect.top, true);

	if (mpRenderTarget)
	{
		mpRenderTarget->Resize(D2D1::SizeU(mRect.right - mRect.left, mRect.bottom - mRect.top));
	}
}

void Button::SetArea(LONG x, LONG y, LONG width, LONG height)
{
	RECT rt = { x, y, x + width, y + height };
	SetArea(rt);
}


void Button::SetBitmap(PCWSTR uri, int x, int y)
{
	SAFE_RELEASE(mpImage);

	Bitmap::LoadBitmapFromFile(
		mpRenderTarget,
		FACTORY->GetWICFactory(),
		uri,
		x,
		y,
		&mpImage
	);
}


void Button::SetAction(std::function<void()> action)
{
	mAction = action;
}


void Button::ToAction()
{
	mAction();
}


LRESULT Button::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Button* pButton = (Button*)pcs->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pButton));
		result = 1;
	}
	else
	{
		Button* pButton = reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		bool wasHandled = false;
		if (pButton)
		{
			switch (message)
			{
			case WM_LBUTTONDOWN:
				SetCapture(pButton->mHwnd);
				pButton->isClicked = true;
				pButton->meState = eState::CLICKED;
				break;
			case WM_MOUSEMOVE:
			{
				POINT ps;
				RECT rt;
				ps.x = LOWORD(lParam);
				ps.y = HIWORD(lParam);
				GetClientRect(pButton->mHwnd, &rt);
				if (!(rt.left > ps.x
					|| rt.right < ps.x
					|| rt.top > ps.y
					|| rt.bottom < ps.y))
				{
					if (pButton->isClicked)
					{
						pButton->meState = eState::CLICKED;
					}
					else
					{
						pButton->meState = eState::ON_MOUSE;
					}
				}
				break;
			}
			case WM_LBUTTONUP:
			{
				ReleaseCapture();
				POINT ps;
				RECT rt;
				ps.x = LOWORD(lParam);
				ps.y = HIWORD(lParam);
				GetClientRect(pButton->mHwnd, &rt);
				if (!(rt.left > ps.x
					|| rt.right < ps.x
					|| rt.top > ps.y
					|| rt.bottom < ps.y)
					&& pButton->isClicked)
				{
					pButton->mAction();
				}
				pButton->isClicked = false;
				break;
			}
			}
		}
		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return result;
}
