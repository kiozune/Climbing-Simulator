#include "MainScene.h"

void MainScene::initText() 
{
	models[TEXT] = MeshBuilder::GenerateText("TEXT", 16, 16);
	models[TEXT]->applyTexture("Image//calibri.tga");
}

void MainScene::initPlayer(Player& p, Vector3 offset)
{
	Joint* chest = new Joint(Vector3(0, 0, 0) + offset);
	Joint* leftFingers = new Joint(Vector3(13, 0, 0) + offset);
	Joint* leftWrist = new Joint(Vector3(10, 0, 0) + offset);
	Joint* leftElbow = new Joint(Vector3(5, 0, 0) + offset);
	Joint* rightFingers = new Joint(Vector3(-13, 0, 0) + offset);
	Joint* rightWrist = new Joint(Vector3(-10, 0, 0) + offset);
	Joint* rightElbow = new Joint(Vector3(-5, 0, 0) + offset);
	Joint* head = new Joint(Vector3(0, 0, 5) + offset);
	Joint* pelvis = new Joint(Vector3(0, 0, -10) + offset);
	Joint* leftFeet = new Joint(Vector3(5, 0, -20) + offset);
	Joint* rightFeet = new Joint(Vector3(-5, 0, -20) + offset);

	p.setLeftFingers(leftFingers);
	p.setRightFingers(rightFingers);

	float mass = 5, size = 2;

	Object* leftHand = new Object(leftFingers, leftWrist, mass, size + 1);
	Object* leftArm = new Object(leftElbow, leftWrist, mass, size);
	Object* leftBicep = new Object(chest, leftElbow, mass, size);

	Object* rightHand = new Object(rightFingers, rightWrist, mass, size);
	Object* rightArm = new Object(rightElbow, rightWrist, mass, size);
	Object* rightBicep = new Object(chest, rightElbow, mass, size);

	Object* neck = new Object(chest, head, mass, size);
	Object* body = new Object(chest, pelvis, mass, size);

	Object* leftLeg = new Object(pelvis, leftFeet, mass, size);
	Object* rightLeg = new Object(pelvis, rightFeet, mass, size);

	Color primary = Color(rand() % 255 / 255.0, rand() % 255 / 255.0, rand() % 255 / 255.0);

	leftHand->setColour(Color(0.9, 0.9, 0));
	leftArm->setColour(primary);
	leftBicep->setColour(primary);

	rightHand->setColour(Color(0.0, 0.9, 0.9));
	rightArm->setColour(primary);
	rightBicep->setColour(primary);

	neck->setColour(primary);

	body->setColour(primary);
	leftLeg->setColour(primary);
	rightLeg->setColour(primary);

	p.setLeftHand(leftHand);
	p.setLeftArm(leftArm);
	p.setRightHand(rightHand);
	p.setRightArm(rightArm);
	p.setBody(body);

	p.setParts({
		leftHand, leftArm, leftBicep,
		rightHand, rightArm, rightBicep,
		neck, body,
		leftLeg, rightLeg
	});

	manager->addObject(leftHand);
	manager->addObject(leftArm);
	manager->addObject(leftBicep);

	manager->addObject(rightHand);
	manager->addObject(rightArm);
	manager->addObject(rightBicep);

	manager->addObject(neck);
	manager->addObject(body);

	manager->addObject(leftLeg);
	manager->addObject(rightLeg);

	Spring* topLeft = new Spring(head, leftWrist, 0.2, 1.5, 0.2);
	Spring* topRight = new Spring(head, rightWrist, 0.2, 1.5, 0.2);
	Spring* mid = new Spring(head, pelvis, 1, 1, 0.2);
	Spring* midLeft = new Spring(pelvis, leftWrist, 0.2, 1.5, 0.2);
	Spring* midRight = new Spring(pelvis, rightWrist, 0.2, 1.5, 0.2);
	Spring* bottom = new Spring(leftFeet, rightFeet, 0.2, 1, 0.2);
	Spring* bottomLeft = new Spring(chest, leftFeet, 0.6, 1, 0.2);
	Spring* bottomRight = new Spring(chest, rightFeet, 0.6, 1, 0.2);

	manager->addSpring(topLeft);
	manager->addSpring(topRight);
	manager->addSpring(mid);
	manager->addSpring(midLeft);
	manager->addSpring(midRight);
	manager->addSpring(bottom);
	manager->addSpring(bottomLeft);
	manager->addSpring(bottomRight);

	manager->addExternalSpring(p.getLeftSpring());
	manager->addExternalSpring(p.getRightSpring());
}

void MainScene::initMap()
{
	blockGen->generateBlocks(80); // start up the linklist and get all the map data to populate param = offset between nodes
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
			for (int i = 0; i < current->getCubeVectors()->size(); i++)
			{
				Vector3 pos = current->getCubeVectors()->at(i);
				float randFloat = static_cast<float>(rand() % 15 + 10);
				Vector3 scale = { randFloat,randFloat,randFloat }; // pseudo sizing
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
		// current = temp;
	}
	if (current == blockGen->getTail()) // finishing goal
	{
		Object* platform = new Object(Vector3(100, 10, 100), current->getVector3(), 0, false);
		platform->setColour(Color(1, 0, 0));
		manager->addToEnvironment(platform);
	}
}