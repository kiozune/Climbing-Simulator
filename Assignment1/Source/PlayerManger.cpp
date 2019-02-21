#include "PlayerManager.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	if (!instance) instance = new PlayerManager;
	return instance;
}

Player* PlayerManager::getMain() { return this->main; }
void PlayerManager::setMain(Player* p) { this->main = p; }

void PlayerManager::setQueue(std::string queue, unsigned clientId)
{ 
	for (char& c : queue)
	{
		unsigned id = (unsigned)c;
		if (id == clientId) continue;
		bool found = false;
		for (RemotePlayer* r : this->remotePlayers)
		{
			if (id == r->getId())
			{
				found = true;
				break;
			}
		}
		if (found) continue;
		this->pendingCreation.push_back(id); 
	}
}

int PlayerManager::popCreationQueue() 
{
	if (this->pendingCreation.size() == 0) return -1;

	int last = this->pendingCreation[this->pendingCreation.size() - 1];
	this->pendingCreation.pop_back();
	return last;
}

std::vector<Player*> PlayerManager::getPlayers() { return this->players; }
void PlayerManager::addPlayer(Player* p) { this->players.push_back(p); }

void PlayerManager::addRemotePlayer(RemotePlayer* p) { this->remotePlayers.push_back(p); }

void PlayerManager::updateRemote(PlayerData data)
{
	for (RemotePlayer* r : this->remotePlayers)
		if (r->getId() == data.id)
			r->update(data);
}