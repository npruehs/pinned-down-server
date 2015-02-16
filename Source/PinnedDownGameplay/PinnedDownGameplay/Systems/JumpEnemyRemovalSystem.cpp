#include "Event.h"
#include "JumpEnemyRemovalSystem.h"

#include "Components\CardComponent.h"
#include "Components\OwnerComponent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownNet::Components;
using namespace PinnedDownGameplay::Systems;


JumpEnemyRemovalSystem::JumpEnemyRemovalSystem()
{
}

void JumpEnemyRemovalSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void JumpEnemyRemovalSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EntityInitializedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(JumpEnemyRemovalSystem, EntityInitializedEvent)
{
	// Check for enemy starship.
	auto entity = data.entity;

	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entity, CardComponent::CardComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(entity, OwnerComponent::OwnerComponentType);

	if (cardComponent->cardType != CardType::Starship)
	{
		return;
	}

	if (ownerComponent->owner == INVALID_ENTITY_ID)
	{
		this->enemyShips.push_back(entity);
	}
}

EVENT_HANDLER_DEFINITION(JumpEnemyRemovalSystem, TurnPhaseChangedEvent)
{
	if (data.newTurnPhase == TurnPhase::Jump)
	{
		// Remove all enemy starships.
		for (auto iterator = this->enemyShips.begin(); iterator != this->enemyShips.end(); ++iterator)
		{
			this->game->entityManager->RemoveEntity(*iterator);
		}

		this->enemyShips.clear();
	}
}