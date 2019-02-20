#include "Light.h"

#include "GL/glew.h"
#include <string>

#include "Utility.h"

#include <iostream>

unsigned Light::count = 0;

Light::Light() {
	this->type = POINT;
	this->position.Set(0, 0, 0);
	this->color.Set(1, 1, 1);
	this->power = 1.f;
	this->kC = 1.f;
	this->kL = 0.01f;
	this->kQ = 0.001f;
	/*this->cosCutoff = cos(rad(45));
	this->cosInner = cos(rad(30));*/
	this->cosCutoff = this->cosInner = 0.8f;
	this->exponent = 3.f;
	this->spotDirection.Set(0.f, 5.f, 0.f);
}

std::string Light::getGLName(std::string end) {
	std::string superscript = std::string("lights[") + std::to_string(count) + "].";
	superscript += end;

	return superscript;
}

void Light::getUniformLocation(const unsigned int id) {
	parameters[L_TYPE] = glGetUniformLocation(id, getGLName("type").c_str());
	parameters[L_POSITION] = glGetUniformLocation(id, getGLName("position_cameraspace").c_str());
	parameters[L_COLOR] = glGetUniformLocation(id, getGLName("color").c_str());
	parameters[L_POWER] = glGetUniformLocation(id, getGLName("power").c_str());
	parameters[L_KC] = glGetUniformLocation(id, getGLName("kC").c_str());
	parameters[L_KL] = glGetUniformLocation(id, getGLName("kL").c_str());
	parameters[L_KQ] = glGetUniformLocation(id, getGLName("kQ").c_str());
	parameters[L_COSCUTOFF] = glGetUniformLocation(id, getGLName("cosCutoff").c_str());
	parameters[L_COSINNER] = glGetUniformLocation(id, getGLName("cosInner").c_str());
	parameters[L_EXPONENT] = glGetUniformLocation(id, getGLName("exponent").c_str());
	parameters[L_SPOTDIRECTION] = glGetUniformLocation(id, getGLName("spotDirection").c_str());
	count++;
}

void Light::setUniform() {
	glUniform1i(parameters[L_TYPE], type);
	glUniform3fv(parameters[L_COLOR], 1, &color.r);
	glUniform1f(parameters[L_POWER], power);
	glUniform1f(parameters[L_KC], kC);
	glUniform1f(parameters[L_KL], kL);
	glUniform1f(parameters[L_KQ], kQ);
	glUniform1f(parameters[L_COSCUTOFF], cosCutoff);
	glUniform1f(parameters[L_COSINNER], cosInner);
	glUniform1f(parameters[L_EXPONENT], exponent);
	glUniform3fv(parameters[L_SPOTDIRECTION], 1, &spotDirection.x);
}

