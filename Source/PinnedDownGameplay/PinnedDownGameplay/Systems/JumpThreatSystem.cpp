#include "Event.h"
#include "JumpThreatSystem.h"

#include "..\Actions\AddThreatAction.h"
#include "Actions\EndTurnAction.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Events;
using namespace PinnedDownServer::Systems;


JumpThreatSystem::JumpThreatSystem()
{
}

void JumpThreatSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void JumpThreatSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void JumpThreatSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Jump)
	{
		// Increase threat.
		auto addThreatAction = std::make_shared<AddThreatAction>(3);
		this->game->eventManager->QueueEvent(addThreatAction);

		// End jump phase.
		auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Main);
		this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
	}
}