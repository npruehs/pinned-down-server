#include "Event.h"
#include "DistanceVictorySystem.h"

#include "Events\VictoryEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


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
	CALL_EVENT_HANDLER(CoveredDistanceChangedEvent);
}

EVENT_HANDLER_DEFINITION(DistanceVictorySystem, CoveredDistanceChangedEvent)
{
	if (data.distanceCovered >= data.distanceMaximum)
	{
		auto victoryEvent = std::make_shared<VictoryEvent>();
		this->game->eventManager->QueueEvent(victoryEvent);
	}
}
