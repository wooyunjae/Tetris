#pragma once
#include "Scene.h"

class Button;
class SoundPlayer;

class MainScene :
	public Scene
{
public:
	MainScene(HWND hwnd, HINSTANCE hInstance);
	virtual ~MainScene();

	HRESULT Initialize(HINSTANCE hInstance) override;
	void Update() override;
	void Render() override;

private:
	ID2D1Bitmap* mpMainScreen;
	HINSTANCE mhInstance;
	Button* mpStartButton;
	Button* mpCloseButton;
	SoundPlayer* mpPlayer;
};

