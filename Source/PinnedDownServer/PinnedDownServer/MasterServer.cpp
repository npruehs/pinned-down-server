#include <stdio.h>
#include "MasterServer.h"

using namespace PinnedDownServer;


MasterServer::MasterServer()
{
	this->socketManager = std::make_shared<SocketManager>(this);
}

void MasterServer::Start()
{
	this->socketManager->InitSocketManager();

	if (this->socketManager->IsInitialized())
	{
		while (true)
		{
			socketManager->Select(100);
		}
	}
}

void MasterServer::OnClientConnected(int clientId)
{
	printf("Client added: %d\n", clientId);

	// Start new game.
	std::shared_ptr<ServerGame> newGame = std::make_shared<ServerGame>();
	this->runningGames.insert(std::pair<int, std::shared_ptr<ServerGame>>(clientId, newGame));
}

void MasterServer::OnClientDisconnected(int clientId)
{
	printf("Client removed: %d\n", clientId);

	// Stop game.
	auto iterator = this->runningGames.find(clientId);

	if (iterator != this->runningGames.end())
	{
		this->runningGames.erase(iterator);
	}
}

void MasterServer::OnClientAction(int clientId, ClientAction clientAction)
{
	printf("Client %d sent action %d.\n", clientId, clientAction.actionType);
}