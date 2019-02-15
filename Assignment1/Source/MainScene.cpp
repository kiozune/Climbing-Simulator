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
	lights[0].position.Set(0, 10, 0);
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

	camera.Init(Vector3(0, 0, -200), Vector3(0, 1, 0), Vector3());

	Joint* chest = new Joint(Vector3(0, 0, 0));
	Joint* leftWrist = new Joint(Vector3(10, 0, 0));
	Joint* leftFingers = new Joint(Vector3(13, 0, 0));
	Joint* rightWrist = new Joint(Vector3(-10, 0, 0));
	Joint* rightFingers = new Joint(Vector3(-13, 0, 0));
	Joint* head = new Joint(Vector3(0, 5, 0));
	Joint* pelvis = new Joint(Vector3(0, -10, 0));
	Joint* leftFeet = new Joint(Vector3(5, -20, 0));
	Joint* rightFeet = new Joint(Vector3(-5, -20, 0));

	p.setLeftFingers(leftFingers);
	p.setRightFingers(rightFingers);

	std::vector<Joint*> hinges;
	hinges.push_back(new Joint(Vector3(0, 60, 0), true));
	for (int i = 1; i < 10; ++i)
	{
		hinges.push_back(new Joint(Vector3(0, 60 - i * 15, 0)));
		Object* chain = new Object(hinges[i - 1], hinges[i], 10, 3);
		manager->addToEnvironment(chain);
	}

	hinges.clear();
	hinges.push_back(new Joint(Vector3(-50, 60, 0), true));
	for (int i = 1; i < 10; ++i)
	{
		hinges.push_back(new Joint(Vector3(-50, 60 - i * 15, 0)));
		Object* chain = new Object(hinges[i - 1], hinges[i], 10, 3);
		manager->addToEnvironment(chain);
	}

	float mass = 15, size = 2;

	Object* leftHand = new Object(leftFingers, leftWrist, mass, size + 1);
	Object* leftArm = new Object(chest, leftWrist, mass, size);
	Object* rightHand = new Object(rightFingers, rightWrist, mass, size);
	Object* rightArm = new Object(chest, rightWrist, mass, size);
	Object* neck = new Object(chest, head, mass, size);
	Object* body = new Object(chest, pelvis, mass, size);
	Object* leftLeg = new Object(pelvis, leftFeet, mass, size);
	Object* rightLeg = new Object(pelvis, rightFeet, mass, size);

	p.setLeftHand(leftHand);
	p.setLeftArm(leftArm);
	p.setRightHand(rightHand);
	p.setRightArm(rightArm);
	p.setBody(body);

	manager->addObject(leftHand);
	manager->addObject(leftArm);
	manager->addObject(rightHand);
	manager->addObject(rightArm);
	manager->addObject(neck);
	manager->addObject(body);
	manager->addObject(leftLeg);
	manager->addObject(rightLeg);

	for (int i = 0; i < 5; ++i)
	{
		Object* box = new Object(Vector3(10, 10, 100), Vector3(11 + i * 20, -5 + i * 5, 0), 0, false);
		manager->addToEnvironment(box);
	}

	Object* platform = new Object(Vector3(100, 10, 100), Vector3(0, -70, 0), 0, false);
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

	const float* analog = Application::getControllerAnalog();





	// Player swing

	Vector3 curr = Application::GetMousePosition();
	Vector3 diff = prevMousePosition - curr;
	if (analog[0] || analog[1]) diff = Vector3(analog[0], analog[1], 0) * 200;
	prevMousePosition = curr;

	Vector3 center = p.getBody()->getCenter();
	Vector3 dir = center - camera.position;
	std::cout << dir.x << ' ' << dir.z << std::endl;
	float yaw = atan(dir.x / dir.z);
	dir.z /= abs(dir.z);
	Mtx44 rotation; rotation.SetToRotation(deg(yaw), 0, 1, 0);

	Vector3 impulse = rotation * Vector3(diff.x * -dir.z, diff.Length() * 1.5, diff.y * dir.z);


	if (p.isLeftGrabbing())
		manager->applyImpulse(p.getRightArm(), impulse, dt);

	if (p.isRightGrabbing())
		manager->applyImpulse(p.getLeftArm(), impulse, dt);





	// grabbing

	if (Application::IsKeyPressed('Q') || analog[4] > 0)
	{
		if (!p.isLeftGrabbing())
		{
			for (Object* obj : manager->getEnvironment())
			{
				if (p.getLeftHand()->getBoundingBox().didCollideWith(obj->getBoundingBox()))
				{
					p.leftGrab(obj->getEnd());
					break;
				}
			}
		}
	}
	else
	{
		p.releaseLeft();
	}

	if (Application::IsKeyPressed('E') || analog[5] > 0)
	{
		if (!p.isRightGrabbing())
		{
			for (Object* obj : manager->getEnvironment())
			{
				if (p.getRightHand()->getBoundingBox().didCollideWith(obj->getBoundingBox()))
				{
					p.rightGrab(obj->getEnd());
					break;
				}
			}
		}
	}
	else
	{
		p.releaseRight();
	}





	// general physics

	//if (Application::IsKeyPressed('G'))
	manager->applyGravity(dt);
	manager->updateObjects(dt);
	manager->updateSprings();





	// camera

	float x = analog[2], y = analog[3];

	if (x || y)
	{
		camera.moveTo(center + Vector3(-x, 0, y).Normalized() * 200, dt * 2);
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

		camera.moveTo(center + d + offset * 10, dt);
	}

	//camera.move(dt);
	camera.setTarget(center);
}

void MainScene::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get reference to camera based on state
	Vector3 target = camera.getTarget();
	Vector3 up = camera.getUp();

	viewStack.LoadIdentity(); 
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
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

	for (Object* obj : manager->getObjects()) {
		renderObject(obj);
		//renderBoundingBox(obj->getBoundingBox());
	}

	std::string text = "AUTO CAM : ";
	text += (camera.isAuto() ? "YES" : "NO");
	renderTextOnScreen(models[TEXT], text, Color(1, 1, 1), 2, 2, 2);
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