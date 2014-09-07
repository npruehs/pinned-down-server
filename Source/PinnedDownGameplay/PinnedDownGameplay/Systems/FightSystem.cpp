#include "Event.h"
#include "FightSystem.h"

#include "Components\PowerComponent.h"

#include "Events\FightResolvedEvent.h"
#include "..\Events\ShipDefeatedEvent.h"
#include "..\Events\ShipVictoriousEvent.h"

using namespace PinnedDownGameplay::Events;
using namespace PinnedDownServer::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


FightSystem::FightSystem()
{
}

void FightSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, FightStartedEvent::FightStartedEventType);
}

void FightSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == FightStartedEvent::FightStartedEventType)
	{
		auto fightStartedEvent = static_cast<FightStartedEvent&>(newEvent);
		this->OnFightStarted(fightStartedEvent);
	}
}

void FightSystem::OnFightStarted(FightStartedEvent& fightStartedEvent)
{
	// Compare power.
	auto playerPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(fightStartedEvent.playerShip, PowerComponent::PowerComponentType);
	auto enemyPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(fightStartedEvent.enemyShip, PowerComponent::PowerComponentType);

	// Notify listeners.
	auto winner = playerPowerComponent->power <= enemyPowerComponent->power ? fightStartedEvent.enemyShip : fightStartedEvent.playerShip;
	auto loser = playerPowerComponent->power <= enemyPowerComponent->power ? fightStartedEvent.playerShip : fightStartedEvent.enemyShip;

	auto shipVictoriousEvent = std::make_shared<ShipVictoriousEvent>(winner);
	this->game->eventManager->QueueEvent(shipVictoriousEvent);
	auto shipDefeatedEvent = std::make_shared<ShipDefeatedEvent>(loser);
	this->game->eventManager->QueueEvent(shipDefeatedEvent);

	// Notify client.
	auto fightResolvedEvent = std::make_shared<FightResolvedEvent>(fightStartedEvent.playerShip);
	this->game->eventManager->QueueEvent(fightResolvedEvent);
}
