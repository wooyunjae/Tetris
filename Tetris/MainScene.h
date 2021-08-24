#pragma once
#include "Scene.h"

class MainScene :
    public Scene
{
public:
	MainScene(HWND hwnd, ID2D1Factory* D2DFactory, IWICImagingFactory* wicFactory);
	virtual ~MainScene();

	HRESULT Initialize() override;
	void Update() override;
	void Render() override;

	void SetWICFactory(IWICImagingFactory* wicFactory);
private:
	IWICImagingFactory* mpWICFactory;
	ID2D1Bitmap* mpMainScreen;
};

