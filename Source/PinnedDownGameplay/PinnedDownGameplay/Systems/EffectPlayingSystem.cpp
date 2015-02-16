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
#include "Events\EffectPlayedEvent.h"
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

	this->game->eventManager->AddListener(this, PlayEffectAction::PlayEffectActionType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, ShipDamagedEvent::ShipDamagedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void EffectPlayingSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PlayEffectAction);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
	CALL_EVENT_HANDLER(ShipDamagedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(EffectPlayingSystem, PlayEffectAction)
{
	if (this->currentTurnPhase != TurnPhase::Fight)
	{
		return;
	}

	// Check for power bonus.
	auto effectPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(data.effectEntity, PowerComponent::PowerComponentType);
	
	if (effectPowerComponent != nullptr)
	{
		// Get target.
		auto targetPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(data.targetEntity, PowerComponent::PowerComponentType);

		if (targetPowerComponent != nullptr)
		{
			int bonusPower = effectPowerComponent->bonusPowerUntilEndOfTurn;

			// Check for conditional bonus.
			auto conditionalPowerComponent = this->game->entityManager->GetComponent<ConditionalPowerComponent>(data.effectEntity, ConditionalPowerComponent::ConditionalPowerComponentType);

			if (conditionalPowerComponent != nullptr)
			{
				auto conditionNotDamagedComponent = this->game->entityManager->GetComponent<ConditionNotDamagedComponent>(data.effectEntity, ConditionNotDamagedComponent::ConditionNotDamagedComponentType);

				if (conditionNotDamagedComponent != nullptr)
				{
					// Check if ship is damaged.
					auto damagedShip = std::find(this->damagedShips.begin(), this->damagedShips.end(), data.targetEntity);

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
			auto bonusPowerChangedEvent = std::make_shared<BonusPowerChangedEvent>(data.targetEntity);
			this->game->eventManager->QueueEvent(bonusPowerChangedEvent);

			// Notify listeners.
			auto powerChangedEvent = std::make_shared<PowerChangedEvent>(data.targetEntity, targetPowerComponent->power);
			this->game->eventManager->QueueEvent(powerChangedEvent);
		}
	}

	// Remove from player hand.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(data.effectEntity, OwnerComponent::OwnerComponentType);
	auto playerDeckComponent = this->game->entityManager->GetComponent<PlayerDeckComponent>(ownerComponent->owner, PlayerDeckComponent::PlayerDeckComponentType);
	playerDeckComponent->hand.remove(data.effectEntity);

	// Notify listeners.
	auto effectPlayedEvent = std::make_shared<EffectPlayedEvent>(data.effectEntity, data.targetEntity);
	this->game->eventManager->QueueEvent(effectPlayedEvent);

	// Remove card.
	this->game->entityManager->RemoveEntity(data.effectEntity);
}

EVENT_HANDLER_DEFINITION(EffectPlayingSystem, EntityRemovedEvent)
{
	this->damagedShips.remove(data.entity);
}

EVENT_HANDLER_DEFINITION(EffectPlayingSystem, ShipDamagedEvent)
{
	this->damagedShips.push_back(data.damagedShip);
}

EVENT_HANDLER_DEFINITION(EffectPlayingSystem, TurnPhaseChangedEvent)
{
	this->currentTurnPhase = data.newTurnPhase;
}