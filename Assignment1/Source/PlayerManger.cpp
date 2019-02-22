#include "PlayerManager.h"

#include "ControllerManager.h"
#include "PhysicsManager.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	if (!instance) instance = new PlayerManager;
	return instance;
}

void PlayerManager::createRemotePlayers(std::string str, unsigned clientId)
{
	for (int i = 0; i < str.size() / 2; ++i)
	{
		unsigned id = (unsigned)str[i * 2];
		unsigned count = (unsigned)str[i * 2 + 1] - 1;
		if (id == clientId) continue;
		bool found = false;
		for (RemotePlayer* r : this->remotePlayers)
		{
			if (id == r->getId() / 10)
			{
				found = true;
				break;
			}
		}
		if (found) continue;

		for (int r = 0; r < count; ++r)
			addRemotePlayer(createRemotePlayer(id, r));
	}
}

void PlayerManager::fixMissingPlayers()
{
	int i = 0;
	ControllerManager* controller = ControllerManager::getInstance();
	while (controller->isPresent(i))
	{
		if (i >= this->localPlayers.size())
		{
			Player* p = createPlayer(i);
			this->localPlayers.push_back(p);
		}
		i++;
	}
}

RemotePlayer* PlayerManager::createRemotePlayer(unsigned clientId, unsigned id)
{
	return (RemotePlayer*)createPlayer(clientId * 10 + id);
}

Player* PlayerManager::createPlayer(unsigned id)
{
	Player* p = new Player;

	p->setId(id + 1);

	Joint* leftFingers = new Joint(Vector3(13, 0, 0));
	Joint* leftWrist = new Joint(Vector3(10, 0, 0));
	Joint* leftElbow = new Joint(Vector3(5, 0, 0));
	Joint* rightFingers = new Joint(Vector3(-13, 0, 0));
	Joint* rightWrist = new Joint(Vector3(-10, 0, 0));
	Joint* rightElbow = new Joint(Vector3(-5, 0, 0));
	Joint* head = new Joint(Vector3(0, 0, 5));
	Joint* chest = new Joint(Vector3(0, 0, 0));
	Joint* pelvis = new Joint(Vector3(0, 0, -10));
	Joint* leftFeet = new Joint(Vector3(5, 0, -20));
	Joint* rightFeet = new Joint(Vector3(-5, 0, -20));

	p->setLeftFingers(leftFingers);
	p->setRightFingers(rightFingers);

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

	Color primary = Color((float)(id % 5) / 5.0, (float)(id % 3) / 5.0, (float)(id % 7) / 5.0);

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

	p->setLeftHand(leftHand);
	p->setLeftArm(leftArm);
	p->setRightHand(rightHand);
	p->setRightArm(rightArm);
	p->setBody(body);

	p->setParts(
		{
		leftHand, leftArm, leftBicep,
		rightHand, rightArm, rightBicep,
		neck, body,
		leftLeg, rightLeg
	});

	PhysicsManager* manager = PhysicsManager::getInstance();
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

	manager->addExternalSpring(p->getLeftSpring());
	manager->addExternalSpring(p->getRightSpring());

	return p;
}

std::vector<Player*> PlayerManager::getLocalPlayers() { return this->localPlayers; }
void PlayerManager::addLocalPlayer(Player* p) { this->localPlayers.push_back(p); }

void PlayerManager::removeLocalPlayer(int i) 
{  
	this->localPlayers.erase(this->localPlayers.begin() + i);
}

std::vector<RemotePlayer*> PlayerManager::getRemotePlayers() { return this->remotePlayers; }
void PlayerManager::addRemotePlayer(RemotePlayer* p) { this->remotePlayers.push_back(p); }

void PlayerManager::updateRemote(PlayerData data)
{
	for (RemotePlayer* r : this->remotePlayers)
		if (r->getId() == data.id)
			r->update(data);
}