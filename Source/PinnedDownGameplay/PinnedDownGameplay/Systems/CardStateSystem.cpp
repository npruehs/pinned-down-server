#include "Event.h"
#include "CardStateSystem.h"

#include "Components\CardComponent.h"
#include "Components\CardStateComponent.h"
#include "Components\OwnerComponent.h"

#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\CardStateChangedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


CardStateSystem::CardStateSystem()
{
}

void CardStateSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
}

void CardStateSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EntityInitializedEvent);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
}

EVENT_HANDLER_DEFINITION(CardStateSystem, EntityInitializedEvent)
{
	auto entity = data.entity;
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entity, CardComponent::CardComponentType);
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(entity, OwnerComponent::OwnerComponentType);
	auto cardStateComponent = this->game->entityManager->GetComponent<CardStateComponent>(entity, CardStateComponent::CardStateComponentType);

	if (cardComponent != nullptr && ownerComponent != nullptr)
	{
		// Notify client.
		auto cardCreatedEvent = std::make_shared<CardCreatedEvent>(entity, ownerComponent->owner, cardComponent->setIndex, cardComponent->cardIndex, cardStateComponent->cardState);
		this->game->eventManager->QueueEvent(cardCreatedEvent);
	}
}

EVENT_HANDLER_DEFINITION(CardStateSystem, EntityRemovedEvent)
{
	auto entity = data.entity;
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entity, CardComponent::CardComponentType);

	if (cardComponent != nullptr)
	{
		// Notify client.
		auto cardRemovedEvent = std::make_shared<CardRemovedEvent>(entity);
		this->game->eventManager->QueueEvent(cardRemovedEvent);
	}
}