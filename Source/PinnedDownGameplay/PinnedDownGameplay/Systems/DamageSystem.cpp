#include "Event.h"
#include "DamageSystem.h"

#include "Events\ShipDamagedEvent.h"

#include "Components\PowerComponent.h"
#include "Components\StructureComponent.h"

using namespace PinnedDownGameplay::Events;
using namespace PinnedDownServer::Systems;
using namespace PinnedDownNet::Events;
using namespace PinnedDownNet::Components;


DamageSystem::DamageSystem()
{
}

void DamageSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Prepare attack deck.
	this->PrepareDamageDeck();

	// Register for events.
	this->game->eventManager->AddListener(this, ShipDefeatedEvent::ShipDefeatedEventType);
}

void DamageSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == ShipDefeatedEvent::ShipDefeatedEventType)
	{
		auto shipDefeatedEvent = static_cast<ShipDefeatedEvent&>(newEvent);
		this->OnShipDefeated(shipDefeatedEvent);
	}
}

void DamageSystem::OnShipDefeated(ShipDefeatedEvent& shipDefeatedEvent)
{
	// Get top damage card.
	if (this->damageDeck->IsEmpty())
	{
		this->PrepareDamageDeck();
	}

	CardData topCard = this->damageDeck->Draw();
	auto damageCardEntity = this->cardFactory->CreateCard(INVALID_ENTITY_ID, topCard.setIndex, topCard.cardIndex);

	// Modify ship stats.
	auto damagePowerComponent = this->game->entityManager->GetComponent<PowerComponent>(damageCardEntity, PowerComponent::PowerComponentType);
	auto damageStructureComponent = this->game->entityManager->GetComponent<StructureComponent>(damageCardEntity, StructureComponent::StructureComponentType);
	auto shipPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(shipDefeatedEvent.shipEntity, PowerComponent::PowerComponentType);
	auto shipStructureComponent = this->game->entityManager->GetComponent<StructureComponent>(shipDefeatedEvent.shipEntity, StructureComponent::StructureComponentType);

	shipPowerComponent->power += damagePowerComponent->power;
	shipStructureComponent->structure += damageStructureComponent->structure;

	// Notify listeners.
	auto shipDamagedEvent = std::make_shared<ShipDamagedEvent>(shipDefeatedEvent.shipEntity, damageCardEntity);
	this->game->eventManager->QueueEvent(shipDamagedEvent);
}

void DamageSystem::PrepareDamageDeck()
{
	// Add cards.
	this->damageDeck = std::make_shared<Deck>();

	this->damageDeck->Add(CardData(0, 3));
	this->damageDeck->Add(CardData(0, 3));

	// Shuffle deck.
	auto random = std::make_shared<Random>();
	this->damageDeck->Shuffle(random);
}
