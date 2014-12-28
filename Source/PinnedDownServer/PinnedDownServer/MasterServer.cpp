#include <stdio.h>

#include "MasterServer.h"

#include "Util\GUID\GUID.h"
using namespace PinnedDownServer;
using namespace PinnedDownServer::Util::GUID;

bool MasterServer::running = false;

MasterServer::MasterServer()
{
	this->socketManager = std::make_shared<SocketManager>(this);
	this->httpClient = std::make_shared<HTTPClient>();

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

	// Create new client data object.
	PinnedDownClientData client = PinnedDownClientData();
	client.clientId = clientId;
	client.clientGUID = NewGUID();

	// Start new game.
	std::shared_ptr<ServerGame> newGame = std::make_shared<ServerGame>(this, clientId);
	client.game = newGame;

	// Add to client list.
	this->connectedClients.insert(std::pair<int, PinnedDownClientData>(clientId, client));
}

void MasterServer::OnClientDisconnected(int clientId)
{
	printf("Client removed: %d\n", clientId);

	// Stop game.
	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		this->connectedClients.erase(iterator);
	}
}

void MasterServer::OnClientAction(int clientId, std::shared_ptr<Event> clientAction)
{
	printf("Client %d sent action %s.\n", clientId, clientAction->GetEventType().GetString());

	// Pass to game.
	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		auto client = iterator->second;
		client.game->OnClientAction(clientAction);
	}
}

void MasterServer::OnServerEvent(int clientId, Event& serverEvent)
{
	printf("Sending event %s to client %d.\n", serverEvent.GetEventType().GetString(), clientId);

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