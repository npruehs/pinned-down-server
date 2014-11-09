#include "Event.h"
#include "FightSystem.h"

#include "Components\PowerComponent.h"

#include "Events\FightResolvedEvent.h"
#include "..\Events\ShipDefeatedEvent.h"
#include "..\Events\ShipVictoriousEvent.h"

using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
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
	// Compute player power.
	auto playerPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(fightStartedEvent.playerShip, PowerComponent::PowerComponentType);
	auto playerPower = playerPowerComponent->power;

	// Compute enemy power.
	auto enemyPower = 0;

	for (auto it = fightStartedEvent.enemyShips->begin(); it != fightStartedEvent.enemyShips->end(); ++it)
	{
		auto enemyPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(*it, PowerComponent::PowerComponentType);
		enemyPower += enemyPowerComponent->power;
	}

	// Compare power.
	if (playerPower <= enemyPower)
	{
		// Enemy victory.
		for (auto it = fightStartedEvent.enemyShips->begin(); it != fightStartedEvent.enemyShips->end(); ++it)
		{
			auto shipVictoriousEvent = std::make_shared<ShipVictoriousEvent>(*it);
			this->game->eventManager->QueueEvent(shipVictoriousEvent);
		}

		auto shipDefeatedEvent = std::make_shared<ShipDefeatedEvent>(fightStartedEvent.playerShip);
		this->game->eventManager->QueueEvent(shipDefeatedEvent);
	}
	else
	{
		// Player victory.
		auto shipVictoriousEvent = std::make_shared<ShipVictoriousEvent>(fightStartedEvent.playerShip);
		this->game->eventManager->QueueEvent(shipVictoriousEvent);

		for (auto it = fightStartedEvent.enemyShips->begin(); it != fightStartedEvent.enemyShips->end(); ++it)
		{
			auto shipDefeatedEvent = std::make_shared<ShipDefeatedEvent>(*it);
			this->game->eventManager->QueueEvent(shipDefeatedEvent);
		}
	}

	// Notify client.
	auto fightResolvedEvent = std::make_shared<FightResolvedEvent>(fightStartedEvent.playerShip);
	this->game->eventManager->QueueEvent(fightResolvedEvent);
}
