#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")
#endif

#include <iostream>
#include <string>

//server startup defines
#define START_FAILED 0
#define START_SUCCESS 1

//getaddrinfo() response
#define ADDR_ERROR 0
#define ADDR_OK 1

//socket handling defines
#define SOCKET_INIT -1
#define SOCKET_ERROR 0
#define SOCKET_BIND_ERROR 1
#define SOCKET_OK 2

namespace DServerShell {

	typedef int startval;
	typedef int addresponse;
	typedef int socketresponse;

	typedef PCSTR portstr;
	typedef PCSTR ipstr;

	typedef int socketfd;

	class DClient;

	class DServer
	{
	public:
		DServer();
		~DServer();
		startval Start();
		void Stop();
		bool CreateRoom(portstr port);
		friend class DClient;
	private:
		static const int BUFFER_LENGTH = 2048;

		addrinfo* result;
		WSADATA wsaData;
		addrinfo hints;
		socketfd socketFD;

		void SetHintsConfig();
		addresponse GetAddr(ipstr ip, portstr port);
		socketresponse SetupSocket(bool isBinded);
		void ShowAddr();
		void ShowPeer(socketfd fd);
	};

}