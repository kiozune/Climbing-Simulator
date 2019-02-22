#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "RemotePlayer.h"
#include <vector>

class PlayerManager
{

private:

	static PlayerManager * instance;

	std::vector<Player*> localPlayers;
	std::vector<RemotePlayer*> remotePlayers;

public:

	static PlayerManager * getInstance();

	void fixMissingPlayers();
	Player* createPlayer(unsigned);
	
	void create(std::string);

	std::vector<Player*> getLocalPlayers();
	void addLocalPlayer(Player*);
	void removeLocalPlayer(int);

	std::vector<RemotePlayer*> getRemotePlayers();
	void addRemotePlayer(RemotePlayer*);


	void updateRemote(PlayerData);
};

#endif