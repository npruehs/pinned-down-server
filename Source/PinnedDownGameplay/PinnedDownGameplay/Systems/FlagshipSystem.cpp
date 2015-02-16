#include "Event.h"
#include "FlagshipSystem.h"

#include "..\Events\FlagshipPlayedEvent.h"

using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Events;


FlagshipSystem::FlagshipSystem()
{
}

void FlagshipSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Register listeners.
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
}

void FlagshipSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PlayerAddedEvent);
}

EVENT_HANDLER_DEFINITION(FlagshipSystem, PlayerAddedEvent)
{
	// Play flagship.
	auto owner = data.serverEntity;
	auto shipEntity = this->cardFactory->CreateCard(owner, 0, 56, CardState::InPlay);

	// Notify listeners.
	auto flagshipPlayedEvent = std::make_shared<FlagshipPlayedEvent>(shipEntity);
	this->game->eventManager->QueueEvent(flagshipPlayedEvent);
}
