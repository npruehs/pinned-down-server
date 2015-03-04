#include "Event.h"
#include "TurnPhaseSystem.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;


TurnPhaseSystem::TurnPhaseSystem()
{
}

void TurnPhaseSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, AllPlayersReadyEvent::AllPlayersReadyEventType);
	this->game->eventManager->AddListener(this, GameStartedEvent::GameStartedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseEndedEvent::TurnPhaseEndedEventType);
}

void TurnPhaseSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(AllPlayersReadyEvent);
	CALL_EVENT_HANDLER(GameStartedEvent);
	CALL_EVENT_HANDLER(TurnPhaseEndedEvent);
}

EVENT_HANDLER_DEFINITION(TurnPhaseSystem, AllPlayersReadyEvent)
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

EVENT_HANDLER_DEFINITION(TurnPhaseSystem, TurnPhaseEndedEvent)
{
	// Set next turn phase.
	switch (this->currentPhase)
	{
	// Attack phase is automatically ended when all enemy ships have spawned.
	case TurnPhase::Attack:
		this->SetTurnPhase(TurnPhase::Assignment);
		break;

	// Assignment phase is automatically ended when all players are ready AND all ships are assigned.
	case TurnPhase::Assignment:
		this->SetTurnPhase(TurnPhase::Fight);
		break;

	// Fight phase is automatically ended when all fights have been resolved.
	case TurnPhase::Fight:
		this->SetTurnPhase(TurnPhase::Jump);
		break;

	// Jump phase is automatically ended when all end-of-turn actions have bene resolved.
	case TurnPhase::Jump:
		this->SetTurnPhase(TurnPhase::Main);
		break;
	}
}

void TurnPhaseSystem::SetTurnPhase(TurnPhase turnPhase)
{
	this->currentPhase = turnPhase;

	// Notify listeners.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(this->currentPhase);
	this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
}