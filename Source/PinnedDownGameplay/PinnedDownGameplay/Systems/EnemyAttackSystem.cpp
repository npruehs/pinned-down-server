#include "Event.h"
#include "EnemyAttackSystem.h"

#include "Actions\EndTurnAction.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


EnemyAttackSystem::EnemyAttackSystem()
{
}

void EnemyAttackSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Register for events.
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void EnemyAttackSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void EnemyAttackSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Attack)
	{
		this->PlayEnemyCards();
	}
}

void EnemyAttackSystem::PlayEnemyCards()
{
	// Play enemy card.
	this->cardFactory->CreateCard(0, 73);

	// End turn phase.
	auto endTurnAction = std::make_shared<EndTurnAction>();
	this->game->eventManager->QueueEvent(endTurnAction);
}