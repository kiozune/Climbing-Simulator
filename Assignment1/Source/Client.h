#ifndef CLIENT_H
#define CLIENT_H

#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

class Client
{
	
private:
	
	unsigned id, knownSize;

	std::string serverIp;
	sockaddr_in server;
	SOCKET out;
	char buff[1024];
	int serverLength;

	int status;


public:
	
	unsigned getId();

	unsigned getKnownSize();
	void setKnownSize(unsigned);

	std::string getServerIp();

	bool start();
	void connectTo(u_short port, const char* ip);
	void sendData(std::string content);
	bool recieve(std::string& data);
	void exit();

	int getStatus();

};

#endif