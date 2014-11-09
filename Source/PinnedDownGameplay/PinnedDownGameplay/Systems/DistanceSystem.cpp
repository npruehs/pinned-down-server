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

	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);

	// Setup victory condition.
	this->distanceMaximum = 5;

	this->SetDistanceCovered(0);
}

void DistanceSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void DistanceSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Jump)
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