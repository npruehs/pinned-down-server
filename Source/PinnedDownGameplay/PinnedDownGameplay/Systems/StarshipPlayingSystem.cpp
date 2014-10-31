#include "Event.h"
#include "StarshipPlayingSystem.h"

#include "..\Actions\AddThreatAction.h"

#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"
#include "..\Components\PlayerDeckComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\CardStateChangedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Events;
using namespace PinnedDownServer::Systems;


StarshipPlayingSystem::StarshipPlayingSystem()
{
}

void StarshipPlayingSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);
}

void StarshipPlayingSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == StarshipPlayedEvent::StarshipPlayedEventType)
	{
		auto starshipPlayedEvent = static_cast<StarshipPlayedEvent&>(newEvent);
		this->OnStarshipPlayed(starshipPlayedEvent);
	}
}

void StarshipPlayingSystem::OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent)
{
	// Put card in play.
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(starshipPlayedEvent.shipEntity, CardStateComponent::CardStateComponentType);
	cardStateComponent->cardState = CardState::InPlay;

	// Remove from player hand.
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(starshipPlayedEvent.shipEntity, OwnerComponent::OwnerComponentType);
	auto playerDeckComponent = this->game->entityManager->GetComponent<PlayerDeckComponent>(ownerComponent->owner, PlayerDeckComponent::PlayerDeckComponentType);
	playerDeckComponent->hand.remove(starshipPlayedEvent.shipEntity);

	// Increase threat.
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(starshipPlayedEvent.shipEntity, ThreatComponent::ThreatComponentType);
	auto addThreatAction = std::make_shared<AddThreatAction>(threatComponent->threat);
	this->game->eventManager->QueueEvent(addThreatAction);

	// Notify listeners.
	auto cardStateChangedEvent = std::make_shared<CardStateChangedEvent>(starshipPlayedEvent.shipEntity, cardStateComponent->cardState);
	this->game->eventManager->QueueEvent(cardStateChangedEvent);
}
