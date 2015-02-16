#include "ServerEventDispatcher.h"
#include "ServerGame.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;

ServerEventDispatcher::ServerEventDispatcher(ServerGame* serverGame, std::shared_ptr<Game> game)
	: game(game)
{
	this->serverGame = serverGame;

	this->game->eventManager->AddListener(this);
}

void ServerEventDispatcher::OnEvent(Event& newEvent)
{
	if (newEvent.GetNetRole() == NetRole::Client)
	{
		this->serverGame->OnServerEvent(newEvent);
	}
}
