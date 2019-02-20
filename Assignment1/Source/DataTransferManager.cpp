#include "DataTransferManager.h"

void DataTransferManager::pad(std::string& raw, int length)
{
	for (int i = 0; i < length; ++i)
		raw = (char)0 + raw;
}

std::string DataTransferManager::getString(int i)
{
	std::string result = "";
	char sign;
	if (i == 0)
		sign = (char)2;
	else
		sign = (char)(i / abs(i) + 1); // sign
	
	i = abs(i);

	int length = 1;
	while (i != 0)
	{
		result += (char)(i % 127);
		i /= 127;
		length++;
	}

	pad(result, buffSize - length);
	result = sign + result;

	return result;
}

float DataTransferManager::getFloat(std::string str, int i)
{
	float result = 0;
	int start = i * buffSize;
	for (i = start + 1; i < start + buffSize; ++i)
		result += (float)str[i];
	result *= (float)(str[start] - 1);
	return result;
}

std::string DataTransferManager::stringify(Vector3 v)
{
	return getString(v.x) + getString(v.y) + getString(v.z);
}

Vector3 DataTransferManager::parse(std::string& str, int i)
{
	return Vector3(getFloat(str, i), getFloat(str, i + 1), getFloat(str, i + 2));
}

DataTransferManager * DataTransferManager::instance = nullptr;

DataTransferManager * DataTransferManager::getInstance()
{
	if (!instance) instance = new DataTransferManager;
	return instance;
}

PlayerData DataTransferManager::getPlayerData(Player& player)
{
	PlayerData data;
	std::vector<Object*> parts = player.getParts();

	data.leftFingers = parts[0]->getStart()->getCurrent() + Vector3(0, 10, 0);
	data.leftWrist = parts[0]->getEnd()->getCurrent() + Vector3(0, 10, 0);

	data.leftElbow = parts[2]->getEnd()->getCurrent() + Vector3(0, 10, 0);

	data.rightFingers = parts[3]->getStart()->getCurrent() + Vector3(0, 10, 0);
	data.rightWrist = parts[3]->getEnd()->getCurrent() + Vector3(0, 10, 0);

	data.rightElbow = parts[5]->getEnd()->getCurrent() + Vector3(0, 10, 0);

	data.chest = parts[6]->getStart()->getCurrent() + Vector3(0, 10, 0);
	data.head = parts[6]->getEnd()->getCurrent() + Vector3(0, 10, 0);

	data.pelvis = parts[7]->getEnd()->getCurrent() + Vector3(0, 10, 0);
	
	data.leftFeet = parts[8]->getEnd()->getCurrent() + Vector3(0, 10, 0);
	data.rightFeet = parts[9]->getEnd()->getCurrent() + Vector3(0, 10, 0);

	return data;
}

std::string DataTransferManager::stringify(PlayerData data)
{
	std::string result = "";
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

PlayerData DataTransferManager::parse(std::string str)
{
	//buffSize = str.size() / 32;
	PlayerData result;
	result.leftFingers = parse(str, 0);
	result.leftWrist = parse(str, 3);
	result.leftElbow = parse(str, 6);
	result.rightFingers = parse(str, 9);
	result.rightWrist = parse(str, 12);
	result.rightElbow = parse(str, 15);
	result.head = parse(str, 18);
	result.chest = parse(str, 21);
	result.pelvis = parse(str, 24);
	result.leftFeet = parse(str, 27);
	result.rightFeet = parse(str, 30);
	return result;
}