#include "MainScene.h"
#include "Utility.h"

void MainScene::renderObject(Object* obj) 
{
	Vector3 r = obj->getRotation();
	Vector3 c = obj->getCenter();

	modelStack.PushMatrix();
	{
		modelStack.Translate(c.x, c.y, c.z);

		modelStack.Rotate(deg(r.y), 0, 1, 0);
		modelStack.Rotate(deg(r.z), 0, 0, 1);

		modelStack.PushMatrix(); 
		{
			modelStack.Scale(obj->getLength(), 2, 2);
			renderMesh(models[CUBE], true);
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();
}

void MainScene::renderJoint(Joint* joint)
{
	Vector3 position = joint->getCurrent();
	modelStack.PushMatrix();
	{
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Scale(4, 4, 4);
		renderMesh(models[CUBE], true);
	}
	modelStack.PopMatrix();
}

void MainScene::renderBoundingBox(BoundingBox bb) 
{
	for (Vector3 v : bb.getVertices())
	{
		modelStack.PushMatrix();
		modelStack.Translate(v.x, v.y, v.z);
		renderMesh(models[CUBE]);
		modelStack.PopMatrix();
	}
}