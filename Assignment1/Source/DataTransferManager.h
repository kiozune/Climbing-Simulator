#ifndef DATA_TRANSFER_MANAGER_H
#define DATA_TRANSFER_MANAGER_H

#include "Client.h"
#include "RemotePlayer.h"

#include <string>

#define DATA_END 2
#define DATA_NEG 1
#define DATA_POS 3
#define DATA_SIZE 125

#define MIN_SIZE 33

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
	
	Client& getClient();

	PlayerData getPlayerData(Player&, unsigned);

	std::string stringifyData(PlayerData);
	PlayerData parseData(std::string);

};

#endif