#ifndef REMOTE_PLAYER_H
#define REMOTE_PLAYER_H

#include "Player.h"
#include "PlayerData.h"

class RemotePlayer : public Player
{
	
private:

	unsigned id;

public:

	unsigned getId();
	void setId(unsigned);

	void update(PlayerData);

};

#endif