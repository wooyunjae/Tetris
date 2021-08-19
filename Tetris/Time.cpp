#include "Time.h"

Time::Time()
{
	Reset();
}

float Time::GetDeltaTime()
{
	float deltaTime;
	QueryPerformanceCounter(&mCurTime);

	deltaTime = static_cast<float>((static_cast<double>(mCurTime.QuadPart)
		- static_cast<double>(mPrevTime.QuadPart))
		/ static_cast<double>(mCountTime.QuadPart));
	mPrevTime = mCurTime;

	return deltaTime;
}

void Time::Reset()
{
	QueryPerformanceFrequency(&mCountTime);
	QueryPerformanceCounter(&mCurTime);
	QueryPerformanceCounter(&mPrevTime);
}
