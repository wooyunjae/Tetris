#pragma once

class MouseInput
{
private:
	MouseInput();
	~MouseInput();

public:
	static MouseInput* GetInstance();
	static void DeleteGetInstance();

	
private:
	static MouseInput* mInstance;
};