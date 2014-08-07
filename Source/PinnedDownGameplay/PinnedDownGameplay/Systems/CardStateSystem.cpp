#include "Event.h"
#include "CardStateSystem.h"

#include "Components\CardComponent.h"

#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


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
	if (newEvent.GetEventType() == EntityInitializedEvent::EntityInitializedEventType)
	{
		auto entityInitializedEvent = static_cast<EntityInitializedEvent&>(newEvent);
		this->OnEntityInitialized(entityInitializedEvent);
	}
	else if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		auto entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->OnEntityRemoved(entityRemovedEvent);
	}
}

void CardStateSystem::OnEntityInitialized(EntityInitializedEvent& entityInitializedEvent)
{
	auto entity = entityInitializedEvent.entity;
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entity, CardComponent::CardComponentType);

	if (cardComponent != nullptr)
	{
		// Notify client.
		auto cardCreatedEvent = std::make_shared<CardCreatedEvent>(entity, cardComponent->setIndex, cardComponent->cardIndex);
		this->game->eventManager->QueueEvent(cardCreatedEvent);
	}
}

void CardStateSystem::OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent)
{
	auto entity = entityRemovedEvent.entity;
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(entity, CardComponent::CardComponentType);

	if (cardComponent != nullptr)
	{
		// Notify client.
		auto cardRemovedEvent = std::make_shared<CardRemovedEvent>(entity);
		this->game->eventManager->QueueEvent(cardRemovedEvent);
	}
}