#include "Event.h"
#include "TemporaryEffectSystem.h"

#include "Components\PowerComponent.h"

#include "Events\PowerChangedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


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
	CALL_EVENT_HANDLER(BonusPowerChangedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(TemporaryEffectSystem, BonusPowerChangedEvent)
{
	this->buffedEntities.push_back(data.entity);
}

EVENT_HANDLER_DEFINITION(TemporaryEffectSystem, TurnPhaseChangedEvent)
{
	if (data.newTurnPhase == TurnPhase::Main)
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
