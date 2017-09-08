#pragma once
#include "DServer.h"

namespace DServerShell {

	class DClient
	{
	public:
		DClient();
		~DClient();
		bool ConnectToRoom(ipstr ip, portstr port);
	private:
		DServer* server;
	};

}
