#include "Event.h"
#include "CardPlayingSystem.h"

#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\ThreatComponent.h"

#include "..\Events\EffectPlayedEvent.h"
#include "..\Events\StarshipPlayedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


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
	if (newEvent.GetEventType() == PlayCardAction::PlayCardActionType)
	{
		auto playCardAction = static_cast<PlayCardAction&>(newEvent);
		this->OnPlayCard(playCardAction);
	}
}

void CardPlayingSystem::OnPlayCard(PlayCardAction& playCardAction)
{
	// Can only play cards from hand.
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(playCardAction.cardToPlay, CardStateComponent::CardStateComponentType);

	if (cardStateComponent == nullptr || cardStateComponent->cardState != CardState::Hand)
	{
		return;
	}

	// Only play own cards.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(playCardAction.cardToPlay, OwnerComponent::OwnerComponentType);

	if (ownerComponent == nullptr || ownerComponent->owner == INVALID_ENTITY_ID)
	{
		return;
	}

	// Check card type.
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(playCardAction.cardToPlay, CardComponent::CardComponentType);

	if (cardComponent->cardType == CardType::Starship)
	{
		// Notify listeners.
		auto starshipPlayedEvent = std::make_shared<StarshipPlayedEvent>(playCardAction.cardToPlay);
		this->game->eventManager->QueueEvent(starshipPlayedEvent);
	}
	else if (cardComponent->cardType == CardType::Effect)
	{
		// Notify listeners.
		auto effectPlayedEvent = std::make_shared<EffectPlayedEvent>(playCardAction.cardToPlay, playCardAction.targetCard);
		this->game->eventManager->QueueEvent(effectPlayedEvent);
	}
}