#include "Event.h"
#include "CardPlayingSystem.h"

#include "..\Actions\AddThreatAction.h"

#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\CardStateChangedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Events;
using namespace PinnedDownServer::Systems;


CardPlayingSystem::CardPlayingSystem()
{
}

void CardPlayingSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, PlayCardAction::PlayCardActionType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void CardPlayingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == PlayCardAction::PlayCardActionType)
	{
		auto playCardAction = static_cast<PlayCardAction&>(newEvent);
		this->OnPlayCard(playCardAction);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void CardPlayingSystem::OnPlayCard(PlayCardAction& playCardAction)
{
	if (this->currentTurnPhase != TurnPhase::Main)
	{
		return;
	}

	// Check owners.
	auto playedCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(playCardAction.cardToPlay, OwnerComponent::OwnerComponentType);

	if (playedCardOwner != nullptr && playedCardOwner->owner != INVALID_ENTITY_ID)
	{
		// Put card in play.
		auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(playCardAction.cardToPlay, OwnerComponent::OwnerComponentType);
		cardStateComponent->cardState = CardState::InPlay;

		// Remove from player hand.
		auto playerDeckComponent = this->game->entityManager->GetComponent<PlayerDeckComponent>(playedCardOwner->owner, PlayerDeckComponent::PlayerDeckComponentType);
		playerDeckComponent->hand.remove(playCardAction.cardToPlay);

		// Increase threat.
		auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(playCardAction.cardToPlay, ThreatComponent::ThreatComponentType);
		auto addThreatAction = std::make_shared<AddThreatAction>(threatComponent->threat);
		this->game->eventManager->QueueEvent(addThreatAction);

		// Notify listeners.
		auto cardStateChangedEvent = std::make_shared<CardStateChangedEvent>(playCardAction.cardToPlay, cardStateComponent->cardState);
		this->game->eventManager->QueueEvent(cardStateChangedEvent);
	}
}

void CardPlayingSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentTurnPhase = turnPhaseChangedEvent.newTurnPhase;
}