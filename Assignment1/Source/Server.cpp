#include "Server.h"

#include <thread>

void Server::fetchIp() 
{
	std::ifstream ifs;
	std::string search = "IPv4 Address. . . . . . . . . . . :";

	system("ipconfig > ip.txt");
	ifs.open("ip.txt");

	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			std::string line;
			std::getline(ifs, line);

			if ((line.find(search, 0)) != std::string::npos)
			{
				line.erase(0, 39);
				this->ip = line;
				ifs.close();
			}
		}
	}
}

bool Server::start()
{
	fetchIp();

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

bool Server::bindSocket()
{
	in = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = INADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(30000);

	int result = bind(in, (sockaddr*)&serverHint, sizeof(serverHint));
	if (result == SOCKET_ERROR)
	{
		std::cout << "Binding of socket failed" << std::endl;
		return false;
	}

	clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);

	return true;
}

void Server::run() 
{
	ZeroMemory(buff, 1024);


	int bytesIn = recvfrom(in, buff, 1024, 0, (sockaddr*)&client, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
		std::cout << "Error recieving data " << WSAGetLastError() << std::endl;
		return;
	}

	char clientIp[256];
	ZeroMemory(clientIp, 256);

	inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

	std::string content = buff;
	if (content.find("CONNECT") != std::string::npos)
	{
		connections.push_back(client);

		std::cout << "\nNew Connection" << std::endl;
		std::cout << "ID   : " << connections.size() << std::endl;
		std::cout << "SIZE : " << (unsigned)content[content.size() - 1] - 1 << "\n\n";

		ids += (char)connections.size();
		ids += (char)content[content.size() - 1];

		std::cout << ids << std::endl;

		for (sockaddr_in& addr : connections)
			sendto(in, ids.c_str(), ids.size(), 0, (sockaddr*)&addr, clientLength);
	}
	else if (content.find("NEW") != std::string::npos)
	{
		int id = (unsigned)content[content.size() - 1];
		int count = 0;

		std::cout << "\nNew player from : " << id << std::endl;

		for (int i = 0; i < (ids.size() - 4) / 2; ++i)
		{
			if (ids[i * 2 + 4] == id)
			{
				count = ++ids[i * 2 + 5];
				break;
			}
		}
		count -= 1;

		std::cout << id << " has " << count << " players now.\n\n";

		content = "NEW";
		content += (char)(id * 10 + count);
		for (sockaddr_in& addr : connections)
			sendto(in, content.c_str(), content.size(), 0, (sockaddr*)&addr, clientLength);
	}

	for (sockaddr_in& addr : connections)
		sendto(in, content.c_str(), content.size(), 0, (sockaddr*)&addr, clientLength);
}

void Server::exit()
{
	closesocket(in);
	WSACleanup();
}

std::string Server::getIp() { return this->ip; }
