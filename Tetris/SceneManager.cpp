#include "SceneManager.h"
#include "Scene.h"
SceneManager* SceneManager::mInstance = nullptr;

SceneManager::SceneManager()
	: mpNowScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	if (mpNowScene)
	{
		delete mpNowScene;
	}
}

SceneManager* SceneManager::GetInstance()
{
	if (!mInstance)
	{
		mInstance = new SceneManager();
	}
	return mInstance;
}

void SceneManager::SetScene(Scene* scene)
{
	if (mpNowScene)
	{
		delete mpNowScene;
	}
	mpNowScene = scene;
}

Scene* SceneManager::GetScene()
{
	return mpNowScene;
}
