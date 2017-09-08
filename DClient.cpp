#include "DClient.h"

using namespace DServerShell;

DClient::DClient()
{
	server = new DServer();
	server->Start();
}

DClient::~DClient()
{
	server->Stop();
	delete server;
}

bool DClient::ConnectToRoom(ipstr ip, portstr port)
{
	if (server->GetAddr(ip, port) == ADDR_ERROR)
		return false;

	if (server->SetupSocket(false) != SOCKET_OK)
		return false;


	std::cerr << "Connected to server\n";

	server->ShowAddr();
	server->ShowPeer(server->socketFD);

	std::string line;
	int bytes;

	while (true) {

		std::cin >> line;
		const char* buffer = line.c_str();

		if ((bytes = sendto(server->socketFD, buffer, strlen(buffer), 0, server->result->ai_addr, server->result->ai_addrlen)) == -1) {
			std::cerr << "Can't send any message to server\n";
		}
		else {
			std::cerr << "Message was sent to server!\n";
		}

	}

	return true;
}