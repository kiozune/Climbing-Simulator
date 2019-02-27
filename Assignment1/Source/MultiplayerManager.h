#ifndef MULTIPLAYER_MANAGER_H
#define MULTIPLAYER_MANAGER_H

#include <string>
#include <thread>

#include "Server.h"

class MultiplayerManager
{

private:

	static MultiplayerManager * instance;

	Server server;

public:

	static MultiplayerManager * getInstance();

	Server& getSever();
	void startSever();

	void connectTo(std::string);
	void end();

	void receive();
	void send();

};

#endif