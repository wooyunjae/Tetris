#pragma once
#include <unordered_map>
#include <Windows.h>

#define KI KeyInput::GetInstance()

enum class eKeySet;
enum class eKeyState;

class KeyInput
{
private:
	KeyInput() = default;
	~KeyInput();
public:
	static KeyInput* GetInstance();
	void Update();
	bool IsKeyDown(int key);
	bool IsKeyUp(int key);

private:
	static KeyInput* mInstance;
	std::unordered_map<eKeySet, eKeyState> mKeys;
};


enum class eKeySet
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,
	SPACE = VK_SPACE,
	P = 0x50,
	R = 0x52,
};

enum class eKeyState
{
	DOWN,
	PRESS,
	UP
};