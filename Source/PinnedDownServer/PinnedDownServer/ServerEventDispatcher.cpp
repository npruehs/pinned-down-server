#include "ServerEventDispatcher.h"
#include "ServerGame.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;

ServerEventDispatcher::ServerEventDispatcher(ServerGame* serverGame, std::shared_ptr<Game> game)
{
	this->serverGame = serverGame;
	this->game = game;

	this->game->eventManager->AddListener(this);
}

void ServerEventDispatcher::OnEvent(Event & newEvent)
{
}

void ServerEventDispatcher::DispatchServerEvent(ServerEvent event)
{
	this->serverGame->OnServerEvent(event);
}
