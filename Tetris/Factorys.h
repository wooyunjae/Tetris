#pragma once
#include "GraphicsAPIHeader.h"

#define FACTORY (Factorys::GetInstance())

class Factorys
{
private:
	Factorys();
	~Factorys();
public:
	static Factorys* GetInstance();
	static void DeleteInstance();
	HRESULT Init();
	ID2D1Factory* GetFactory();
	IWICImagingFactory* GetWICFactory();
	IDWriteFactory* GetDWriteFactory();

private:
	static Factorys* mpInstance;
	ID2D1Factory* mpD2DFactory;
	IWICImagingFactory* mpWICFactory;
	IDWriteFactory* mpDWriteFactory;
};