#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "RemotePlayer.h"
#include <vector>

class PlayerManager
{

private:

	static PlayerManager * instance;

	Player* main;
	std::vector<Player*> players;
	std::vector<RemotePlayer*> remotePlayers;

public:

	static PlayerManager * getInstance();

	Player* getMain();
	void setMain(Player*);

	std::vector<Player*> getPlayers();
	void addPlayer(Player*);

	void addRemotePlayer(RemotePlayer*);

	void updateRemote(PlayerData);
};

#endif