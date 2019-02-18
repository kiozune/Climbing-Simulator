#include "MainScene.h"

#include "GL\glew.h"
#include "Mtx44.h"
#include "Utility.h"

#include "shader.hpp"
#include "Application.h"

#include <iostream>

void MainScene::Init()
{
	srand(time(NULL));

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	projectionStack.LoadMatrix(projection);

	// clear screen and fill with white
	glClearColor(0, 0, 0, 0);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a VAO
	glGenVertexArrays(1, &m_vertexArrayID);
	// bind that VAO
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");

	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	for (int i = 0; i < LIGHT_COUNT; ++i)
		lights[i].getUniformLocation(m_programID);

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::SPOT;
	lights[0].position.Set(0, 300, 0);
	lights[0].setUniform();

	glUniform1i(m_parameters[U_NUMLIGHTS], LIGHT_COUNT);

	// initialisation of personal variables

	lightingEnabled = true;

	initText();

	models[LIGHT] = MeshBuilder::GenerateSphere("LIGHT", Color(1, 1, 1), 1, 36);

	models[SKY_BOX] = MeshBuilder::GenerateOBJ("skybox");
	models[SKY_BOX]->applyTexture("Image//skybox.tga");
	applyMaterial(models[SKY_BOX]);

	models[CUBE] = MeshBuilder::GenerateCube("CUBE", Color(1, 1, 1), 1, 1, 1);
	applyMaterial(models[CUBE]);

	camera.Init(Vector3(0, 0, 0), 200, 100, 180);

	Joint* chest = new Joint(Vector3(0, 0, 0));
	Joint* leftFingers = new Joint(Vector3(13, 0, 0));
	Joint* leftWrist = new Joint(Vector3(10, 0, 0));
	Joint* leftElbow = new Joint(Vector3(5, 0, 0));
	Joint* rightFingers = new Joint(Vector3(-13, 0, 0));
	Joint* rightWrist = new Joint(Vector3(-10, 0, 0));
	Joint* rightElbow = new Joint(Vector3(-5, 0, 0));
	Joint* head = new Joint(Vector3(0, 0, 5));
	Joint* pelvis = new Joint(Vector3(0, 0, -10));
	Joint* leftFeet = new Joint(Vector3(5, 0, -20));
	Joint* rightFeet = new Joint(Vector3(-5, 0, -20));

	p.setLeftFingers(leftFingers);
	p.setRightFingers(rightFingers);

	float mass = 15, size = 2;

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

	leftHand->setColour(Vector3(0.9, 0.9, 0));
	leftArm->setColour(Vector3(0.0, 0.2, 0.5));
	leftBicep->setColour(Vector3(0.0, 0.2, 0.5));

	rightHand->setColour(Vector3(0.0, 0.9, 0.9));
	rightArm->setColour(Vector3(0.0, 0.2, 0.5));
	rightBicep->setColour(Vector3(0.0, 0.2, 0.5));
	
	neck->setColour(Vector3(0.0, 0.2, 0.5));
	
	body->setColour(Vector3(0.0, 0.2, 0.5));
	leftLeg->setColour(Vector3(0.0, 0.2, 0.5));
	rightLeg->setColour(Vector3(0.0, 0.2, 0.5));

	p.setLeftHand(leftHand);
	p.setLeftArm(leftArm);
	p.setRightHand(rightHand);
	p.setRightArm(rightArm);
	p.setBody(body);

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

	int x, y, z;
	int w, h, d;
	x = y = z = 0;
	for (int i = 0; i < 100; ++i)
	{
		w = rand() % 25 + 5;
		h = rand() % 25 + 5;
		d = rand() % 25 + 5;
		int dir = (rand() % 2) * 2 - 1;
		if (rand() % 2) x += dir * (rand() % 5 + 20 + w / 2.0);
		else z += dir * (rand() % 5 + 20 + d / 2.0);

		if (rand() % 2) y += dir * (rand() % 15 + 5 + h / 2.0);

		if (rand() % 5)
		{
			Object* box = new Object(Vector3(w, h, d), Vector3(x, y, z), 0, false);
			box->setColour(Vector3(0.5, 0.5, 0.5));
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
				chain->setColour(Vector3(0.2, 0.6, 0.5));
				chain->setClipping(true);	
				manager->addToEnvironment(chain);
			}
		}
	}
	

	Object* platform = new Object(Vector3(100, 10, 100), Vector3(0, -20, 0), 0, false);
	platform->setColour(Vector3(0.5, 0.5, 0.5));
	manager->addToEnvironment(platform);

	Spring* topLeft = new Spring(head, leftWrist, 0.2, 1.5, 0.2);
	Spring* topRight = new Spring(head, rightWrist, 0.2, 1.5, 0.2);
	Spring* mid = new Spring(head, pelvis, 1, 1, 0.2);
	Spring* midLeft = new Spring(pelvis, leftWrist , 0.2, 1.5, 0.2);
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

