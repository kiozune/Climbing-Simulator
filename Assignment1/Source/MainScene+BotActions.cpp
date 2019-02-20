#include "MainScene.h"

void MainScene::updateBot(int i, double& dt)
{
	Bot& b = bots[i];
	Brain& brain = b.getBrain();
	brain.update();
	// Player swing

	Vector3 diff = Vector3(brain.getXSwing(), brain.getYSwing(), 0) * 200;

	Vector3 impulse = Vector3(diff.x, diff.Length() * 1.5, diff.y);

	if (b.isGrabbingLeft())
		manager->applyImpulse(b.getRightArm(), impulse, dt);

	if (b.isGrabbingRight())
		manager->applyImpulse(b.getLeftArm(), impulse, dt);


	// grabbing

	if (brain.isGrabbingLeft())
	{
		if (!b.isGrabbingLeft())
		{
			Object* leftHand = b.getLeftHand();

			CollisionDetails details;
			for (int j = 0; j < PLAYER_COUNT; ++j)
			{
				details = manager->getCollisionDetails(leftHand, players[j].getParts());

				if (details.result.collided) break;
			}

			if (details.result.collided)
			{
				b.leftGrab(details.object->getEnd());
			}
			else
			{
				details = manager->getEnviromentalCollision(leftHand);
				if (details.result.collided)
				{
					b.leftGrab(details.object->getEnd());
				}
			}
		}
	}
	else
	{
		b.releaseLeft();
	}

	if (brain.isGrabbingRight())
	{
		if (!b.isGrabbingRight())
		{
			Object* rightHand = b.getRightHand();

			CollisionDetails details;
			for (int j = 0; j < PLAYER_COUNT; ++j)
			{
				details = manager->getCollisionDetails(rightHand, players[j].getParts());

				if (details.result.collided) break;
			}

			if (details.result.collided)
			{
				b.rightGrab(details.object->getEnd());
			}
			else
			{
				details = manager->getEnviromentalCollision(rightHand);
				if (details.result.collided)
				{
					b.rightGrab(details.object->getEnd());
				}
			}
		}
	}
	else
	{
		b.releaseRight();
	}

	manager->updateObjects();
	manager->updateSprings();
}