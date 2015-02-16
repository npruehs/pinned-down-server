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
	CALL_EVENT_HANDLER(EntityRemovedEvent);
}

EVENT_HANDLER_DEFINITION(FlagshipDefeatSystem, EntityRemovedEvent)
{
	auto flagshipComponent = this->game->entityManager->GetComponent<FlagshipComponent>(data.entity, FlagshipComponent::FlagshipComponentType);

	if (flagshipComponent != nullptr)
	{
		auto defeatEvent = std::make_shared<DefeatEvent>();
		this->game->eventManager->QueueEvent(defeatEvent);
	}
}