#include "Event.h"
#include "ThreatSystem.h"

#include "Actions\EndTurnAction.h"

#include "Components\CardComponent.h"
#include "Components\OwnerComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\ThreatChangedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


ThreatSystem::ThreatSystem()
	: threat(0),
	locationThreat(0),
	playerShips(0)
{
}

void ThreatSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EnemyCardPlayedEvent::EnemyCardPlayedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, FlagshipPlayedEvent::FlagshipPlayedEventType);
	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);

	// Set initial threat value.
	this->SetThreat(0);
}

void ThreatSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EnemyCardPlayedEvent::EnemyCardPlayedEventType)
	{
		auto enemyCardPlayedEvent = static_cast<EnemyCardPlayedEvent&>(newEvent);
		this->OnEnemyCardPlayed(enemyCardPlayedEvent);
	}
	else if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		auto entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->OnEntityRemoved(entityRemovedEvent);
	}
	else if (newEvent.GetEventType() == FlagshipPlayedEvent::FlagshipPlayedEventType)
	{
		auto flagshipPlayedEvent = static_cast<FlagshipPlayedEvent&>(newEvent);
		this->OnFlagshipPlayed(flagshipPlayedEvent);
	}
	else if (newEvent.GetEventType() == StarshipPlayedEvent::StarshipPlayedEventType)
	{
		auto starshipPlayedEvent = static_cast<StarshipPlayedEvent&>(newEvent);
		this->OnStarshipPlayed(starshipPlayedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void ThreatSystem::OnEnemyCardPlayed(EnemyCardPlayedEvent& enemyCardPlayedEvent)
{
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(enemyCardPlayedEvent.cardEntity, ThreatComponent::ThreatComponentType);
	auto newThreat = this->threat - threatComponent->threat;
	newThreat = newThreat < 0 ? 0 : newThreat;
	this->SetThreat(newThreat);
}

void ThreatSystem::OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent)
{
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entityRemovedEvent.entity, CardComponent::CardComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(entityRemovedEvent.entity, OwnerComponent::OwnerComponentType);

	if (cardComponent == nullptr || ownerComponent == nullptr || cardComponent->cardType != CardType::Starship || ownerComponent->owner == INVALID_ENTITY_ID)
	{
		return;
	}

	// Count player ships.
	--this->playerShips;
}

void ThreatSystem::OnFlagshipPlayed(FlagshipPlayedEvent& flagshipPlayedEvent)
{
	// Count player ships.
	++this->playerShips;
}

void ThreatSystem::OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent)
{
	if (this->currentTurnPhase != TurnPhase::Main)
	{
		return;
	}

	// Increase threat.
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(starshipPlayedEvent.shipEntity, ThreatComponent::ThreatComponentType);
	auto newThreat = this->threat + threatComponent->threat;
	this->SetThreat(newThreat);

	// Count player ships.
	++this->playerShips;
}

void ThreatSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentTurnPhase = turnPhaseChangedEvent.newTurnPhase;

	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Jump)
	{
		// Increase threat.
		++this->locationThreat;

		auto newThreat = this->threat + this->locationThreat + this->playerShips;
		this->SetThreat(newThreat);

		// End jump phase.
		auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Main);
		this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
	}
}

void ThreatSystem::SetThreat(int newThreat)
{
	this->threat = newThreat;

	// Notify listeners.
	auto threatChangedEvent = std::make_shared<ThreatChangedEvent>(this->threat);
	this->game->eventManager->QueueEvent(threatChangedEvent);
}