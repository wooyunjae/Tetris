#include "KeyInput.h"

KeyInput* KeyInput::mInstance = nullptr;

KeyInput* KeyInput::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new KeyInput();
		mInstance->mKeys.insert(std::make_pair(eKeySet::UP,eKeyState::UP));
		mInstance->mKeys.insert(std::make_pair(eKeySet::DOWN, eKeyState::UP));
		mInstance->mKeys.insert(std::make_pair(eKeySet::LEFT, eKeyState::UP));
		mInstance->mKeys.insert(std::make_pair(eKeySet::RIGHT, eKeyState::UP));
		mInstance->mKeys.insert(std::make_pair(eKeySet::SPACE, eKeyState::UP));
		mInstance->mKeys.insert(std::make_pair(eKeySet::P, eKeyState::UP));
		mInstance->mKeys.insert(std::make_pair(eKeySet::R, eKeyState::UP));
	}
	return mInstance;
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
	int lowKey = toupper(key);
	auto k = mKeys.find(static_cast<eKeySet>(lowKey));
	return k->second == eKeyState::DOWN;
}

bool KeyInput::IsKeyUp(int key)
{
	int lowKey = toupper(key);
	auto k = mKeys.find(static_cast<eKeySet>(lowKey));
	return k->second == eKeyState::UP;
}
