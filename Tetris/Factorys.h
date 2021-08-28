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
	ID2D1Factory* GetFactory();
	IWICImagingFactory* GetWICFactory();
private:
	HRESULT Init();

private:
	static Factorys* mInstance;
	ID2D1Factory* mpD2DFactory;
	IWICImagingFactory* mpWICFactory;
};