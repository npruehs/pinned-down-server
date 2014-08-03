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
}

void MasterServer::OnClientDisconnected(int clientId)
{
	printf("Client removed: %d\n", clientId);
}

void MasterServer::OnClientAction(int clientId, ClientAction clientAction)
{
	printf("Client %d sent action %d.\n", clientId, clientAction.actionType);
}