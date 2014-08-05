#include "Event.h"
#include "DistanceSystem.h"

#include "Events\CoveredDistanceChangedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


DistanceSystem::DistanceSystem()
{
}

void DistanceSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup victory condition.
	this->distanceCovered = 0;
	this->distanceMaximum = 5;

	// Notify listeners.
	auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>(this->distanceCovered, this->distanceMaximum);
	this->game->eventManager->QueueEvent(coveredDistanceChangedEvent);
}

void DistanceSystem::OnEvent(Event & newEvent)
{
}
