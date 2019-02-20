#include "BlockGenerator.h"
/*
Rule for PCG sp2 walker algorithm
Rule 1 - Doesn't go back the same way it came from
Rule 2 - Choose a random length from maximum allowed length and draw it at given direction
Rule 3 - Avoids the edges of the map
Rule 4 - Spawn block till it runs out
Rule 5 - Check with world space if object is there
*/

BlockGenerator* BlockGenerator::instance = 0;

BlockGenerator::BlockGenerator()
{
	head = nullptr;
	tail = nullptr;
	oppDirection = -1;
}

BlockGenerator * BlockGenerator::GetInstance()
{
	if (instance == nullptr)
		instance = new BlockGenerator();
	return instance;
}

void BlockGenerator::generateBlocks()
{
	Vector3 currentPos = { 0,0,0 };

	// first block
	{
		block *temp = new block;
		temp->setVector3(currentPos);
		temp->setMesh(STARTING);
		temp->setPrevious(nullptr);
		head = temp;
		tail = temp;
	}
}

void BlockGenerator::generateBlocks(int valBlocks, int maxHeight, int maxLength, int offsetPos, int boundary)
{
	Vector3 currentPos = { 0,0,0 };
	int blockTemp = valBlocks; // blockTemp val will need to decrease during whileloop
	
	// first block
	{
		block *temp = new block;
		temp->setVector3(currentPos);
		temp->setMesh();
		temp->setPrevious(nullptr); // starting block doesn't have a pervious block
		head = temp;
		tail = temp;
	}

	while (blockTemp) // Rule 4
	{
		switch (randomDirection(oppDirection))
		{
		case LEFT:
		{
			block *temp = new block;
			if ((currentPos.x -= offsetPos) < -boundary && goUP) // hit boundary, go up
			{
				
				currentPos.x++;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos;
				oppDirection = RIGHT;
				goUP = false;
				break;
			}
			else if (checkWorldSpace(currentPos)) // hit object in world, go up
			{
				currentPos.x++;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos;
				oppDirection = RIGHT;
				goUP = false;
				break;
			}
			temp->setVector3(currentPos);
			temp->setMesh();
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			--blockTemp;
			oppDirection = RIGHT;
			goUP = true;
			break;
		}
		case RIGHT:
		{
			block *temp = new block;
			if ((currentPos.x += offsetPos) > boundary && goUP)
			{
				currentPos.x--;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos + 1;
				oppDirection = LEFT;
				goUP = false;
				break;
			}
			else if (checkWorldSpace(currentPos))
			{
				currentPos.x--;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos + 1;
				oppDirection = LEFT;
				goUP = false;
				break;
			}
			temp->setVector3(currentPos);
			temp->setMesh();
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			--blockTemp;
			oppDirection = LEFT;
			goUP = true;
			break;
		}
		case FORWARD:
		{
			block *temp = new block;
			if ((currentPos.z += offsetPos) > boundary && goUP) // hit boundary, go up
			{
				currentPos.z--;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos + 1;
				oppDirection = BACKWARD;
				goUP = false;
				break;
			}
			else if (checkWorldSpace(currentPos)) // hit object in the world space, go up
			{
				currentPos.z--;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos + 1;
				oppDirection = BACKWARD;
				goUP = false;
				break;
			}
			temp->setVector3(currentPos);
			temp->setMesh();
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			--blockTemp;
			oppDirection = BACKWARD;
			goUP = true;
			break;
		}
		case BACKWARD:
		{
			block *temp = new block;
			if ((currentPos.z -= offsetPos) < -boundary && goUP)
			{
				currentPos.z++;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos + 1;
				oppDirection = FORWARD;
				goUP = false;
				break;
			}
			else if (checkWorldSpace(currentPos))
			{
				currentPos.z++;
				currentPos.y += offsetPos + 1;
				temp->setVector3(currentPos);
				temp->setMesh();
				temp->setPrevious(tail);
				tail->setNext(temp);
				tail = temp;
				blockTemp -= offsetPos + 1;
				oppDirection = FORWARD;
				goUP = false;
				break;

			}
			temp->setVector3(currentPos);
			temp->setMesh();
			temp->setPrevious(tail);
			tail->setNext(temp);
			tail = temp;
			--blockTemp;
			oppDirection = FORWARD;
			goUP = true;
			break;
		}
		default:
			break;
		}
		printf("%d\n",oppDirection);
	}
	//while (blockTemp) // Rule 4 | loop till there is no more blocks to build
	//{
	//	switch (randomDirection(lastDirection))
	//	{
	//	case UP:
	//	{
	//		int randomHeight = rand() % maxHeight + 1;
	//		while (randomHeight) // rule 2
	//		{ // rule 3 & 5 doesn't applied to height
	//			if (blockTemp == 0) // rule 4
	//				break;
	//			block* temp = new block;
	//			currentPos.y += offsetPos;
	//			temp->setVector3(currentPos);
	//			temp->setMesh(rand() % NUM_TEMPLATE);
	//			tail->setNext(temp);
	//			temp->setPrevious(tail);
	//			tail = temp;
	//			--blockTemp;
	//			--randomHeight;
	//		}
	//		lastDirection = UP;
	//	}
	//	break;
	//	case LEFT:
	//	{
	//		int randomLength = rand() % maxLength + 1; 
	//		while (randomLength) // rule 2
	//		{
	//			if (checkWorldSpace(currentPos)) // rule 5
	//				break;
	//			else if (currentPos.x <= -boundary) // rule 3
	//				break;
	//			else if (blockTemp == 0) // rule 4
	//				break;
	//			block* temp = new block;
	//			currentPos.x -= offsetPos;
	//			temp->setVector3(currentPos);
	//			temp->setMesh(rand() % NUM_TEMPLATE);
	//			tail->setNext(temp);
	//			temp->setPrevious(tail);
	//			tail = temp;
	//			--blockTemp;
	//			--randomLength;
	//		}
	//		lastDirection = LEFT;
	//	}
	//	break;
	//	case RIGHT:
	//	{
	//		int randomLength = rand() % maxLength + 1;
	//		while (randomLength) // rule 2
	//		{
	//			if (checkWorldSpace(currentPos)) // rule 5
	//				break;
	//			else if (currentPos.x >= boundary) // rule 3
	//				break;
	//			else if (blockTemp == 0) // rule 4
	//				break;
	//			block* temp = new block;
	//			currentPos.x += offsetPos;
	//			temp->setVector3(currentPos);
	//			temp->setMesh(rand() % NUM_TEMPLATE);
	//			tail->setNext(temp);
	//			temp->setPrevious(tail);
	//			tail = temp;
	//			--blockTemp;
	//			--randomLength;
	//		}
	//		lastDirection = RIGHT;
	//	}
	//	break;
	//	case FORWARD:
	//	{
	//		int randomLength = rand() % maxLength + 1;
	//		while (randomLength) // rule 2
	//		{
	//			if (checkWorldSpace(currentPos)) // rule 5
	//				break;
	//			else if (currentPos.z >= boundary) // rule 3
	//				break;
	//			else if (blockTemp == 0) // rule 4
	//				break;
	//			block * temp = new block;
	//			currentPos.z += offsetPos;
	//			temp->setVector3(currentPos);
	//			temp->setMesh(rand() % NUM_TEMPLATE);
	//			tail->setNext(temp);
	//			temp->setPrevious(tail);
	//			tail = temp;
	//			--blockTemp;
	//			--randomLength;
	//		}
	//		lastDirection = FORWARD;
	//	}
	//	break;
	//	case BACKWARD:
	//	{
	//		int randomLength = rand() % maxLength + 1;
	//		while (randomLength) // rule 2
	//		{
	//			if (checkWorldSpace(currentPos)) // rule 5
	//				break;
	//			else if (currentPos.z <= boundary) // rule 3
	//				break;
	//			else if (blockTemp == 0) // rule 4
	//				break;
	//			block * temp = new block;
	//			currentPos.z -= offsetPos;
	//			temp->setVector3(currentPos);
	//			temp->setMesh(rand() % NUM_TEMPLATE);
	//			tail->setNext(temp);
	//			temp->setPrevious(tail);
	//			tail = temp;
	//			--blockTemp;
	//			--randomLength;
	//		}
	//		lastDirection = BACKWARD;
	//	}
	//	break;
	//	default:
	//		printf("ERROR at switch case > blockGenerator.cpp > generateblocks function");
	//		break;
	//	}
	//	printf("%d\n",lastDirection);
	//}

}

int BlockGenerator::randomDirection(int oppDir)
{
	int temp = 0;

	do
	{
		temp = rand() % NUM_DIRECTION; // 0 to 3
	} while (temp == oppDir); // Rule 1

	return temp;
}

bool BlockGenerator::checkWorldSpace(Vector3 pos)
{
	block* current = head;

	while (current != tail)
	{
		block* temp = current->getNext();
		if (current->getVector3() == pos)
			return true;
		current = temp;
	}
	return false;
}

void BlockGenerator::getLevelData()
{
	sizeX = 64; // predeteremine?
	sizeY = 3; // predetermine?
	level = new int[sizeX * sizeY]; // level[i*sizeY+j]

	std::ifstream myFile("levels.txt");
	int i = 0;
	int j = 0;
	char value = 0;
	if (!myFile.is_open())
	{
		perror("File Error : levels.txt");
		system("pause");
		exit(EXIT_FAILURE);
	}
	else
	{
		while (myFile.get(value))
		{

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

	delete[] level;
}