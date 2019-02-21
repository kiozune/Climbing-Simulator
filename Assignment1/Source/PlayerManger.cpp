#include "PlayerManager.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	if (!instance) instance = new PlayerManager;
	return instance;
}

Player* PlayerManager::getMain() { return this->main; }
void PlayerManager::setMain(Player* p) { this->main = p; }

std::vector<Player*> PlayerManager::getPlayers() { return this->players; }
void PlayerManager::addPlayer(Player* p) { this->players.push_back(p); }

void PlayerManager::addRemotePlayer(RemotePlayer* p) { this->remotePlayers.push_back(p); }

void PlayerManager::updateRemote(PlayerData data)
{
	if (remotePlayers.size())
		remotePlayers[0]->update(data);
}