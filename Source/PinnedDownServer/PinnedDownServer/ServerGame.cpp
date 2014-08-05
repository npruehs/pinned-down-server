

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

	this->serverEventDispatcher = std::make_shared<ServerEventDispatcher>(this, this->game);
}

void ServerGame::OnClientAction(std::shared_ptr<Event> clientAction)
{
	this->game->eventManager->QueueEvent(clientAction);
	this->Update();
}

void ServerGame::OnServerEvent(Event& serverEvent)
{
	this->masterServer->OnServerEvent(this->clientId, serverEvent);
}

void ServerGame::Update()
{
	this->game->Update(1.0f);
}
