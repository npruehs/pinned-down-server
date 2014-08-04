#include <stdio.h>

#include "MasterServer.h"

using namespace PinnedDownServer;

bool MasterServer::running = false;

MasterServer::MasterServer()
{
	this->socketManager = std::make_shared<SocketManager>(this);

	SetConsoleCtrlHandler(&PinnedDownServer::MasterServer::OnConsoleCtrlSignal, TRUE);
}

void MasterServer::Start()
{
	this->socketManager->InitSocketManager();
	
	if (this->socketManager->IsInitialized())
	{
		MasterServer::running = true;

		while (MasterServer::running)
		{
			socketManager->Select(100);
		}
	}
}

void MasterServer::OnClientConnected(int clientId)
{
	printf("Client added: %d\n", clientId);

	// Start new game.
	std::shared_ptr<ServerGame> newGame = std::make_shared<ServerGame>(this, clientId);
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

	// Pass to game.
	auto iterator = this->runningGames.find(clientId);

	if (iterator != this->runningGames.end())
	{
		auto game = iterator->second;
		game->OnClientAction(clientAction);
	}
}

void MasterServer::OnServerEvent(int clientId, ServerEvent serverEvent)
{
	printf("Sending event %d to client %d.\n", serverEvent.eventType, clientId);

	// Pass to socket manager.
	this->socketManager->SendServerEvent(clientId, serverEvent);
}

BOOL WINAPI MasterServer::OnConsoleCtrlSignal(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		MasterServer::running = false;
		printf("Shutting down server...");
	}		

	return true;
}