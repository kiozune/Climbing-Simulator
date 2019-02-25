#include "MainScene.h"


void MainScene::applyMaterial(Mesh* model)
{
	model->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	model->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	model->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	model->material.kShininess = 1.0f;
}

void MainScene::changeColour(Mesh* model, Color color)
{
	model->material.kAmbient.Set(color.r, color.g, color.b);
}

void MainScene::initText() 
{
	models[TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
	models[TEXT]->applyTexture("Image//calibri.tga");
}

void MainScene::initMap()
{
	int x, y, z;
	int w, h, d;
	x = y = z = 0;
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		w = rand() % 25 + 5;
		h = rand() % 25 + 5;
		d = rand() % 25 + 5;
		int dir = (rand() % 2) * 2 - 1;
		if (rand() % 2) x += dir * (rand() % 5 + 20 + w / 2.0);
		else z += dir * (rand() % 5 + 20 + d / 2.0);

		if (rand() % 2) y += rand() % 15 + 5 + h / 2.0;

		if (rand() % 5)
		{
			Object* box = new Object(Vector3(w, h, d), Vector3(x, y, z), 0, false);
			box->setColour(Color(0.5, 0.5, 0.5));
			//box->setClipping(true);
			manager->addToEnvironment(box);
		}
		else
		{
			std::vector<Joint*> hinges;
			hinges.push_back(new Joint(Vector3(x, 60 + y, z), true));
			for (int i = 1; i < rand() % 10 + 5; ++i)
			{
				hinges.push_back(new Joint(Vector3(x, 60 + y - i * 15, z)));
				Object* chain = new Object(hinges[i - 1], hinges[i], 10, 5);
				chain->setColour(Color(0.2, 0.6, 0.5));
				chain->setClipping(true);
				manager->addToEnvironment(chain);
			}
		}
	}


	Object* platform = new Object(Vector3(100, 50, 100), Vector3(0, -30, 0), 0, false);
	platform->setColour(Color(0.5, 0.5, 0.5));
	manager->addToEnvironment(platform);
}