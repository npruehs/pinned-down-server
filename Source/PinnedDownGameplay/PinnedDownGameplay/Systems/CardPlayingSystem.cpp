#include "Event.h"
#include "CardPlayingSystem.h"

#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\CardPlayedEvent.h"
#include "..\Events\EffectPlayedEvent.h"
#include "..\Events\StarshipPlayedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;


CardPlayingSystem::CardPlayingSystem()
{
}

void CardPlayingSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, PlayCardAction::PlayCardActionType);
}

void CardPlayingSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PlayCardAction);
}

EVENT_HANDLER_DEFINITION(CardPlayingSystem, PlayCardAction)
{
	// Can only play cards from hand.
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(data.cardToPlay, CardStateComponent::CardStateComponentType);

	if (cardStateComponent == nullptr || cardStateComponent->cardState != CardState::Hand)
	{
		return;
	}

	// Only play own cards.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(data.cardToPlay, OwnerComponent::OwnerComponentType);

	if (ownerComponent == nullptr || ownerComponent->owner == INVALID_ENTITY_ID)
	{
		return;
	}

	// Check card type.
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(data.cardToPlay, CardComponent::CardComponentType);

	auto cardPlayedEvent = std::make_shared<CardPlayedEvent>(data.cardToPlay);
	this->game->eventManager->QueueEvent(cardPlayedEvent);

	if (cardComponent->cardType == CardType::Starship)
	{
		// Notify listeners.
		auto starshipPlayedEvent = std::make_shared<StarshipPlayedEvent>(data.cardToPlay);
		this->game->eventManager->QueueEvent(starshipPlayedEvent);
	}
	else if (cardComponent->cardType == CardType::Effect)
	{
		// Notify listeners.
		auto effectPlayedEvent = std::make_shared<EffectPlayedEvent>(data.cardToPlay, data.targetCard);
		this->game->eventManager->QueueEvent(effectPlayedEvent);
	}
}