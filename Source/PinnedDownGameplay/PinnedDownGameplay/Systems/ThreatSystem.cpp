#include "Event.h"
#include "ThreatSystem.h"

#include "Components\CardComponent.h"
#include "Components\OwnerComponent.h"
#include "Components\ThreatComponent.h"

#include "Events\ThreatChangedEvent.h"
#include "..\Events\TurnPhaseEndedEvent.h"

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
	this->game->eventManager->AddListener(this, GameStartedEvent::GameStartedEventType);
	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void ThreatSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EnemyCardPlayedEvent);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
	CALL_EVENT_HANDLER(FlagshipPlayedEvent);
	CALL_EVENT_HANDLER(GameStartedEvent);
	CALL_EVENT_HANDLER(StarshipPlayedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(ThreatSystem, EnemyCardPlayedEvent)
{
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(data.cardEntity, ThreatComponent::ThreatComponentType);
	auto newThreat = this->threat - threatComponent->threat;
	newThreat = newThreat < 0 ? 0 : newThreat;
	this->SetThreat(newThreat);
}

EVENT_HANDLER_DEFINITION(ThreatSystem, EntityRemovedEvent)
{
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(data.entity, CardComponent::CardComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(data.entity, OwnerComponent::OwnerComponentType);

	if (cardComponent == nullptr || ownerComponent == nullptr || cardComponent->cardType != CardType::Starship || ownerComponent->owner == INVALID_ENTITY_ID)
	{
		return;
	}

	// Count player ships.
	--this->playerShips;
}

EVENT_HANDLER_DEFINITION(ThreatSystem, FlagshipPlayedEvent)
{
	// Count player ships.
	++this->playerShips;
}

EVENT_HANDLER_DEFINITION(ThreatSystem, GameStartedEvent)
{
	// Set initial threat value.
	this->SetThreat(initialThreat);
}

EVENT_HANDLER_DEFINITION(ThreatSystem, StarshipPlayedEvent)
{
	if (this->currentTurnPhase != TurnPhase::Main)
	{
		return;
	}

	// Increase threat.
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(data.shipEntity, ThreatComponent::ThreatComponentType);
	auto newThreat = this->threat + threatComponent->threat;
	this->SetThreat(newThreat);

	// Count player ships.
	++this->playerShips;
}

EVENT_HANDLER_DEFINITION(ThreatSystem, TurnPhaseChangedEvent)
{
	this->currentTurnPhase = data.newTurnPhase;

	if (data.newTurnPhase == TurnPhase::Jump)
	{
		// Increase threat.
		++this->locationThreat;

		auto newThreat = this->threat + this->locationThreat + this->playerShips;
		this->SetThreat(newThreat);

		// End jump phase.
		auto turnPhaseEndedEvent = std::make_shared<TurnPhaseEndedEvent>();
		this->game->eventManager->QueueEvent(turnPhaseEndedEvent);
	}
}

void ThreatSystem::SetThreat(int newThreat)
{
	this->threat = newThreat;

	// Notify listeners.
	auto threatChangedEvent = std::make_shared<ThreatChangedEvent>(this->threat);
	this->game->eventManager->QueueEvent(threatChangedEvent);
}