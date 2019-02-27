#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager * SceneManager::getInstance()
{
	if (!instance) instance = new SceneManager;
	return instance;
}

Scene* SceneManager::getCurrent() { return this->current; }
void SceneManager::setCurrent(Scene* scene) { this->current = scene; }

Scene* SceneManager::getNext() { return this->next; }
void SceneManager::setNext(Scene* scene) { this->next = scene; }