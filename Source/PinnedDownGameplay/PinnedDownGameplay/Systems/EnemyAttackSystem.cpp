#include "EntityManager.h"
#include "Event.h"
#include "EnemyAttackSystem.h"

#include "Actions\EndTurnAction.h"

#include "Components\ThreatComponent.h"

#include "..\Events\EnemyCardPlayedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Data;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;


EnemyAttackSystem::EnemyAttackSystem()
	: currentThreat(0)
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
	CALL_EVENT_HANDLER(ThreatChangedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(EnemyAttackSystem, ThreatChangedEvent)
{
	this->currentThreat = data.newThreat;
}

EVENT_HANDLER_DEFINITION(EnemyAttackSystem, TurnPhaseChangedEvent)
{
	if (data.newTurnPhase == TurnPhase::Attack)
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
		auto cardEntity = this->cardFactory->PrepareCard(INVALID_ENTITY_ID, topCard.setIndex, topCard.cardIndex, CardState::InPlay);

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

			if (remainingThreat == 0)
			{
				// Stop playing.
				break;
			}
		}
		else
		{
			// Discard card.
			this->game->entityManager->RemoveEntity(cardEntity);

			// Stop playing.
			break;
		}
	}
	
	// End attack phase.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Assignment);
	this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
}

void EnemyAttackSystem::PrepareAttackDeck()
{
	// Add cards.
	this->attackDeck = std::make_shared<Deck>();

	this->attackDeck->Add(CardData(0, 72));
	this->attackDeck->Add(CardData(0, 72));
	this->attackDeck->Add(CardData(0, 73));
	this->attackDeck->Add(CardData(0, 73));
	this->attackDeck->Add(CardData(0, 75));
	this->attackDeck->Add(CardData(0, 75));
	this->attackDeck->Add(CardData(0, 84));
	this->attackDeck->Add(CardData(0, 84));

	// Shuffle deck.
	auto random = std::make_shared<Random>();
	this->attackDeck->Shuffle(random);
}