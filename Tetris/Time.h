#pragma once
#include <Windows.h>
class Time
{
public:
	Time();
	~Time() = default;
	float GetDeltaTime();
	void Reset();
private:
	LARGE_INTEGER mCountTime;
	LARGE_INTEGER mCurTime;
	LARGE_INTEGER mPrevTime;
};