void MainScene::Update(double dt)
{

	elapseTime += dt;
	fps = 1 / dt;

	// standard controls

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('7'))
		lightingEnabled = false;
	if (Application::IsKeyPressed('8'))
		lightingEnabled = true;

	if (Application::IsKeyPressed('9'))
		dt /= 100;

	if (Application::IsControllerPressed(GLFW_JOYSTICK_1) && elapseTime > bounceTime)
	{
		camera.setAuto(!camera.isAuto());
		bounceTime = elapseTime + 0.2;
	}

	if (Application::IsControllerPressed(GLFW_JOYSTICK_2))
		camera.zoomIn(dt);

	if (Application::IsControllerPressed(GLFW_JOYSTICK_3))
		camera.zoomOut(dt);

	const float* analog;
	float swingX, swingY, LT, RT, camX, camY;
	swingX = swingY = LT = RT = camX = camY = 0;

	if (Application::isControllerPresent())
	{
		analog = Application::getControllerAnalog();
		swingX = analog[0], swingY = analog[1];
		LT = analog[4], RT = analog[5];
		camX = analog[2], camY = analog[3];

		if (!isXboxController)
		{
			swingX = analog[0], swingY = -analog[1];
			LT = analog[3] + 1, RT = analog[4] + 1;
			camX = -int(analog[2] * 10) / 10.0, camY = int(analog[5] * 10) / 10.0;
		}
	}





	manager->applyGravity(dt);





	// Player swing

	Vector3 curr = Application::GetMousePosition();
	Vector3 diff = prevMousePosition - curr;
	if (swingX || swingY) diff = Vector3(swingX, swingY, 0) * 200;
	prevMousePosition = curr;

	Vector3 center = p.getBody()->getCenter();
	Vector3 dir = center - camera.getPosition();
	float yaw = atan(dir.x / dir.z);
	dir.z /= abs(dir.z);
	Mtx44 rotation; rotation.SetToRotation(deg(yaw), 0, 1, 0);

	Vector3 impulse = rotation * Vector3(diff.x * -dir.z, diff.Length() * 1.5, diff.y * dir.z);

	if (p.isLeftGrabbing())
		manager->applyImpulse(p.getRightArm(), impulse, dt);

	if (p.isRightGrabbing())
		manager->applyImpulse(p.getLeftArm(), impulse, dt);




	manager->resolveCollisions();

	// grabbing

	if (Application::IsKeyPressed('Q') || LT > 0)
	{
		if (!p.isLeftGrabbing())
		{
			CollisionDetails details = manager->getEnviromentalCollision(p.getLeftHand());
			if (details.result.collided)
			{
				p.leftGrab(details.object->getEnd());
			}
		}
	}
	else
	{
		p.releaseLeft();
	}

	if (Application::IsKeyPressed('E') || RT > 0)
	{
		if (!p.isRightGrabbing())
		{
			CollisionDetails details = manager->getEnviromentalCollision(p.getRightHand());
			if (details.result.collided)
			{
				p.rightGrab(details.object->getEnd());
			}
		}
	}
	else
	{
		p.releaseRight();
	}





	// general physics

	//if (Application::IsKeyPressed('G'))
	//manager->resolveCollisions();
	manager->updateObjects();
	manager->updateSprings();




	// camera

	if (camX || camY)
	{
		camera.changeYaw(camX, dt);
		camera.changePitch(camY, dt);
		camera.setAuto(false);
	}
	else if (camera.isAuto())
	{
		Vector3 offset, d;
		if (p.isLeftGrabbing())
			offset = p.getRightArm()->getCenter() - center;
		else if (p.isRightGrabbing())
			offset = p.getLeftArm()->getCenter() - center;

		d = Vector3(fabs(offset.z), -1, fabs(offset.x)).Normalized();
		d.x *= 200; d.y = -50;  d.z *= 200;

		// camera.moveTo(center + d + offset * 10, dt);
	}

	if (Application::IsKeyPressed(VK_UP))
		camera.changePitch(-1, dt);
	if (Application::IsKeyPressed(VK_DOWN))
		camera.changePitch(1, dt);
	if (Application::IsKeyPressed(VK_LEFT))
		camera.changeYaw(1, dt);
	if (Application::IsKeyPressed(VK_RIGHT))
		camera.changeYaw(-1, dt);

	//camera.move(dt);
	Vector3 target = Vector3(int(center.x / 5) * 5, int(center.y / 5) * 5, int(center.z / 5) * 5);
	camera.setTarget(target);
}

void MainScene::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	Vector3 left  = p.getLeftHand()->getCenter();
	Vector3 right = p.getRightHand()->getCenter();
	float yaw = camera.getYaw();
	float pitch = camera.getPitch();

	modelStack.PushMatrix();
	{
		modelStack.Translate(left.x, left.y + 5, left.z);

		modelStack.Rotate(270 - yaw, 0, 1, 0);
		modelStack.Rotate(pitch, 1, 0, 0);

		modelStack.Scale(3, 3, 3);

		Color color = p.isLeftGrabbing() ? Color(.9, .9, 0) : Color(1, 1, 1);
		renderText(models[TEXT], "LT", color);
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	{
		modelStack.Translate(right.x, right.y + 5, right.z);

		modelStack.Rotate(270 - yaw, 0, 1, 0);
		modelStack.Rotate(pitch, 1, 0, 0);

		modelStack.Scale(3, 3, 3);

		Color color = p.isRightGrabbing() ? Color(0, .9, .9) : Color(1, 1, 1);
		renderText(models[TEXT], "RT", color);
	}
	modelStack.PopMatrix();

	Vector3 center = p.getBody()->getCenter();
	std::string content;
	content += std::to_string((int)center.x) + " / " + std::to_string((int)center.y) + " / " + std::to_string((int)center.z) + "\n";
	content += std::to_string(yaw) + " / " + std::to_string(pitch);
	renderTextOnScreen(models[TEXT], content, Color(0, 1, 0), 1, 2, 2);
}

void MainScene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

Camera& MainScene::getCamera() 
{
	return this->camera;
}