#include "DataTransferManager.h"

std::string DataTransferManager::getString(int i)
{
	std::string result = "";
	char sign;
	if (i == 0)
		result = (char)DATA_POS;
	else
		result = (char)(i / abs(i) + 2); // sign
	
	i = abs(i);

	int length = 1;
	while (i != 0)
	{
		result = result + (char)(i % DATA_SIZE + DATA_POS);
		i /= DATA_SIZE;
		length++;
	}

	result = result + (char)DATA_END;

	return result;
}

float DataTransferManager::getFloat(std::string str)
{
	float sign = str[this->iterator] - 2;
	float result = 0;
	int i = 1;
	while (str[this->iterator + i] != DATA_END)
	{
		result += (float)(str[this->iterator + i] - DATA_POS) * pow(DATA_SIZE, i - 1);
		i++;
	}
	this->iterator = this->iterator + i + 1;
	result *= sign;
	return result;
}

std::string DataTransferManager::stringify(Vector3 v)
{
	return getString(v.x) + getString(v.y) + getString(v.z);
}

Vector3 DataTransferManager::parse(std::string& str)
{
	Vector3 result;
	result.x = getFloat(str);
	result.y = getFloat(str);
	result.z = getFloat(str);
	return result;
}

DataTransferManager * DataTransferManager::instance = nullptr;

DataTransferManager * DataTransferManager::getInstance()
{
	if (!instance) instance = new DataTransferManager;
	return instance;
}

DataTransferManager::DataTransferManager() 
{
	this->iterator = 0;
	if (!this->client.start())
		return;
	this->client.connectTo(30000, "172.27.180.32");
}

Client& DataTransferManager::getClient()
{
	return this->client;
}

PlayerData DataTransferManager::getPlayerData(Player& player, unsigned id)
{
	PlayerData data;
	std::vector<Object*> parts = player.getParts();

	data.id = id;

	data.leftFingers = parts[0]->getStart()->getCurrent();
	data.leftWrist = parts[0]->getEnd()->getCurrent();

	data.leftElbow = parts[2]->getEnd()->getCurrent();

	data.rightFingers = parts[3]->getStart()->getCurrent();
	data.rightWrist = parts[3]->getEnd()->getCurrent();

	data.rightElbow = parts[5]->getEnd()->getCurrent();

	data.chest = parts[6]->getStart()->getCurrent();
	data.head = parts[6]->getEnd()->getCurrent();

	data.pelvis = parts[7]->getEnd()->getCurrent();
	
	data.leftFeet = parts[8]->getEnd()->getCurrent();
	data.rightFeet = parts[9]->getEnd()->getCurrent();

	return data;
}

std::string DataTransferManager::stringifyData(PlayerData data)
{
	std::string result = "";
	result += (char)data.id;
	result += stringify(data.leftFingers);
	result += stringify(data.leftWrist);
	result += stringify(data.leftElbow);
	result += stringify(data.rightFingers);
	result += stringify(data.rightWrist);
	result += stringify(data.rightElbow);
	result += stringify(data.head);
	result += stringify(data.chest);
	result += stringify(data.pelvis);
	result += stringify(data.leftFeet);
	result += stringify(data.rightFeet);
	return result;
}

PlayerData DataTransferManager::parseData(std::string str)
{
	this->iterator = 1;
	PlayerData result;
	result.id = (unsigned)str[0];
	result.leftFingers = parse(str);
	result.leftWrist = parse(str);
	result.leftElbow = parse(str);
	result.rightFingers = parse(str);
	result.rightWrist = parse(str);
	result.rightElbow = parse(str);
	result.head = parse(str);
	result.chest = parse(str);
	result.pelvis = parse(str);
	result.leftFeet = parse(str);
	result.rightFeet = parse(str);
	return result;
}