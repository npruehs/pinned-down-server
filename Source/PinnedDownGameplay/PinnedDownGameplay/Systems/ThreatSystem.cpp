#include "Event.h"
#include "ThreatSystem.h"

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

	this->game->eventManager->AddListener(this, AddThreatAction::AddThreatActionType);
	this->game->eventManager->AddListener(this, EnemyCardPlayedEvent::EnemyCardPlayedEventType);

	// Set initial threat value.
	this->SetThreat(0);
}

void ThreatSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AddThreatAction::AddThreatActionType)
	{
		auto addThreatAction = static_cast<AddThreatAction&>(newEvent);
		this->OnAddThreat(addThreatAction);
	}
	else if (newEvent.GetEventType() == EnemyCardPlayedEvent::EnemyCardPlayedEventType)
	{
		auto enemyCardPlayedEvent = static_cast<EnemyCardPlayedEvent&>(newEvent);
		this->OnEnemyCardPlayed(enemyCardPlayedEvent);
	}
}

void ThreatSystem::OnAddThreat(AddThreatAction& addThreatAction)
{
	this->SetThreat(this->threat + addThreatAction.threat);
}

void ThreatSystem::OnEnemyCardPlayed(EnemyCardPlayedEvent& enemyCardPlayedEvent)
{
	auto threatComponent = this->game->entityManager->GetComponent<ThreatComponent>(enemyCardPlayedEvent.cardEntity, ThreatComponent::ThreatComponentType);
	auto newThreat = this->threat - threatComponent->threat;
	newThreat = newThreat < 0 ? 0 : newThreat;
	this->SetThreat(newThreat);
}

void ThreatSystem::SetThreat(int newThreat)
{
	this->threat = newThreat;

	// Notify listeners.
	auto threatChangedEvent = std::make_shared<ThreatChangedEvent>(this->threat);
	this->game->eventManager->QueueEvent(threatChangedEvent);
}