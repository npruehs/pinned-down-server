#include <iostream>
#include <memory>

#include "Network\SocketManager.h"

using namespace PinnedDownServer::Network;

int main()
{
	std::shared_ptr<SocketManager> socketManager = std::make_shared<SocketManager>();
	socketManager->InitSocketManager();

	if (socketManager->IsInitialized())
	{
		while (true)
		{
			socketManager->Select(0);
		}
	}

	int i;
	std::cin >> i;
	return 0;
}
