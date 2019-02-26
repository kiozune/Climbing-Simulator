#include "BlockGenerator.h"

BlockGenerator* BlockGenerator::instance = 0;

BlockGenerator::BlockGenerator()
{
	head = nullptr;
	tail = nullptr;
	numBlocks = 5;
}

BlockGenerator * BlockGenerator::GetInstance()
{
	if (instance == nullptr)
		instance = new BlockGenerator();
	return instance;
}

void BlockGenerator::generateBlocks(int offsetPos)
{
	Vector3 currentPos = { 0,0,0 };

	// first block
	{
		block *temp = new block;
		temp->setVector3(currentPos);
		temp->setPrevious(nullptr);
		head = temp;
		tail = temp;
	}
	
	std::ifstream myFile("Level/levels.txt");
	std::string holdingString;
	if (!myFile.is_open())
	{
		perror("File Error : levels.txt");
		system("pause");
		exit(EXIT_FAILURE);
	}
	else
	{
		const int choice = rand() % NUM_LEVEL;
		while (!myFile.eof())
		{
			std::getline(myFile, holdingString);
			switch (choice)
			{
			case STANDARD:
				if (holdingString[0] == 'A')
					getLevelData(holdingString, currentPos, offsetPos,numBlocks);
				break;
			case INCLINE:
				if (holdingString[0] == 'B')
					getLevelData(holdingString, currentPos, offsetPos,numBlocks);
				break;
			case ROUNDABOUT:
				if (holdingString[0] == 'C')
					getLevelData(holdingString, currentPos, offsetPos,numBlocks);
				break;
			default:
				printf("Error at geneerateBlocks getLevelData");
				break;
			}
		}
	}

	// last block
	{
		block *temp = new block;
		Vector3 pos = tail->getVector3();
		pos.x += 1;
		temp->setVector3(pos);
		temp->setPrevious(tail);
		tail->setNext(temp);
		tail = temp;
	}
}

void BlockGenerator::getLevelData(std::string val, Vector3 pos, int offset, int cubes)
{
	for (int i = 1; i < val.size(); ++i)
	{
		block *temp = new block;
		switch (val[i])
		{
		case '0': // Left
			pos.x -= offset;
			temp->setVector3(pos);
			temp->populateNode(cubes, offset);
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			break;
		case '1': // right
			pos.x += offset;
			temp->setVector3(pos);
			temp->populateNode(cubes, offset);
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			break;
		case '2': // Forward
			pos.z += offset;
			temp->setVector3(pos);
			temp->populateNode(cubes, offset);
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			break;
		case '3': // backward
			pos.z -= offset;
			temp->setVector3(pos);
			temp->populateNode(cubes, offset);
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			break;
		case '4': // upward
			pos.y += offset;
			temp->setVector3(pos);
			temp->populateNode(cubes, offset);
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			break;
		default:
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