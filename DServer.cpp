#include "DServer.h"

using namespace DServerShell;

DServer::DServer()
{
	result = NULL;
	memset(&hints, 0, sizeof(hints));
	socketFD = SOCKET_INIT;
}


DServer::~DServer()
{
	Stop();
}

void DServer::SetHintsConfig()
{
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
}

startval DServer::Start()
{
	SetHintsConfig();

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed" << "\n";
		return START_FAILED;
	}

	return START_SUCCESS;
}

void DServer::Stop() {
	if (socketFD != SOCKET_INIT)
		closesocket(socketFD);
	freeaddrinfo(result);
	WSACleanup();
}

addresponse DServer::GetAddr(ipstr ip, portstr port)
{
	INT addrResult;

	if ((addrResult = getaddrinfo(ip, port, &hints, &result)) != 0) {
		std::cerr << "getaddrinfo failed with error: " << addrResult << "\n";
		WSACleanup();
		return ADDR_ERROR;
	}

	std::cerr << "struct has been filled by getaddrinfo() and is ready to use\n";

	return ADDR_OK;
}

socketresponse DServer::SetupSocket(bool isBinded)
{
	if ((socketFD = socket(result->ai_family, result->ai_socktype, 0)) == -1) {
		std::cerr << "Socket initialization error\n";
		return SOCKET_ERROR;
	}

	if (isBinded) {
		if (bind(socketFD, result->ai_addr, result->ai_addrlen) != 0) {
			std::cerr << "Socket binding error\n";
			return SOCKET_BIND_ERROR;
		}
	}

	std::cerr << "Socket has been successfully initialized\n";
	std::cerr << "Socket descriptor: " << socketFD << "\n";

	return SOCKET_OK;
}

void DServerShell::DServer::ShowAddr()
{
	std::cerr << "\nAddress:\n\n";

	char ip[INET_ADDRSTRLEN];
	int port;

	sockaddr_in* addr = (sockaddr_in *)result->ai_addr;
	inet_ntop(AF_INET, (sockaddr *)&addr->sin_addr, ip, INET_ADDRSTRLEN);
	port = ntohs(addr->sin_port);
	std::cerr << "IP: " << ip;
	std::cerr << "\nPort: " <<  port << "\n\n";
}

void DServerShell::DServer::ShowPeer(socketfd fd)
{

	std::cerr << "\nPeer info:\n\n";

	sockaddr_storage addr;
	socklen_t addrlen = sizeof(addr);
	getpeername(fd, (sockaddr *)&addr, &addrlen);

	char ipstr[INET_ADDRSTRLEN];
	sockaddr_in *addr_in = (sockaddr_in *)&addr;
	int portstr = ntohs(addr_in->sin_port);
	inet_ntop(AF_INET, &addr_in->sin_addr, ipstr, INET_ADDRSTRLEN);
	std::cerr << "IP: " << ipstr << "\n";
	std::cerr << "Port: " << portstr << "\n\n";
}

bool DServer::CreateRoom(portstr port)
{
	if (GetAddr(NULL, port) == ADDR_ERROR)
		return false;
		
	if (SetupSocket(true) != SOCKET_OK)
		return false;
	
	ShowAddr();

	char buffer[BUFFER_LENGTH];
	int bytes;

	sockaddr_in remoteaddr;
	socklen_t remoteaddrlen = sizeof(remoteaddr);

	std::cerr << "Start listening...\n";

	while (true) {
		if ((bytes = recvfrom(socketFD, buffer, BUFFER_LENGTH, 0, (sockaddr *)&remoteaddr, &remoteaddrlen)) != -1) {
			buffer[bytes] = '\0';
			std::cerr << buffer << "\n";
		}
	}

	return true;
}
