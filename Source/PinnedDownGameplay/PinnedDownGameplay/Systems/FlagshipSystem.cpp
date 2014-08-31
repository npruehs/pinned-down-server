#include "Event.h"
#include "FlagshipSystem.h"

using namespace PinnedDownServer::Systems;
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
	if (newEvent.GetEventType() == PlayerAddedEvent::PlayerAddedEventType)
	{
		auto playerAddedEvent = static_cast<PlayerAddedEvent&>(newEvent);
		this->OnPlayerAdded(playerAddedEvent);
	}
}

void FlagshipSystem::OnPlayerAdded(PlayerAddedEvent& playerAddedEvent)
{
	// Play flagship.
	auto owner = playerAddedEvent.serverEntity;
	this->cardFactory->CreateCard(owner, 0, 56);
}
