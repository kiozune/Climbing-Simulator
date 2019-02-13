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

	camera.Init(Vector3(0, 0, -100), Vector3(0, 1, 0), 0, 90, 10, 10);

	Joint* chest	 = new Joint(Vector3(0, 0, 0));
	Joint* leftHand	 = new Joint(Vector3(12, 0, 0));
	Joint* rightHand = new Joint(Vector3(-12, 0, 0));
	Joint* head		 = new Joint(Vector3(0, 5, 0));
	Joint* pelvis	 = new Joint(Vector3(0, -10, 0));
	Joint* leftFeet  = new Joint(Vector3(5, -20, 0));
	Joint* rightFeet = new Joint(Vector3(-5, -20, 0));

	p.setLeftHand(leftHand);
	p.setRightHand(rightHand);

	Object* leftArm  = new Object(25, new Bone(chest, leftHand));
	Object* rightArm = new Object(25, new Bone(chest, rightHand));
	Object* neck	 = new Object(25, new Bone(chest, head));
	Object* body	 = new Object(25, new Bone(chest, pelvis));
	Object* leftLeg  = new Object(25, new Bone(pelvis, leftFeet));
	Object* rightLeg = new Object(25, new Bone(pelvis, rightFeet));

	p.setLeftArm(leftArm);
	p.setRightArm(rightArm);	
	p.setBody(body);

	manager->addObject(leftArm);
	manager->addObject(rightArm);
	manager->addObject(neck);
	manager->addObject(body);
	manager->addObject(leftLeg);
	manager->addObject(rightLeg);

	Spring* topLeft = new Spring(head, leftHand, 0.2, 1.5, 0.2);
	Spring* topRight = new Spring(head, rightHand, 0.2, 1.5, 0.2);
	Spring* mid = new Spring(head, pelvis, 1, 1, 0.2);
	Spring* midLeft = new Spring(pelvis, leftHand, 0.2, 1.5, 0.2);
	Spring* midRight = new Spring(pelvis, rightHand, 0.2, 1.5, 0.2);
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
		dt /= 10;

	p.releaseLeft();
	p.releaseRight();

	if (Application::IsKeyPressed('Q'))
		p.grabLeft();

	if (Application::IsKeyPressed('E'))
		p.grabRight();


	Vector3 curr = Application::GetMousePosition();
	Vector3 diff = prevMousePosition - curr;
	prevMousePosition = curr;

	Vector3 impulse = Vector3(diff.x, diff.Length(), diff.y);

	if (p.getLeftHand()->isFixed())
		manager->applyImpulse(p.getRightArm(), impulse, dt);

	if (p.getRightHand()->isFixed())
		manager->applyImpulse(p.getLeftArm(), impulse, dt);

	//if (Application::IsKeyPressed('G'))
	manager->applyGravity(dt);
	manager->updateObjects();
	manager->updateSprings();

	camera.move(dt);
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

	modelStack.Scale(0.5, 0.5, 0.5);
	for (Object* obj : manager->getObjects()) {
		Bone* bone = obj->getBone();
		renderBone(bone);
		renderJoint(bone->getStart());
		renderJoint(bone->getEnd());
	}

	if (Application::IsKeyPressed('P'))
	{
		Bone* bone = manager->getObjects()[0]->getBone();
		Vector3 r = bone->getRotation();
		std::string content = "Angle : " + std::to_string(r.x) + "/" + std::to_string(r.y) + "/" + std::to_string(r.z) + "\n";
		content += "Length : " + std::to_string(bone-> getLength()) + "\n";
		Vector3 c = bone->getCenter();
		content += "Center : " + std::to_string((float)c.x) + "/" + std::to_string((float)c.y) + "/" + std::to_string((float)c.z) + "\n";
		Vector3 a = bone->getStart()->getCurrent(), b = bone->getEnd()->getCurrent();
		content += "Start  : " + std::to_string((float)a.x) + "/" + std::to_string((float)a.y) + "/" + std::to_string((float)a.z) + "\n";
		content += "End    : " + std::to_string((float)b.x) + "/" + std::to_string((float)b.y) + "/" + std::to_string((float)b.z);
		renderTextOnScreen(models[TEXT], content, Color(1, 1, 1), 2, 2, 2);
	}
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