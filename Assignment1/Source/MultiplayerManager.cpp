#include "MultiplayerManager.h"

#include <thread>

#include "Constants.h"
#include "PlayerManager.h"
#include "DataTransferManager.h"

MultiplayerManager* MultiplayerManager::instance = nullptr;

MultiplayerManager * MultiplayerManager::getInstance()
{
	if (instance == nullptr)
		instance = new MultiplayerManager;

	return instance;
}

Server MultiplayerManager::getSever() { return this->server; }

void MultiplayerManager::startSever()
{
	server.start();
	server.bindSocket();
	std::thread main([]() {
		while (true)
			MultiplayerManager::getInstance()->getSever().run();
	});
	main.detach();
}

void MultiplayerManager::connectTo(std::string IP)
{
	DataTransferManager* transferManager = DataTransferManager::getInstance();
	transferManager->getClient().connectTo(SERVER_PORT, IP.c_str());
}

void MultiplayerManager::end()
{
	DataTransferManager* transferManager = DataTransferManager::getInstance();
	transferManager->getClient().exit();
	server.exit();
}

void MultiplayerManager::receive()
{
	std::thread sendThread([]() {
		DataTransferManager* transferManager = DataTransferManager::getInstance();
		Client& client = transferManager->getClient();
		unsigned clientId = client.getId();
		PlayerManager* playerManager = PlayerManager::getInstance();

		while (true)
		{
			std::vector<Player*> local = playerManager->getLocalPlayers();
			if (local.size() > client.getKnownSize())
			{
				client.setKnownSize(local.size());
				std::string data = "NEW";
				data += (char)clientId;
				client.sendData(data);
			}
			else
			{
				for (Player* p : local)
				{
					PlayerData pData = transferManager->getPlayerData(*p, clientId);
					std::string data = transferManager->stringifyData(pData);
					if (data.size() > MIN_SIZE)
					{
						transferManager->getClient().sendData(data);
					}
				}
			}
			Sleep(1);
		}
	});
	sendThread.detach();
}

void MultiplayerManager::send()
{
	std::thread receiveThread([]() {
		DataTransferManager* transferManager = DataTransferManager::getInstance();
		Client& client = transferManager->getClient();
		PlayerManager* playerManager = PlayerManager::getInstance();
		while (true)
		{
			std::string data;
			bool didRecieve = client.recieve(data);
			if (didRecieve)
			{
				size_t pos = data.find("IDS:");

				if (pos != std::string::npos)
				{
					data.erase(pos, 4);
					playerManager->createRemotePlayers(data, client.getId());
				}
				else
				{
					size_t newPlayer = data.find("NEW");
					if (newPlayer != std::string::npos)
					{
						unsigned id = (unsigned)data[data.size() - 1];
						unsigned clientId = id / 10;
						unsigned playerId = id % 10 - 1;
						if (client.getId() != clientId)
						{
							RemotePlayer* r = playerManager->createRemotePlayer(clientId, playerId);
							if (r != nullptr)	playerManager->addRemotePlayer(r);
						}
					}
					else
					{
						size_t connect = data.find("CONNECT");
						if (connect == std::string::npos)
						{
							playerManager->updateRemote(transferManager->parseData(data));
						}
					}
				}
			}
		}
	});
	receiveThread.detach();
}