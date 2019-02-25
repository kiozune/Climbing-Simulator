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
	if (leftJS.x || leftJS.y) diff = Vector3(leftJS.x, leftJS.y, 0);

	Vector3 center = p->getBody()->getCenter();
	Vector3 dir = center - cameras[i].getPosition();
	float yaw = atan(dir.x / dir.z);
	dir.z /= abs(dir.z);
	Mtx44 rotation; rotation.SetToRotation(deg(yaw), 0, 1, 0);

	Vector3 impulse = rotation * Vector3(diff.x * -dir.z, diff.Length() * 1.5, diff.y * dir.z) * 40 * p->getEnergy();
	float energyRequired = dt * max(fabs(diff.x), fabs(diff.y));

	if (p->isGrabbingLeft())
	{
		manager->applyImpulse(p->getRightArm(), impulse, dt);
		p->tire(energyRequired);
		//manager->applyImpulse(p->getBody(), impulse * -0.5, dt);
	}

	if (p->isGrabbingRight())
	{
		manager->applyImpulse(p->getLeftArm(), impulse, dt);
		p->tire(energyRequired);
		//manager->applyImpulse(p->getBody(), impulse * -0.5, dt);
	}

	if (!(int)(energyRequired * 100))
	{
		p->recover(dt * 10);
	}


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

void MainScene::renderForPlayer(Player* p)
{
	FixedCamera& camera = cameras[p->getId()];
	// get reference to camera based on state
	Vector3 position = camera.getPosition();
	Vector3 target = camera.getTarget();
	Vector3 up = camera.getUp();

	viewStack.LoadIdentity();
	viewStack.LookAt(
		position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z
	);

	modelStack.LoadIdentity();

	for (Light& light : lights) {
		if (light.type == Light::DIRECTIONAL)
		{
			Vector3 lightDir(light.position.x, light.position.y, light.position.z);
			Mtx44 vs = viewStack.Top();
			vs.a[12] = vs.a[13] = vs.a[14] = 0;
			Vector3 lightDirection_cameraspace = vs * lightDir;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightDirection_cameraspace.x);
		}
		else if (light.type == Light::SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light.position;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightPosition_cameraspace.x);
			Mtx44 vs = viewStack.Top();
			vs.a[12] = vs.a[13] = vs.a[14] = 0;
			Vector3 spotDirection_cameraspace = vs * light.spotDirection;
			glUniform3fv(light.parameters[Light::L_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * light.position;
			glUniform3fv(light.parameters[Light::L_POSITION], 1, &lightPosition_cameraspace.x);
		}

		// render light
		modelStack.PushMatrix();
		{
			modelStack.Translate(light.position.x, light.position.y, light.position.z);
			renderMesh(models[LIGHT]);
		}
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	//renderMesh(models[SKY_BOX]);
	modelStack.PopMatrix();

	for (Object* obj : manager->getObjects())
	{
		renderObject(obj);
		//renderBoundingBox(obj->getBoundingBox());
	}

	float yaw = camera.getYaw();
	float pitch = camera.getPitch();

	Vector3 center = p->getBody()->getCenter();
	Vector3 left = p->getLeftHand()->getCenter();
	Vector3 right = p->getRightHand()->getCenter();

	modelStack.PushMatrix();
	{
		float scale = 3 * p->getEnergy();
		float offset = (9 - scale) / 2.0;
		modelStack.Translate(center.x, center.y + 15, center.z);

		modelStack.Rotate(180 - yaw, 0, 1, 0);
		modelStack.Rotate(-pitch, 0, 0, 1);

		modelStack.Scale(1, 2, scale);

		glDisable(GL_DEPTH_TEST);
		renderMesh(models[QUAD]);
		glEnable(GL_DEPTH_TEST);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(left.x, left.y + 5, left.z);

		modelStack.Rotate(270 - yaw, 0, 1, 0);
		modelStack.Rotate(pitch, 1, 0, 0);

		modelStack.Scale(3, 3, 3);

		Color color = p->isGrabbingLeft() ? Color(.9, .9, 0) : Color(1, 1, 1);
		renderText(models[TEXT], "LT", color);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(right.x, right.y + 5, right.z);

		modelStack.Rotate(270 - yaw, 0, 1, 0);
		modelStack.Rotate(pitch, 1, 0, 0);

		modelStack.Scale(3, 3, 3);

		Color color = p->isGrabbingRight() ? Color(0, .9, .9) : Color(1, 1, 1);
		renderText(models[TEXT], "RT", color);
	}
	modelStack.PopMatrix();
}