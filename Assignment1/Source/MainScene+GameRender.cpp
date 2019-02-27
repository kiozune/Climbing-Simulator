#include "MainScene.h"

//Renders the world
void MainScene::RenderGame()
{
	modelStack.PushMatrix();
	modelStack.Scale(100, 100, 100);
	// renderMesh(models[SKY_BOX]);
	modelStack.PopMatrix();

	std::vector<Player*> localPlayers = players->getLocalPlayers();
	unsigned size = localPlayers.size();
	Vector3 frameSize = Application::getFrameSize();

	float rows = ceil((float)size / 2.0f);
	float columns = ceil((float)size / rows);

	float windowWidth = frameSize.x / columns;
	float windowHeight = frameSize.y / rows;

	for (int i = 0; i < size; ++i)
	{
		int x = i % 2;
		int y = i / 2;
		glViewport(windowWidth * x, windowHeight * y, windowWidth, windowHeight);
		renderForPlayer(localPlayers[i]);
	}

	if (size == 3)
	{
		std::vector<Player*> all = players->getPlayers();
		glViewport(windowWidth, windowHeight, windowWidth, windowHeight);
		renderTextOnScreen(models[TEXT], "SPECTATOR\nCAM", Color(1, 1, 1), 2, 1, 1);
		renderForPlayer(all[spectatingPlayer]);

		if (elapseTime - prevTime > 5)
		{
			spectatingPlayer++;
			spectatingPlayer = spectatingPlayer % all.size();
			prevTime = elapseTime;
		}
	}
	// renderMesh(models[SKY_BOX]);
	// renderMesh(models[AXES]);

	std::string content;
	content += "FPS : " + std::to_string(fps) + '\n';
	content += "ELT : " + std::to_string(elapseTime);

	renderTextOnScreen(models[TEXT], content, Color(1, 1, 1), 4, 2, 2);
}