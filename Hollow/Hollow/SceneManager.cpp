#include "SceneManager.h"

void SceneManager::startUp()
{
	setStartedUp();
}

void SceneManager::shutdown()
{
	for (auto& it : sceneObjects)
		delete it;
	setShutdown();
}	

std::vector<IRenderable*>* SceneManager::GetSceneObjects()
{
	return &sceneObjects;
}
