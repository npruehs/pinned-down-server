#include "Event.h"
#include "AssignmentSystem.h"

#include "Components\OwnerComponent.h"

#include "Events\CardAssignedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


AssignmentSystem::AssignmentSystem()
{
}

void AssignmentSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, AssignCardAction::AssignCardActionType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AssignCardAction::AssignCardActionType)
	{
		auto assignCardAction = static_cast<AssignCardAction&>(newEvent);
		this->OnAssignCard(assignCardAction);
	}
}

void AssignmentSystem::OnAssignCard(AssignCardAction& assignCardAction)
{
	// Check owners.
	auto assignedCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(assignCardAction.assignedCard, OwnerComponent::OwnerComponentType);
	auto targetCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(assignCardAction.targetCard, OwnerComponent::OwnerComponentType);

	if (assignedCardOwner != nullptr && assignedCardOwner->owner != INVALID_ENTITY_ID && targetCardOwner != nullptr && targetCardOwner->owner == INVALID_ENTITY_ID)
	{
		// Check for previous assignment.
		auto assignment = this->currentAssignments.find(assignCardAction.assignedCard);

		if (assignment != this->currentAssignments.end())
		{
			this->currentAssignments.erase(assignment);
		}

		// Assign card.
		this->currentAssignments.insert(std::pair<Entity, Entity>(assignCardAction.assignedCard, assignCardAction.targetCard));

		// Notify listeners.
		auto cardAssignedEvent = std::make_shared<CardAssignedEvent>(assignCardAction.assignedCard, assignCardAction.targetCard);
		this->game->eventManager->QueueEvent(cardAssignedEvent);
	}
}
