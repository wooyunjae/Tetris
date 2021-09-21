#include "KeyInput.h"

KeyInput* KeyInput::mpInstance = nullptr;

KeyInput::~KeyInput()
{
	if (mpInstance)
	{
		delete mpInstance;
	}
}

KeyInput* KeyInput::GetInstance()
{
	if (!mpInstance)
	{
		mpInstance = new KeyInput();
		mpInstance->mKeys.insert(std::make_pair(eKeySet::UP,eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::DOWN, eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::LEFT, eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::RIGHT, eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::SPACE, eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::P, eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::R, eKeyState::UP));
		mpInstance->mKeys.insert(std::make_pair(eKeySet::S, eKeyState::UP));
	}
	return mpInstance;
}

void KeyInput::Update()
{
	for (auto& key : mKeys)
	{
		int vk = static_cast<int>(key.first);
		if (GetAsyncKeyState(vk) && key.second == eKeyState::UP)
		{
			key.second = eKeyState::DOWN;
		}
		else if (GetAsyncKeyState(vk) && 
			(key.second == eKeyState::DOWN || key.second == eKeyState::PRESS))
		{
			key.second = eKeyState::PRESS;
		}
		else
		{
			key.second = eKeyState::UP;
		}
	}
}

bool KeyInput::IsKeyDown(int key)
{
	auto k = mKeys.find(static_cast<eKeySet>(key));
	return k->second == eKeyState::DOWN;
}

bool KeyInput::IsKeyUp(int key)
{
	auto k = mKeys.find(static_cast<eKeySet>(key));
	return k->second == eKeyState::UP;
}
