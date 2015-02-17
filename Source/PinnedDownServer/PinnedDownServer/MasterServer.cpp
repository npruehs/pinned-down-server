#include <stdio.h>

#include "MasterServer.h"

#include "Actions\DisconnectClientAction.h"

#include "Logger.h"

#include "PinnedDownServerVersion.h"

#include "Util/GUID/GUID.h"


using namespace PinnedDownCore;
using namespace PinnedDownServer;
using namespace PinnedDownServer::Util::GUID;


bool MasterServer::running = false;

MasterServer::MasterServer()
	: logger(std::make_shared<ServerLogger>(LogLevel::Debug))
{
	this->socketManager = std::make_shared<SocketManager>(this, this->logger);
	this->httpClient = std::make_shared<HTTPClient>(this->logger);

	SetConsoleCtrlHandler(&PinnedDownServer::MasterServer::OnConsoleCtrlSignal, TRUE);
}

void MasterServer::Start()
{
	// Log server version.
	std::string versionString(PINNED_DOWN_SERVER_VERSION);
	this->logger->LogInfo("Pinned Down Server " + versionString);

	// Init socket manager.
	this->socketManager->InitSocketManager();
	
	if (this->socketManager->IsInitialized())
	{
		// Start receive loop.
		MasterServer::running = true;

		while (MasterServer::running)
		{
			socketManager->Select(100);
		}
	}
}

void MasterServer::OnClientConnected(int clientId)
{
	this->logger->LogInfo("Client added: " + std::to_string(clientId));

	// Create new client data object.
	PinnedDownClientData* pinnedDownClient = new PinnedDownClientData();
	pinnedDownClient->clientId = clientId;
	pinnedDownClient->clientGUID = NewGUID();

	// Start new game.
	std::shared_ptr<ServerGame> newGame = std::make_shared<ServerGame>(this, this->httpClient, this->logger, pinnedDownClient);
	pinnedDownClient->game = newGame;

	// Add to client list.
	this->connectedClients.insert(std::pair<int, PinnedDownClientData*>(clientId, pinnedDownClient));
}

void MasterServer::OnClientDisconnected(int clientId)
{
	this->logger->LogInfo("Client removed: " + std::to_string(clientId));

	// Stop game.
	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		delete iterator->second;
		this->connectedClients.erase(iterator);
	}
}

void MasterServer::OnClientAction(int clientId, std::shared_ptr<Event> clientAction)
{
	// Log action.
	this->logger->LogDebug("FROM client " + std::to_string(clientId) + ": " + clientAction->ToString());

	// Pass to game.
	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		// Check for willing to disconnect.
		if (clientAction->GetEventType() == DisconnectClientAction::DisconnectClientActionType)
		{
			// Remove client.
			this->socketManager->RemoveClient(clientId);
		}
		else
		{
			// Pass action to game.
			auto client = iterator->second;
			client->game->OnClientAction(clientAction);
		}
	}
}

void MasterServer::OnServerEvent(int clientId, Event& serverEvent)
{
	// Log event.
	this->logger->LogDebug("TO client " + std::to_string(clientId) + ": " + serverEvent.ToString());

	// Pass to socket manager.
	this->socketManager->SendServerEvent(clientId, serverEvent);
}

BOOL WINAPI MasterServer::OnConsoleCtrlSignal(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		MasterServer::running = false;
	}		

	return true;
}
