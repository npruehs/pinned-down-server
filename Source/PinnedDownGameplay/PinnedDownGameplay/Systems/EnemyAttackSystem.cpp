#include "EntityManager.h"
#include "Event.h"
#include "EnemyAttackSystem.h"

#include "Actions\EndTurnAction.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Data;
using namespace PinnedDownServer::Systems;


EnemyAttackSystem::EnemyAttackSystem()
{
}

void EnemyAttackSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Prepare attack deck.
	this->PrepareAttackDeck();

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
	CardData topCard = this->attackDeck->Draw();
	this->cardFactory->CreateCard(INVALID_ENTITY_ID, topCard.setIndex, topCard.cardIndex);

	// End turn phase.
	auto endTurnAction = std::make_shared<EndTurnAction>();
	this->game->eventManager->QueueEvent(endTurnAction);
}

void EnemyAttackSystem::PrepareAttackDeck()
{
	// Add cards.
	this->attackDeck = std::make_shared<Deck>();

	for (int i = 0; i < 100; i++)
	{
		this->attackDeck->Add(CardData(0, 84));
	}
}