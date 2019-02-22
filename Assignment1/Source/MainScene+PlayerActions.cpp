#include "MainScene.h"

#include "Utility.h"

void MainScene::updatePlayer(Player* p, double& dt)
{
	int i = p->getId();
	controller->getInput(GLFW_JOYSTICK_1 + i);
	joystickEvents(dt, i);

	// Player swing

	Vector3 curr = Application::GetMousePosition();
	Vector3 diff = prevMousePosition - curr;
	prevMousePosition = curr;

	Vector3 leftJS = controller->getLeftJoystick();
	if (leftJS.x || leftJS.y) diff = Vector3(leftJS.x, leftJS.y, 0) * 200;

	Vector3 center = p->getBody()->getCenter();
	Vector3 dir = center - camera.getPosition();
	float yaw = atan(dir.x / dir.z);
	dir.z /= abs(dir.z);
	Mtx44 rotation; rotation.SetToRotation(deg(yaw), 0, 1, 0);

	Vector3 impulse = rotation * Vector3(diff.x * -dir.z, diff.Length() * 1.5, diff.y * dir.z);

	if (p->isGrabbingLeft())
		manager->applyImpulse(p->getRightArm(), impulse, dt);

	if (p->isGrabbingRight())
		manager->applyImpulse(p->getLeftArm(), impulse, dt);



	// grabbing

	if (Application::IsKeyPressed('Q') || controller->getLT() > 0)
	{
		if (!p->isGrabbingLeft())
		{
			Object* leftHand = p->getLeftHand();

			CollisionDetails details;
			for (Player* other : players->getLocalPlayers())
			{
				if (i == other->getId()) continue;
				details = manager->getCollisionDetails(leftHand, other->getParts());

				if (details.result.collided) break;
			}

			if (details.result.collided)
			{
				p->leftGrab(details.object->getEnd());
			}
			else
			{
				for (Player* p : players->getRemotePlayers())
				{
					details = manager->getCollisionDetails(leftHand, p->getParts());

					if (details.result.collided) break;
				}

				if (details.result.collided)
				{
					p->leftGrab(details.object->getEnd());
				}
				else
				{
					details = manager->getEnviromentalCollision(leftHand);
					if (details.result.collided)
					{
						p->leftGrab(details.object->getEnd());
					}
				}
			}
		}
	}
	else
	{
		p->releaseLeft();
	}

	if (Application::IsKeyPressed('E') || controller->getRT() > 0)
	{
		if (!p->isGrabbingRight())
		{
			Object* rightHand = p->getRightHand();

			CollisionDetails details;
			for (Player* other : players->getLocalPlayers())
			{
				if (i == other->getId()) continue;
				details = manager->getCollisionDetails(rightHand, other->getParts());

				if (details.result.collided) break;
			}

			if (details.result.collided)
			{
				p->rightGrab(details.object->getEnd());
			}
			else
			{
				for (Player* p : players->getRemotePlayers())
				{
					details = manager->getCollisionDetails(rightHand, p->getParts());

					if (details.result.collided) break;
				}

				if (details.result.collided)
				{
					p->rightGrab(details.object->getEnd());
				}
				else
				{
					details = manager->getEnviromentalCollision(rightHand);
					if (details.result.collided)
					{
						p->rightGrab(details.object->getEnd());
					}
				}
			}
		}
	}
	else
	{
		p->releaseRight();
	}

	manager->updateObjects();
	manager->updateSprings();
	manager->resolveCollisions();

}