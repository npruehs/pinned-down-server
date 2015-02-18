#include "Event.h"
#include "TurnPhaseSystem.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


TurnPhaseSystem::TurnPhaseSystem()
{
}

void TurnPhaseSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EndTurnAction::EndTurnActionType);
	this->game->eventManager->AddListener(this, GameStartedEvent::GameStartedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void TurnPhaseSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EndTurnAction);
	CALL_EVENT_HANDLER(GameStartedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(TurnPhaseSystem, EndTurnAction)
{
	// Set next turn phase.
	switch (this->currentPhase)
	{
	case TurnPhase::Main:
		this->SetTurnPhase(TurnPhase::Attack);
		break;
	}
}

EVENT_HANDLER_DEFINITION(TurnPhaseSystem, GameStartedEvent)
{
	// Setup turn sequence.
	this->SetTurnPhase(TurnPhase::Main);
}

EVENT_HANDLER_DEFINITION(TurnPhaseSystem, TurnPhaseChangedEvent)
{
	this->currentPhase = data.newTurnPhase;
}

void TurnPhaseSystem::SetTurnPhase(TurnPhase turnPhase)
{
	this->currentPhase = turnPhase;

	// Notify listeners.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(this->currentPhase);
	this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
}