#ifndef BOT_H
#define BOT_H

#include "Player.h"
#include "Brain.h"

class Bot : public Player
{
	
private:

	Brain brain;

public:

	Brain& getBrain();

};

#endif