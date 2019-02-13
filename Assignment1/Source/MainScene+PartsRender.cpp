#include "MainScene.h"
#include "Utility.h"

void MainScene::renderBone(Bone* bone) 
{
	Vector3 r = bone->getRotation();
	Vector3 c = bone->getCenter();

	modelStack.PushMatrix();
	{
		modelStack.Translate(c.x, c.y, c.z);

		modelStack.Rotate(deg(r.y), 0, 1, 0);
		modelStack.Rotate(deg(r.z), 0, 0, 1);
		//modelStack.Rotate(deg(r.x), 1, 0, 0);
		//modelStack.Rotate(90, 0, 0, 1);

		modelStack.PushMatrix(); 
		{
			modelStack.Scale(bone->getLength(), 0.5, 0.5);
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
		modelStack.Scale(1.5, 1.5, 1.5);
		renderMesh(models[CUBE], true);
	}
	modelStack.PopMatrix();
}