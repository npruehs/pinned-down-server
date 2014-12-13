#include "Event.h"
#include "DamageSystem.h"

#include "Events\PowerChangedEvent.h"
#include "Events\StructureChangedEvent.h"
#include "Events\ShipDamagedEvent.h"

#include "Components\PowerComponent.h"
#include "Components\StructureComponent.h"

using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
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
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, ShipDefeatedEvent::ShipDefeatedEventType);
}

void DamageSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EntityRemovedEvent);
	CALL_EVENT_HANDLER(ShipDefeatedEvent);
}

EVENT_HANDLER_DEFINITION(DamageSystem, EntityRemovedEvent)
{
	auto entity = data.entity;

	// Remove all damage attached to removed ship.
	for (auto iterator = this->damageList.begin(); iterator != this->damageList.end();)
	{
		if (iterator->damagedShip == entity)
		{
			this->game->entityManager->RemoveEntity(iterator->damage);
			iterator = this->damageList.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
}

EVENT_HANDLER_DEFINITION(DamageSystem, ShipDefeatedEvent)
{
	auto damagedShipEntity = data.shipEntity;

	// Get top damage card.
	if (this->damageDeck->IsEmpty())
	{
		this->PrepareDamageDeck();
	}

	CardData topCard = this->damageDeck->Draw();
	auto damageCardEntity = this->cardFactory->CreateCard(INVALID_ENTITY_ID, topCard.setIndex, topCard.cardIndex, CardState::InPlay);

	this->damageList.push_back(DamageData(damagedShipEntity, damageCardEntity));

	// Modify ship stats.
	auto damagePowerComponent = this->game->entityManager->GetComponent<PowerComponent>(damageCardEntity, PowerComponent::PowerComponentType);
	auto damageStructureComponent = this->game->entityManager->GetComponent<StructureComponent>(damageCardEntity, StructureComponent::StructureComponentType);
	auto shipPowerComponent = this->game->entityManager->GetComponent<PowerComponent>(damagedShipEntity, PowerComponent::PowerComponentType);
	auto shipStructureComponent = this->game->entityManager->GetComponent<StructureComponent>(damagedShipEntity, StructureComponent::StructureComponentType);

	shipPowerComponent->power += damagePowerComponent->power;
	shipStructureComponent->structure += damageStructureComponent->structure;

	// Notify listeners.
	auto shipDamagedEvent = std::make_shared<ShipDamagedEvent>(damagedShipEntity, damageCardEntity);
	this->game->eventManager->QueueEvent(shipDamagedEvent);

	auto powerChangedEvent = std::make_shared<PowerChangedEvent>(damagedShipEntity, shipPowerComponent->power);
	this->game->eventManager->QueueEvent(powerChangedEvent);

	auto structureChangedEvent = std::make_shared<StructureChangedEvent>(damagedShipEntity, shipStructureComponent->structure);
	this->game->eventManager->QueueEvent(structureChangedEvent);

	// Check for destruction.
	if (shipStructureComponent->structure <= 0)
	{
		// Remove ship.
		this->game->entityManager->RemoveEntity(damagedShipEntity);
	}
}

void DamageSystem::PrepareDamageDeck()
{
	// Add cards.
	this->damageDeck = std::make_shared<Deck>();

	this->damageDeck->Add(CardData(0, 0));
	this->damageDeck->Add(CardData(0, 0));
	this->damageDeck->Add(CardData(0, 1));
	this->damageDeck->Add(CardData(0, 1));
	this->damageDeck->Add(CardData(0, 3));
	this->damageDeck->Add(CardData(0, 3));
	this->damageDeck->Add(CardData(0, 4));
	this->damageDeck->Add(CardData(0, 4));
	this->damageDeck->Add(CardData(0, 5));
	this->damageDeck->Add(CardData(0, 5));
	this->damageDeck->Add(CardData(0, 6));
	this->damageDeck->Add(CardData(0, 6));
	this->damageDeck->Add(CardData(0, 7));
	this->damageDeck->Add(CardData(0, 7));

	// Shuffle deck.
	auto random = std::make_shared<Random>();
	this->damageDeck->Shuffle(random);
}
