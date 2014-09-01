#include "Event.h"
#include "FightSystem.h"

#include "Components\PowerComponent.h"

#include "Events\FightResolvedEvent.h"

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

	// Remove defeated ship.
	if (playerPowerComponent->power <= enemyPowerComponent->power)
	{
		this->game->entityManager->RemoveEntity(fightStartedEvent.playerShip);
	}
	else
	{
		this->game->entityManager->RemoveEntity(fightStartedEvent.enemyShip);
	}

	// Notify client.
	auto fightResolvedEvent = std::make_shared<FightResolvedEvent>(fightStartedEvent.playerShip);
	this->game->eventManager->QueueEvent(fightResolvedEvent);
}
