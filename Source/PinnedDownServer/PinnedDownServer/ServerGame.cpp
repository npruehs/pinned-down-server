

#include "MasterServer.h"
#include "ServerGame.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;

ServerGame::ServerGame(MasterServer* masterServer, int clientId)
{
	this->masterServer = masterServer;
	this->clientId = clientId;

	// Setup game infrastructure.
	this->game = std::make_shared<Game>();

	this->clientActionDispatcher = std::make_shared<ClientActionDispatcher>(this->game);
	this->serverEventDispatcher = std::make_shared<ServerEventDispatcher>(this, this->game);
}

void ServerGame::OnClientAction(ClientAction clientAction)
{
	this->clientActionDispatcher->DispatchClientAction(clientAction);
	this->Update();
}

void ServerGame::OnServerEvent(ServerEvent serverEvent)
{
	this->masterServer->OnServerEvent(this->clientId, serverEvent);
}

void ServerGame::Update()
{
	this->game->Update(1.0f);
}
