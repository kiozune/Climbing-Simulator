#ifndef REMOTE_PLAYER_H
#define REMOTE_PLAYER_H

#include "Player.h"
#include "PlayerData.h"

class RemotePlayer : public Player
{
public:

	void update(PlayerData);

};

#endif