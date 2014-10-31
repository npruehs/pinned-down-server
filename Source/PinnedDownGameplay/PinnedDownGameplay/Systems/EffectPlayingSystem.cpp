#include "Event.h"
#include "EffectPlayingSystem.h"

#include "..\Actions\AddThreatAction.h"

#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\PowerComponent.h"
#include "Components\ThreatComponent.h"

#include "..\Events\BonusPowerChangedEvent.h"
#include "Events\PowerChangedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Events;
using namespace PinnedDownServer::Systems;


EffectPlayingSystem::EffectPlayingSystem()
{
}

void EffectPlayingSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EffectPlayedEvent::EffectPlayedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void EffectPlayingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EffectPlayedEvent::EffectPlayedEventType)
	{
		auto effectPlayedEvent = static_cast<EffectPlayedEvent&>(newEvent);
		this->OnEffectPlayed(effectPlayedEvent);
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
			// Apply power bonus.
			targetPowerComponent->bonusPowerUntilEndOfTurn += effectPowerComponent->bonusPowerUntilEndOfTurn;
			targetPowerComponent->power += effectPowerComponent->bonusPowerUntilEndOfTurn;

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

void EffectPlayingSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentTurnPhase = turnPhaseChangedEvent.newTurnPhase;
}