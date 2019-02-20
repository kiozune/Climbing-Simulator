#ifndef DATA_TRANSFER_MANAGER_H
#define DATA_TRANSFER_MANAGER_H

#include "RemotePlayer.h"

#include <string>

class DataTransferManager
{
	
private:

	static DataTransferManager * instance;

	int buffSize = 6;

	void pad(std::string&, int);
	std::string getString(int);
	float getFloat(std::string, int i);

	std::string stringify(Vector3);
	Vector3 parse(std::string&, int);

public:

	static DataTransferManager * getInstance();

	PlayerData getPlayerData(Player&);

	std::string stringify(PlayerData);
	PlayerData parse(std::string);

};

#endif