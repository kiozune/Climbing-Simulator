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
	models[JOINONLINE_QUAD] = MeshBuilder::GenerateText("Join_Online", 16, 16);
	models[JOINONLINE_QUAD]->applyTexture("Image//calibri.tga");
	models[STARTLOCAL_QUAD] = MeshBuilder::GenerateText("Start Local", 16, 16);
	models[STARTLOCAL_QUAD]->applyTexture("Image//calibriOpacity.tga");
	models[JOINLOCAL_QUAD] = MeshBuilder::GenerateText("JOIN LOCAL", 16, 16);
	models[JOINLOCAL_QUAD]->applyTexture("Image//calibriOpacity.tga");
	models[EXIT] = MeshBuilder::GenerateText("EXIT", 16, 16);
	models[EXIT]->applyTexture("Image//calibriOpacity.tga");

}

void MainScene::initMap()
{
	blockGen->setSize(rand() % 15 + 5); // set the sizing first before starting up the linklist
	blockGen->generateBlocks(75); // start up the linklist and get all the map data to populate param = offset between nodes
	block* current = blockGen->getHead();

	while (current != blockGen->getTail()) // loop through the linklist then 
	{
		block* temp = current->getNext();
		if (current == blockGen->getHead()) // start of the linklist
		{
			Object* platform = new Object(Vector3(100, 10, 100), Vector3(0, -20, 0), 0, false);
			platform->setColour(Color(0.5, 0.5, 0.5));
			manager->addToEnvironment(platform);
			current = temp;
		}
		for (int i = 0; i < current->getCubeVectors()->size(); i++)
		{
			Vector3 pos = current->getCubeVectors()->at(i);
			float randFloat = static_cast<float>(rand() % 15 + 5);
			Vector3 scale = { randFloat,randFloat,randFloat };
			if (i == current->getCubeVectors()->size() - 1 && rand() % 2) // last cube will be a rope with an off chance there isn't
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
				box->setColour(Color(0.5, 0.5, 0.5));
				manager->addToEnvironment(box);
			}
		}
		current = temp;
	}

	
	//int x, y, z;
	//int w, h, d;
	//x = y = z = 0;
	//for (int i = 0; i < 100; ++i)
	//{
	//	w = rand() % 25 + 5;
	//	h = rand() % 25 + 5;
	//	d = rand() % 25 + 5;
	//	int dir = (rand() % 2) * 2 - 1;
	//	if (rand() % 2) x += dir * (rand() % 5 + 20 + w / 2.0);
	//	else z += dir * (rand() % 5 + 20 + d / 2.0);

	//	if (rand() % 2) y += dir * (rand() % 15 + 5 + h / 2.0);

	//	if (rand() % 5)
	//	{
	//		Object* box = new Object(Vector3(w, h, d), Vector3(x, y, z), 0, false);
	//		box->setColour(Color(0.5, 0.5, 0.5));
	//		//box->setClipping(true);
	//		manager->addToEnvironment(box);
	//	}
	//	else
	//	{
	//		std::vector<Joint*> hinges;
	//		hinges.push_back(new Joint(Vector3(x, 60 + y, z), true));
	//		for (int i = 1; i < rand() % 10 + 5; ++i)
	//		{
	//			hinges.push_back(new Joint(Vector3(x, 60 + y - i * 15, z)));
	//			Object* chain = new Object(hinges[i - 1], hinges[i], 10, 5);
	//			chain->setColour(Color(0.2, 0.6, 0.5));
	//			chain->setClipping(true);
	//			manager->addToEnvironment(chain);
	//		}
	//	}
	//}

	Object* platform = new Object(Vector3(100, 50, 100), Vector3(0, -30, 0), 0, false);
	platform->setColour(Color(0.5, 0.5, 0.5));
	manager->addToEnvironment(platform);
}