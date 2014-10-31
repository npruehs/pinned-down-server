#include "Event.h"
#include "TemporaryEffectSystem.h"

#include "Components\PowerComponent.h"

#include "Events\PowerChangedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


TemporaryEffectSystem::TemporaryEffectSystem()
{
}

void TemporaryEffectSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, BonusPowerChangedEvent::BonusPowerChangedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void TemporaryEffectSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == BonusPowerChangedEvent::BonusPowerChangedEventType)
	{
		auto bonusPowerChangedEvent = static_cast<BonusPowerChangedEvent&>(newEvent);
		this->OnBonusPowerChanged(bonusPowerChangedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void TemporaryEffectSystem::OnBonusPowerChanged(BonusPowerChangedEvent& bonusPowerChangedEvent)
{
	this->buffedEntities.push_back(bonusPowerChangedEvent.entity);
}

void TemporaryEffectSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Main)
	{
		for (auto it = this->buffedEntities.begin(); it != this->buffedEntities.end(); ++it)
		{
			auto entity = *it;
			auto powerComponent = this->game->entityManager->GetComponent<PowerComponent>(entity, PowerComponent::PowerComponentType);

			if (powerComponent != nullptr)
			{
				powerComponent->power -= powerComponent->bonusPowerUntilEndOfTurn;
				powerComponent->bonusPowerUntilEndOfTurn = 0;

				// Notify listeners.
				auto powerChangedEvent = std::make_shared<PowerChangedEvent>(entity, powerComponent->power);
				this->game->eventManager->QueueEvent(powerChangedEvent);
			}
		}

		this->buffedEntities.clear();
	}
}
