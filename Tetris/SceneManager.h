#pragma once

#define SM (SceneManager::GetInstance())

class Scene;
class SceneManager
{
private:
	SceneManager();
	~SceneManager();

public:
	static SceneManager* GetInstance();

	void SetScene(Scene* scene);

	Scene* GetScene();
private:
	static SceneManager* mInstance;
	Scene* mpNowScene;
};