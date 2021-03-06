#include "EntityManager.h"
#include "Event.h"
#include "PlayerSystem.h"

#include "Events\PlayerAddedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


PlayerSystem::PlayerSystem()
	: players(0)
{
}

void PlayerSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Create player factory.
	this->playerFactory = std::make_shared<PlayerFactory>(game);

	// Register listeners.
	this->game->eventManager->AddListener(this, ClientConnectedEvent::ClientConnectedEventType);
}

void PlayerSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(ClientConnectedEvent);
}

EVENT_HANDLER_DEFINITION(PlayerSystem, ClientConnectedEvent)
{
	auto clientId = data.clientId;
	auto playerName = "Player" + std::to_string(++this->players);

	// Create player entity.
	Entity playerEntity = this->playerFactory->CreatePlayer(clientId, playerName);

	// Notify listeners.
	auto playerAddedEvent = std::make_shared<PlayerAddedEvent>(playerEntity, clientId, playerName);
	this->game->eventManager->QueueEvent(playerAddedEvent);
}
