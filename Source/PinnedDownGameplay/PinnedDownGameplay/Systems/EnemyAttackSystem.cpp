#include "EntityManager.h"
#include "Event.h"
#include "EnemyAttackSystem.h"

#include "Actions\EndTurnAction.h"

#include "Components\ThreatComponent.h"

#include "..\Events\EnemyCardPlayedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Data;
using namespace PinnedDownServer::Events;
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
	this->game->eventManager->AddListener(this, ThreatChangedEvent::ThreatChangedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void EnemyAttackSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == ThreatChangedEvent::ThreatChangedEventType)
	{
		auto threatChangedEvent = static_cast<ThreatChangedEvent&>(newEvent);
		this->OnThreatChanged(threatChangedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void EnemyAttackSystem::OnThreatChanged(ThreatChangedEvent& threatChangedEvent)
{
	this->currentThreat = threatChangedEvent.newThreat;
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
	auto remainingThreat = this->currentThreat;

	while (true)
	{
		// Get top enemy card.
		if (this->attackDeck->IsEmpty())
		{
			this->PrepareAttackDeck();
		}

		CardData topCard = this->attackDeck->Draw();
		auto cardEntity = this->cardFactory->PrepareCard(INVALID_ENTITY_ID, topCard.setIndex, topCard.cardIndex);

		// Check card cost.
		auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(cardEntity, ThreatComponent::ThreatComponentType);

		if (remainingThreat >= threatComponent->threat)
		{
			// Play card.
			this->cardFactory->FinishCard(cardEntity);
			remainingThreat -= threatComponent->threat;

			// Notify listeners.
			auto enemyCardPlayedEvent = std::make_shared<EnemyCardPlayedEvent>(cardEntity);
			this->game->eventManager->QueueEvent(enemyCardPlayedEvent);
		}
		else
		{
			// Discard card.
			this->game->entityManager->RemoveEntity(cardEntity);

			// Stop playing.
			break;
		}
	}
	
	// End turn phase.
	auto endTurnAction = std::make_shared<EndTurnAction>();
	this->game->eventManager->QueueEvent(endTurnAction);
}

void EnemyAttackSystem::PrepareAttackDeck()
{
	// Add cards.
	this->attackDeck = std::make_shared<Deck>();

	this->attackDeck->Add(CardData(0, 84));
}