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
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void DistanceVictorySystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == CoveredDistanceChangedEvent::CoveredDistanceChangedEventType)
	{
		auto coveredDistanceChangedEvent = static_cast<CoveredDistanceChangedEvent&>(newEvent);
		this->OnCoveredDistanceChanged(coveredDistanceChangedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void DistanceVictorySystem::OnCoveredDistanceChanged(CoveredDistanceChangedEvent& coveredDistanceChangedEvent)
{
	if (coveredDistanceChangedEvent.distanceCovered >= coveredDistanceChangedEvent.distanceMaximum)
	{
		this->lastTurn = true;
	}
}

void DistanceVictorySystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::WrapUp && this->lastTurn)
	{
		auto victoryEvent = std::make_shared<VictoryEvent>();
		this->game->eventManager->QueueEvent(victoryEvent);
	}
}
