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
}

void TurnPhaseSystem::OnEndTurn(EndTurnAction& endTurnAction)
{
	// Set next turn phase.
	switch (this->currentPhase)
	{
	case TurnPhase::Main:
		this->SetTurnPhase(TurnPhase::Jump);
		break;
	case TurnPhase::Jump:
		this->SetTurnPhase(TurnPhase::Attack);
		break;
	case TurnPhase::Attack:
		this->SetTurnPhase(TurnPhase::Assignment);
		break;
	case TurnPhase::Assignment:
		this->SetTurnPhase(TurnPhase::Fight);
		break;
	case TurnPhase::Fight:
		this->SetTurnPhase(TurnPhase::WrapUp);
		break;
	case TurnPhase::WrapUp:
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