#ifndef DATA_TRANSFER_MANAGER_H
#define DATA_TRANSFER_MANAGER_H

#include "Client.h"
#include "RemotePlayer.h"

#include <string>

#define DATA_END 1
#define DATA_NEG 0
#define DATA_POS 2
#define DATA_SIZE 126

class DataTransferManager
{
	
private:

	static DataTransferManager * instance;

	Client client;

	int iterator;
	std::string getString(int);
	float getFloat(std::string);

	std::string stringify(Vector3);
	Vector3 parse(std::string&);

public:

	static DataTransferManager * getInstance();


	DataTransferManager();
	~DataTransferManager();

	Client& getClient();

	PlayerData getPlayerData(Player&);

	std::string stringifyData(PlayerData);
	PlayerData parseData(std::string);

};

#endif