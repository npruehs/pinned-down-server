#include "Event.h"
#include "FlagshipSystem.h"

#include "Events\CardCreatedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


FlagshipSystem::FlagshipSystem()
{
}

void FlagshipSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Play flagships.
	this->PlayFlagships();
}

void FlagshipSystem::OnEvent(Event & newEvent)
{
}

void FlagshipSystem::PlayFlagships()
{
	Entity flagship = this->cardFactory->CreateCard(0, 56);

	auto cardCreatedEvent = std::make_shared<CardCreatedEvent>(flagship, 0, 56);
	this->game->eventManager->QueueEvent(cardCreatedEvent);
}