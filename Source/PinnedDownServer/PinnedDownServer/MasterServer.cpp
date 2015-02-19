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
	: logger(std::make_shared<ServerLogger>(LogLevel::Debug)),
	nextGameId(0)
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

	// Check for running game.
	std::shared_ptr<ServerGame> runningGame;

	for (auto it = this->runningGames.begin(); it != this->runningGames.end(); ++it)
	{
		auto game = *it;

		if (game->GetClientCount() < CLIENTS_PER_GAME)
		{
			runningGame = game;
		}
	}

	if (runningGame == nullptr)
	{
		// Start new game.
		runningGame = std::make_shared<ServerGame>(this, ++this->nextGameId, this->httpClient, this->logger);
		this->runningGames.push_back(runningGame);
	}

	// Assign client.
	runningGame->AddClient(pinnedDownClient);
	pinnedDownClient->game = runningGame;

	this->logger->LogInfo("Client added to game " + std::to_string(runningGame->GetGameId()));

	// Add to client list.
	this->connectedClients.insert(std::pair<int, PinnedDownClientData*>(clientId, pinnedDownClient));

	// Check if game full.
	if (runningGame->GetClientCount() == CLIENTS_PER_GAME)
	{
		runningGame->StartGame();
	}
}

void MasterServer::OnClientDisconnected(int clientId)
{
	this->logger->LogInfo("Client disconnected: " + std::to_string(clientId));

	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		// Check if last player.
		auto client = iterator->second;

		if (client->game->GetClientCount() == 1)
		{
			this->logger->LogInfo("Shutting down game " + std::to_string(client->game->GetGameId()));

			// Stop game.
			this->runningGames.remove(client->game);
		}

		// Remove client.
		delete client;
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
		auto client = iterator->second;

		// Check for willing to disconnect.
		if (clientAction->GetEventType() == DisconnectClientAction::DisconnectClientActionType)
		{
			// Remove client, everything else will be handled by OnClientDisconnected.
			this->socketManager->RemoveClient(clientId);
		}
		else
		{
			// Pass action to game.
			client->game->OnClientAction(clientAction);
		}

		// Check for game over.
		if (client->game->IsGameOver())
		{
			auto gameClients = client->game->GetClients();

			for (auto it = gameClients.begin(); it != gameClients.end(); ++it)
			{
				auto gameClient = *it;

				// Remove clients, everything else will be handled by OnClientDisconnected.
				this->logger->LogInfo("Game over, disconnecting client " + std::to_string(gameClient->clientId));
				this->socketManager->RemoveClient(gameClient->clientId);
			}
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
