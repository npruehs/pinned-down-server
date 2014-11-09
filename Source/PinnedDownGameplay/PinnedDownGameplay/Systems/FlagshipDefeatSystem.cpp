#include "Event.h"
#include "FlagshipDefeatSystem.h"

#include "Components\FlagshipComponent.h"

#include "Events\DefeatEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Systems;


FlagshipDefeatSystem::FlagshipDefeatSystem()
{
}

void FlagshipDefeatSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
}

void FlagshipDefeatSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == EntityRemovedEvent::EntityRemovedEventType)
	{
		auto entityRemovedEvent = static_cast<EntityRemovedEvent&>(newEvent);
		this->OnEntityRemoved(entityRemovedEvent);
	}
}

void FlagshipDefeatSystem::OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent)
{
	auto flagshipComponent = this->game->entityManager->GetComponent<FlagshipComponent>(entityRemovedEvent.entity, FlagshipComponent::FlagshipComponentType);

	if (flagshipComponent != nullptr)
	{
		auto defeatEvent = std::make_shared<DefeatEvent>();
		this->game->eventManager->QueueEvent(defeatEvent);
	}
}