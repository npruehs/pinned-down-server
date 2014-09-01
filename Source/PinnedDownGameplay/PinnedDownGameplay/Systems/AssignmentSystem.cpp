#include "Event.h"
#include "AssignmentSystem.h"

#include "Components\OwnerComponent.h"

#include "Events\CardAssignedEvent.h"
#include "..\Events\FightStartedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownServer::Systems;


AssignmentSystem::AssignmentSystem()
{
}

void AssignmentSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, AssignCardAction::AssignCardActionType);
	this->game->eventManager->AddListener(this, ResolveFightAction::ResolveFightActionType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AssignCardAction::AssignCardActionType)
	{
		auto assignCardAction = static_cast<AssignCardAction&>(newEvent);
		this->OnAssignCard(assignCardAction);
	}
	else if (newEvent.GetEventType() == ResolveFightAction::ResolveFightActionType)
	{
		auto resolveFightAction = static_cast<ResolveFightAction&>(newEvent);
		this->OnResolveFight(resolveFightAction);
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

void AssignmentSystem::OnResolveFight(ResolveFightAction& resolveFightAction)
{
	// Get assignment.
	auto assignedCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(resolveFightAction.assignedCard, OwnerComponent::OwnerComponentType);

	if (assignedCardOwner != nullptr && assignedCardOwner->owner != INVALID_ENTITY_ID)
	{
		auto assignment = this->currentAssignments.find(resolveFightAction.assignedCard);

		if (assignment != this->currentAssignments.end())
		{
			// Start fight.
			auto fightStartedEvent = std::make_shared<FightStartedEvent>(assignment->first, assignment->second);
			this->game->eventManager->QueueEvent(fightStartedEvent);
		}
	}
}
