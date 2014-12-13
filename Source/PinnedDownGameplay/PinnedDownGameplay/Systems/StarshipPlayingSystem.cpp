#include "Event.h"
#include "StarshipPlayingSystem.h"

#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\CardStateChangedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;


StarshipPlayingSystem::StarshipPlayingSystem()
{
}

void StarshipPlayingSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void StarshipPlayingSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(StarshipPlayedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(StarshipPlayingSystem, StarshipPlayedEvent)
{
	if (this->currentTurnPhase != TurnPhase::Main)
	{
		return;
	}

	// Put card in play.
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(data.shipEntity, CardStateComponent::CardStateComponentType);
	cardStateComponent->cardState = CardState::InPlay;

	// Remove from player hand.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(data.shipEntity, OwnerComponent::OwnerComponentType);
	auto playerDeckComponent = this->game->entityManager->GetComponent<PlayerDeckComponent>(ownerComponent->owner, PlayerDeckComponent::PlayerDeckComponentType);
	playerDeckComponent->hand.remove(data.shipEntity);

	// Notify listeners.
	auto cardStateChangedEvent = std::make_shared<CardStateChangedEvent>(data.shipEntity, cardStateComponent->cardState);
	this->game->eventManager->QueueEvent(cardStateChangedEvent);
}

EVENT_HANDLER_DEFINITION(StarshipPlayingSystem, TurnPhaseChangedEvent)
{
	this->currentTurnPhase = data.newTurnPhase;
}