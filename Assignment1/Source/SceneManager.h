#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

class SceneManager
{

private:

	static SceneManager * instance;

	Scene *current, *next;

public:

	static SceneManager * getInstance();

	Scene* getCurrent();
	void setCurrent(Scene*);

	Scene* getNext();
	void setNext(Scene*);

};

#endif