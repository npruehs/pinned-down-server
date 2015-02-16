#include "Event.h"
#include "DistanceVictorySystem.h"

#include "Events\VictoryEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


DistanceVictorySystem::DistanceVictorySystem()
	: defeat(false)
{
}

void DistanceVictorySystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	this->game->eventManager->AddListener(this, DefeatEvent::DefeatEventType);
}

void DistanceVictorySystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(CoveredDistanceChangedEvent);
	CALL_EVENT_HANDLER(DefeatEvent);
}

EVENT_HANDLER_DEFINITION(DistanceVictorySystem, CoveredDistanceChangedEvent)
{
	// Can't win anymore if defeated, e.g. by losing flagship in very last fight during last turn.
	if (this->defeat)
	{
		return;
	}

	if (data.distanceCovered >= data.distanceMaximum)
	{
		auto victoryEvent = std::make_shared<VictoryEvent>();
		this->game->eventManager->QueueEvent(victoryEvent);
	}
}

EVENT_HANDLER_DEFINITION(DistanceVictorySystem, DefeatEvent)
{
	this->defeat = true;
}