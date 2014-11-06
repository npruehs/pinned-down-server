#include "Event.h"
#include "FlagshipSystem.h"

#include "..\Events\FlagshipPlayedEvent.h"

using namespace PinnedDownServer::Systems;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Events;

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
	auto shipEntity = this->cardFactory->CreateCard(owner, 0, 56, CardState::InPlay);

	// Notify listeners.
	auto flagshipPlayedEvent = std::make_shared<FlagshipPlayedEvent>(shipEntity);
	this->game->eventManager->QueueEvent(flagshipPlayedEvent);
}
