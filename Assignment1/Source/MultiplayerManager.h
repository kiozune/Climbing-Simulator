#ifndef MULTIPLAYER_MANAGER_H
#define MULTIPLAYER_MANAGER_H

#include <string>

class MultiplayerManager
{

private:

	static MultiplayerManager * instance;

public:

	static MultiplayerManager * getInstance();
	
	void connectTo(std::string);
	void end();

	void receive();
	void send();

};

#endif