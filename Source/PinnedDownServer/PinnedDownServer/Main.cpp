#include <iostream>
#include <memory>

#include "MasterServer.h"

using namespace PinnedDownServer;

int main()
{
	// Start server.
	std::shared_ptr<MasterServer> masterServer = std::make_shared<MasterServer>();
	masterServer->Start();

	return 0;
}
