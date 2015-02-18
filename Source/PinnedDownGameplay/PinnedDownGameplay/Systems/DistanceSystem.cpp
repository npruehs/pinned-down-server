#include "Event.h"
#include "DistanceSystem.h"

#include "Events\CoveredDistanceChangedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


DistanceSystem::DistanceSystem()
	: distanceCovered(0),
	distanceMaximum(0)
{
}

void DistanceSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, GameStartedEvent::GameStartedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
	
	// Setup victory condition.
	this->distanceMaximum = 5;
}

void DistanceSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(GameStartedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(DistanceSystem, GameStartedEvent)
{
	// Set initial distance.
	this->SetDistanceCovered(0);
}

EVENT_HANDLER_DEFINITION(DistanceSystem, TurnPhaseChangedEvent)
{
	if (data.newTurnPhase == TurnPhase::Jump)
	{
		this->SetDistanceCovered(this->distanceCovered + 1);
	}
}

void DistanceSystem::SetDistanceCovered(int distanceCovered)
{
	this->distanceCovered = distanceCovered;

	// Notify listeners.
	auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>(this->distanceCovered, this->distanceMaximum);
	this->game->eventManager->QueueEvent(coveredDistanceChangedEvent);
}