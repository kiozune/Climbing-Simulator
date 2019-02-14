#include "BlockGenerator.h"

BlockGenerator* BlockGenerator::instance = 0;

BlockGenerator::BlockGenerator()
{
	head = nullptr;
	tail = nullptr;
	currentPos = Vector3(0, 0, 0);
}

BlockGenerator * BlockGenerator::GetInstance()
{
	if (instance == nullptr)
		instance = new BlockGenerator();
	return instance;
}

void BlockGenerator::generateBlocks(Vector3 startCoord, int maxHeight, int offsetPos, int boundary)
{
	currentPos = startCoord;
	block *temp = new block;
	temp->setVector3(currentPos);
	temp->setMesh((rand() % NUM_TEMPLATE) + 1);
	temp->setPervious(nullptr); // starting block doesn't have a pervious block 
	if (head == nullptr)
	{
		head = temp;
		tail = temp;
		delete temp;
	}
	temp = new block;
	currentPos.y += offsetPos;
	temp->setVector3(currentPos);
	temp->setMesh((rand() % NUM_TEMPLATE) + 1);
	tail->setNext(temp);
	temp->setPervious(tail);
	tail = temp;
	delete temp;
	while (currentPos.y != maxHeight)
	{
		if (currentPos.y >= maxHeight) // safety check
			break;
		temp = new block;
		switch (int direction = (rand() % 5) + 1) // 1 left 2 right 3 forwad 4 backward 5 upward
		{
		case 1: // left
			// x- 
			if ((currentPos.x >= -boundary && currentPos.x <= boundary) && currentPos.x != tail->getVector3().x)
			{
				currentPos.x -= offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			else
			{
				currentPos.y += offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			break;
		case 2: // right
			// x +
			if ( (currentPos.x >= -boundary && currentPos.x <= boundary) && currentPos.x != tail->getVector3().x)
			{
				currentPos.x += offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			else
			{
				currentPos.y += offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			break;
		case 3: // forward
			// + z
			if ( (currentPos.z >= -boundary && currentPos.z <= boundary) && currentPos.z != tail->getVector3().z)
			{
				currentPos.z += offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			else
			{
				currentPos.y += offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			break;
		case 4: // backward
			// - z
			if ( (currentPos.z >= -boundary && currentPos.z <= boundary) && currentPos.z != tail->getVector3().z )
			{
				currentPos.z -= offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			else
			{
				currentPos.y += offsetPos;
				tail->setNext(temp);
				temp->setMesh((rand() % NUM_TEMPLATE) + 1);
				temp->setVector3(currentPos);
				temp->setPervious(tail);
				tail = temp;
			}
			break;
		case 5: // upwards
			// + y
			currentPos.y += offsetPos;
			tail->setNext(temp);
			temp->setMesh((rand() % NUM_TEMPLATE) + 1);
			temp->setVector3(currentPos);
			temp->setPervious(tail);
			tail = temp;
			break;
		default:
			printf("Error at BlockGenerator / generateBlocks() switch case");
			break;
		}
	}
}

block * BlockGenerator::getHead()
{
	return head;
}

block * BlockGenerator::getTail()
{
	return tail;
}

BlockGenerator::~BlockGenerator()
{
	block* current = head;

	while (current != tail)
	{
		block* temp = current->getNext();
		delete current;
		current = temp;
	}
	delete current;
}