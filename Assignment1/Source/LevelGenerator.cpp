#include "LevelGenerator.h"

LevelGenerator::LevelGenerator()
{
	head = nullptr;
	tail = nullptr;
}

void LevelGenerator::generateLevel(Vector3 minCoord,Vector3 maxCoord, int offsetPos)
{
	currentY = minCoord.y + offsetPos; // start at the bottom row + posOffset for each model size
	currentX = (rand() % (int)maxCoord.x) + 1; // Rand for a column to start on
	level *temp = new level;
	temp->setVector3((Vector3)(currentX,currentY,0)); // set the position for printing
	temp->setMesh("skybox"); // set the start node (!!! change the obj when rdy !!!)
	temp->setDOWN(nullptr); // starting node doesn't go downward.
	if (currentX == minCoord.x)
	{
		temp->setLEFT(nullptr);
		temp->setDOWN(nullptr);
	}
	else if (currentX == maxCoord.x)
	{
		temp->setRIGHT(nullptr);
		temp->setDOWN(nullptr);
	}
	if (head == nullptr)
	{
		head = temp;
		tail = temp;
		delete temp;
	}
	while (1)
	{
		temp = new level;
		switch (int direction = (rand() % 3) + 1)
		{
		case 1: // left
			if (currentX <= minCoord.x) // cannot proceed to the left so go upwards
			{
				if (currentY >= maxCoord.y)
				{
					return; // return out of the function. Finish building the level.
				}
				else
				{
					currentY += offsetPos;
					tail->setUP(temp); // pervious node set the direction accordingly
					temp->setMesh((rand() % 3) + 1);
					temp->setVector3(Vector3(currentX, currentY, 0)); // set the coords to the new node
					temp->setDOWN(tail);
					tail = temp;
				}
			}
			else
			{
				currentX -= offsetPos;
				tail->setLEFT(temp);
				temp->setMesh((rand() % 3) + 1);
				temp->setVector3(Vector3(currentX, currentY, 0));
				temp->setRIGHT(tail);
				tail = temp;
			}
			break;
		case 2: // right
			if (currentX >= maxCoord.x)
			{
				if (currentY >= maxCoord.y)
					return;
				else
				{
					currentY += offsetPos;
					tail->setUP(temp);
					temp->setMesh((rand() % 3) + 1);
					temp->setVector3(Vector3(currentX, currentY, 0));
					temp->setDOWN(tail);
					tail = temp;
				}
			}
			else
			{
				currentX += offsetPos;
				tail->setRIGHT(temp);
				temp->setMesh((rand() % 3) + 1);
				temp->setVector3(Vector3(currentX, currentY, 0));
				temp->setLEFT(tail);
				tail = temp;
			}
			break;
		case 3: // up
			if (currentY >= maxCoord.y)
				return;
			else
			{
				currentY += offsetPos;
				tail->setUP(temp);
				temp->setMesh((rand() % 3) + 1);
				temp->setVector3(Vector3(currentX, currentY, 0));
				temp->setDOWN(tail);
				tail = temp;
			}
			break;
		default:
			printf("Error at LevelGenerator / generateLevel() switch case");
			break;
		}
	}
}

LevelGenerator::~LevelGenerator()
{
	level* current = head;

	while (current != tail)
	{
		level* temp = current->getDirection();
		delete current;
		current = temp;
	}
	delete current;
}
