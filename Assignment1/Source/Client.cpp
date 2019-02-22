#include "Client.h"

#include <iostream>

#include "PlayerManager.h"

unsigned Client::getId() { return this->id; }

unsigned Client::getKnownSize() { return this->knownSize; }
void Client::setKnownSize(unsigned s) { this->knownSize = s; }

bool Client::start()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		std::cout << "Failed to start" << std::endl;
		return false;
	}
	return true;
}

void Client::connectTo(u_short port, const char* ip)
{
	serverLength = sizeof(server);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	InetPton(AF_INET, ip, &server.sin_addr);

	out = socket(AF_INET, SOCK_DGRAM, 0);

	PlayerManager* manager = PlayerManager::getInstance();
	int count = manager->getLocalPlayers().size();
	this->knownSize = count;
	std::string data = "CONNECT";
	data += (char)(count + 1);
	this->sendData(data);

	if (recieve(data))
	{
		this->id = (unsigned)data[data.size() - 2];
		data.pop_back();

		size_t pos = data.find("IDS:");
		if (pos != std::string::npos)
		{
			data.erase(pos, 4);

			while (data.size())
			{
				count = (unsigned)data[data.size() - 1] - 1;
				data.pop_back();
				int id = (unsigned)data[data.size() - 1];
				data.pop_back();
				for (int i = 0; i < count; ++i)
				{
					RemotePlayer* r = (RemotePlayer*)manager->createPlayer(id * 10 + i);
					manager->addRemotePlayer(r);
				}
			}
		}
	}
}

void Client::sendData(std::string content)
{
	int sendResult = sendto(out, content.c_str(), content.size(), 0, (sockaddr*)&server, serverLength);

	if (sendResult == SOCKET_ERROR)
		std::cout << "Failed to send : " << WSAGetLastError() << std::endl;
}

bool Client::recieve(std::string& data)
{
	ZeroMemory(buff, 1024);

	int bytesIn = recvfrom(out, buff, 1024, 0, (sockaddr*)&server, &serverLength);
	if (bytesIn == SOCKET_ERROR)
	{
		std::cout << "Error recieving data " << WSAGetLastError() << std::endl;
		return false;
	}
	//std::cout << "Date recievied : " << buff << std::endl;
	data = buff;
	return true;

}

void Client::exit()
{
	closesocket(out);
	WSACleanup();
}