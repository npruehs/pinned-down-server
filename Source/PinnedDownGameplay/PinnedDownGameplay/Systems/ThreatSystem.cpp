#include "Event.h"
#include "ThreatSystem.h"

#include "Actions\EndTurnAction.h"

#include "Components\ThreatComponent.h"

#include "Events\ThreatChangedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


ThreatSystem::ThreatSystem()
	: threat(0)
{
}

void ThreatSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EnemyCardPlayedEvent::EnemyCardPlayedEventType);
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
}

void ThreatSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentTurnPhase = turnPhaseChangedEvent.newTurnPhase;

	if (turnPhaseChangedEvent.newTurnPhase == TurnPhase::Jump)
	{
		// Increase threat.
		auto newThreat = this->threat + 3;
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