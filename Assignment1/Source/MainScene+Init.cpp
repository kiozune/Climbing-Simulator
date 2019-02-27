#include "MainScene.h"

#include "LoadTGA.h"

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
	blockGen->generateBlocks(60); // start up the linklist and get all the map data to populate param = offset between nodes
	block* current = blockGen->getHead();

	while (current != blockGen->getTail()) // loop through the linklist then 
	{
		block* temp = current->getNext();
		if (current == blockGen->getHead()) // start of the linklist (starting block)
		{
			Object* platform = new Object(Vector3(100, 10, 100), Vector3(0, -20, 0), 0, false);
			platform->setColour(Color(0.5, 0.5, 0.5));
			manager->addToEnvironment(platform);
			current = temp;
		}
		else
		{
			for (unsigned i = 0; i < current->getCubeVectors()->size(); i++)
			{
				Vector3 pos = current->getCubeVectors()->at(i);
				float randFloat = static_cast<float>(rand() % 20 + 8); // for scaling
				Vector3 scale = { randFloat,randFloat,randFloat }; // pseudo sizing
				if (i == current->getCubeVectors()->size() - 1 )//&& rand() % 2) // last cube will be a rope with an off chance there isn't
				{
					std::vector<Joint*> hinges;
					hinges.push_back(new Joint(Vector3(pos.x, 60 + pos.y, pos.z), true));
					for (int i = 1; i < rand() % 10 + 5; ++i)
					{
						hinges.push_back(new Joint(Vector3(pos.x, 60 + pos.y - i * 15, pos.z)));
						Object* chain = new Object(hinges[i - 1], hinges[i], 10, 5);
						chain->setColour(Color(0.2, 0.6, 0.5));
						chain->setClipping(true);
						manager->addToEnvironment(chain);
					}
				}
				else // normal cubes
				{
					Object* box = new Object(scale, pos, 0, false);
					float randomFloat = -90 + static_cast<float> (rand()) / (static_cast <float> (RAND_MAX / (90 - (-90))));
					Vector3 rotation = { 0,randomFloat,randomFloat };
					box->setRotation(rotation);
					box->setColour(Color(0.5, 0.5, 0.5));
					manager->addToEnvironment(box);
				}
			}
			current = temp;
		}
	}
	if (current == blockGen->getTail()) // finishing goal
	{
		Object* platform = new Object(Vector3(100, 10, 100), current->getVector3(), 0, false);
		platform->setColour(Color(1, 0, 0));
		manager->addToEnvironment(platform);
		finishingPlatform = platform;
	}
}

void MainScene::initMenu() 
{
	//Ini textures into unsigned
	t_opaque = LoadTGA("Image//calibri.tga");
	t_alpha = LoadTGA("Image//calibriOpacity.tga");

	models[RESUME_GAME] = MeshBuilder::GenerateText("RESUME_GAME", 16, 16);
	models[RESUME_GAME]->applyTexture("Image//calibriOpacity.tga");
	models[EXIT_GAME] = MeshBuilder::GenerateText("EXIT_GAME", 16, 16);
	models[EXIT_GAME]->applyTexture("Image//calibri.tga");
}