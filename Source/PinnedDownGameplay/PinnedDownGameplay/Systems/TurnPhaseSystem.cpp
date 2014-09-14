#include "Event.h"
#include "TurnPhaseSystem.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


TurnPhaseSystem::TurnPhaseSystem()
{
}

void TurnPhaseSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EndTurnAction::EndTurnActionType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);

	// Setup turn sequence.
	this->SetTurnPhase(TurnPhase::Main);
}

void TurnPhaseSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EndTurnAction::EndTurnActionType)
	{
		auto endTurnAction = static_cast<EndTurnAction&>(newEvent);
		this->OnEndTurn(endTurnAction);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void TurnPhaseSystem::OnEndTurn(EndTurnAction& endTurnAction)
{
	// Set next turn phase.
	switch (this->currentPhase)
	{
	case TurnPhase::Main:
		this->SetTurnPhase(TurnPhase::Attack);
		break;
	case TurnPhase::Attack:
		this->SetTurnPhase(TurnPhase::Assignment);
		break;
	case TurnPhase::Fight:
		this->SetTurnPhase(TurnPhase::Jump);
		break;
	case TurnPhase::Jump:
		this->SetTurnPhase(TurnPhase::Main);
		break;
	}
}

void TurnPhaseSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentPhase = turnPhaseChangedEvent.newTurnPhase;
}

void TurnPhaseSystem::SetTurnPhase(TurnPhase turnPhase)
{
	this->currentPhase = turnPhase;

	// Notify listeners.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(this->currentPhase);
	this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
}