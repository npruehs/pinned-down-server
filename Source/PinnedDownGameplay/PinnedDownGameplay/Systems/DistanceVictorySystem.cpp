#include "Event.h"
#include "DistanceVictorySystem.h"

#include "Events\VictoryEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


DistanceVictorySystem::DistanceVictorySystem()
{
}

void DistanceVictorySystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
}

void DistanceVictorySystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CoveredDistanceChangedEvent::CoveredDistanceChangedEventType)
	{
		auto coveredDistanceChangedEvent = static_cast<CoveredDistanceChangedEvent&>(newEvent);
		this->OnCoveredDistanceChanged(coveredDistanceChangedEvent);
	}
}

void DistanceVictorySystem::OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent)
{
	if (coveredDistanceChangedEvent.distanceCovered >= coveredDistanceChangedEvent.distanceMaximum)
	{
		auto victoryEvent = std::make_shared<VictoryEvent>();
		this->game->eventManager->QueueEvent(victoryEvent);
	}
}
