#include "Event.h"
#include "JumpEnemyRemovalSystem.h"

#include "Components\CardComponent.h"
#include "Components\OwnerComponent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownNet::Components;
using namespace PinnedDownServer::Systems;


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
	if (newEvent.GetEventType() == EntityInitializedEvent::EntityInitializedEventType)
	{
		auto entityInitializedEvent = static_cast<EntityInitializedEvent&>(newEvent);
		this->OnEntityInitialized(entityInitializedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void JumpEnemyRemovalSystem::OnEntityInitialized(EntityInitializedEvent& entityInitializedEvent)
{
	// Check for enemy starship.
	auto entity = entityInitializedEvent.entity;

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

void JumpEnemyRemovalSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Jump)
	{
		// Remove all enemy starships.
		for (auto iterator = this->enemyShips.begin(); iterator != this->enemyShips.end(); ++iterator)
		{
			this->game->entityManager->RemoveEntity(*iterator);
		}

		this->enemyShips.clear();
	}
}