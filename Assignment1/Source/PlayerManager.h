#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "RemotePlayer.h"
#include <vector>

class PlayerManager
{

private:

	static PlayerManager * instance;

	std::vector<Player*> players;
	std::vector<Player*> localPlayers;
	std::vector<RemotePlayer*> remotePlayers;

public:

	static PlayerManager * getInstance();

	void fixMissingPlayers();

	void createRemotePlayers(std::string, unsigned);
	RemotePlayer* createRemotePlayer(unsigned, unsigned);
	Player* createPlayer(unsigned);

	std::vector<Player*> getPlayers();

	std::vector<Player*> getLocalPlayers();
	void addLocalPlayer(Player*);

	std::vector<RemotePlayer*> getRemotePlayers();
	void addRemotePlayer(RemotePlayer*);

	void updateRemote(PlayerData);

	void destroy();

};

#endif