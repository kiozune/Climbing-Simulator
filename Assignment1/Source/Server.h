#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <fstream>

#pragma comment (lib, "ws2_32.lib")

class Server
{

private:

	std::string ip;
	std::vector<sockaddr_in> connections;
	std::string ids = "IDS:";

	char buff[1024];

	SOCKET in;
	sockaddr_in client;
	int clientLength;


	void fetchIp();

public:

	bool start();
	bool bindSocket();
	void run();
	void exit();

	std::string getIp();

};

#endif
