#ifndef REMOTE_PLAYER_H
#define REMOTE_PLAYER_H

#include "Player.h"

#include <string>

class RemotePlayer : public Player
{
	
public:

	void update(std::string);

};

#endif