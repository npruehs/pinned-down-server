#include "Event.h"
#include "EffectPlayingSystem.h"

#include "Components\CardStateComponent.h"
#include "Components\ConditionalPowerComponent.h"
#include "Components\ConditionNotDamagedComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\PowerComponent.h"
#include "Components\ThreatComponent.h"

#include "..\Events\BonusPowerChangedEvent.h"
#include "Events\PowerChangedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;


EffectPlayingSystem::EffectPlayingSystem()
{
}

void EffectPlayingSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EffectPlayedEvent::EffectPlayedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, ShipDamagedEvent::ShipDamagedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void EffectPlayingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EffectPlayedEvent::EffectPlayedEventType)
	{
		auto effectPlayedEvent = static_cast<EffectPlayedEvent&>(newEvent);
		this->OnEffectPlayed(effectPlayedEvent);
	}
	else if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		auto entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->OnEntityRemoved(entityRemovedEvent);
	}
	else if (newEvent.GetEventType() == ShipDamagedEvent::ShipDamagedEventType)
	{
		auto shipDamagedEvent = static_cast<ShipDamagedEvent&>(newEvent);
		this->OnShipDamaged(shipDamagedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void EffectPlayingSystem::OnEffectPlayed(EffectPlayedEvent& effectPlayedEvent)
{
	if (this->currentTurnPhase != TurnPhase::Fight)
	{
		return;
	}

	// Check for power bonus.
	auto effectPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(effectPlayedEvent.effectEntity, PowerComponent::PowerComponentType);
	
	if (effectPowerComponent != nullptr)
	{
		// Get target.
		auto targetPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(effectPlayedEvent.targetEntity, PowerComponent::PowerComponentType);

		if (targetPowerComponent != nullptr)
		{
			int bonusPower = effectPowerComponent->bonusPowerUntilEndOfTurn;

			// Check for conditional bonus.
			auto conditionalPowerComponent = this->game->entityManager->GetComponent<ConditionalPowerComponent>(effectPlayedEvent.effectEntity, ConditionalPowerComponent::ConditionalPowerComponentType);

			if (conditionalPowerComponent != nullptr)
			{
				auto conditionNotDamagedComponent = this->game->entityManager->GetComponent<ConditionNotDamagedComponent>(effectPlayedEvent.effectEntity, ConditionNotDamagedComponent::ConditionNotDamagedComponentType);

				if (conditionNotDamagedComponent != nullptr)
				{
					// Check if ship is damaged.
					auto damagedShip = std::find(this->damagedShips.begin(), this->damagedShips.end(), effectPlayedEvent.targetEntity);

					if (damagedShip == this->damagedShips.end())
					{
						// Conditions met - increase power.
						bonusPower = conditionalPowerComponent->conditionalPower;
					}
				}
			}

			// Apply power bonus.
			targetPowerComponent->bonusPowerUntilEndOfTurn += bonusPower;
			targetPowerComponent->power += bonusPower;

			// Notify listeners.
			auto bonusPowerChangedEvent = std::make_shared<BonusPowerChangedEvent>(effectPlayedEvent.targetEntity);
			this->game->eventManager->QueueEvent(bonusPowerChangedEvent);

			// Notify listeners.
			auto powerChangedEvent = std::make_shared<PowerChangedEvent>(effectPlayedEvent.targetEntity, targetPowerComponent->power);
			this->game->eventManager->QueueEvent(powerChangedEvent);
		}
	}

	// Remove from player hand.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(effectPlayedEvent.effectEntity, OwnerComponent::OwnerComponentType);
	auto playerDeckComponent = this->game->entityManager->GetComponent<PlayerDeckComponent>(ownerComponent->owner, PlayerDeckComponent::PlayerDeckComponentType);
	playerDeckComponent->hand.remove(effectPlayedEvent.effectEntity);

	// Remove card.
	this->game->entityManager->RemoveEntity(effectPlayedEvent.effectEntity);
}

void EffectPlayingSystem::OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent)
{
	this->damagedShips.remove(entityRemovedEvent.entity);
}

void EffectPlayingSystem::OnShipDamaged(ShipDamagedEvent& shipDamagedEvent)
{
	this->damagedShips.push_back(shipDamagedEvent.damagedShip);
}

void EffectPlayingSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentTurnPhase = turnPhaseChangedEvent.newTurnPhase;
}