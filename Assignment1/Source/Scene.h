#ifndef SCENE_H
#define SCENE_H

#include "MatrixStack.h"
#include "MeshBuilder.h"
#include "Light.h"
#include "Camera.h"
class Scene
{
public:
	Scene() {}
	~Scene() {}

	enum UNIFORM_TYPE
	{
		U_MVP = 0,

		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,

		U_LIGHT_DEPTH_MVP_FIRSTPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MVP,

		U_SHADOWMAP,
		U_LIGHTMATRIX,

		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_NUMLIGHTS,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_SHADOW_ENABLED,
		U_SHADOW_COLOR,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	float toRad(const int x);

	float v(int i = 1);
	float c(int i = 1);

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif