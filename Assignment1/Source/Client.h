#ifndef CLIENT_H
#define CLIENT_H

#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

class Client
{
	
private:

	unsigned id;

	sockaddr_in server;
	SOCKET out;
	char buff[1024];
	int serverLength;

public:

	unsigned getId();

	bool start();
	void connectTo(u_short port, const char* ip);
	void sendData(std::string content);
	bool recieve(std::string& data);
	void exit();

};

#endif