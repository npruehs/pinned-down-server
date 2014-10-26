#include "Event.h"
#include "AssignmentSystem.h"

#include "Components\OwnerComponent.h"

#include "Events\CardAssignedEvent.h"
#include "Events\ErrorMessageEvent.h"
#include "..\Events\FightStartedEvent.h"

using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Data;
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
	this->game->eventManager->AddListener(this, CardCreatedEvent::CardCreatedEventType);
	this->game->eventManager->AddListener(this, CardRemovedEvent::CardRemovedEventType);
	this->game->eventManager->AddListener(this, EndTurnAction::EndTurnActionType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
	this->game->eventManager->AddListener(this, ResolveFightAction::ResolveFightActionType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AssignCardAction::AssignCardActionType)
	{
		auto assignCardAction = static_cast<AssignCardAction&>(newEvent);
		this->OnAssignCard(assignCardAction);
	}
	else if (newEvent.GetEventType() == CardCreatedEvent::CardCreatedEventType)
	{
		auto cardCreatedEvent = static_cast<CardCreatedEvent&>(newEvent);
		this->OnCardCreated(cardCreatedEvent);
	}
	else if (newEvent.GetEventType() == CardRemovedEvent::CardRemovedEventType)
	{
		auto cardRemovedEvent = static_cast<CardRemovedEvent&>(newEvent);
		this->OnCardRemoved(cardRemovedEvent);
	}
	else if (newEvent.GetEventType() == EndTurnAction::EndTurnActionType)
	{
		auto endTurnAction = static_cast<EndTurnAction&>(newEvent);
		this->OnEndTurn(endTurnAction);
	}
	else if (newEvent.GetEventType() == FightResolvedEvent::FightResolvedEventType)
	{
		auto fightResolvedEvent = static_cast<FightResolvedEvent&>(newEvent);
		this->OnFightResolved(fightResolvedEvent);
	}
	else if (newEvent.GetEventType() == ResolveFightAction::ResolveFightActionType)
	{
		auto resolveFightAction = static_cast<ResolveFightAction&>(newEvent);
		this->OnResolveFight(resolveFightAction);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
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
		auto enemies = std::make_shared<std::list<Entity>>();
		enemies->push_back(assignCardAction.targetCard);
		this->currentAssignments.insert(std::pair<Entity, std::shared_ptr<std::list<Entity>>>(assignCardAction.assignedCard, enemies));

		// Notify listeners.
		auto cardAssignedEvent = std::make_shared<CardAssignedEvent>(assignCardAction.assignedCard, assignCardAction.targetCard);
		this->game->eventManager->QueueEvent(cardAssignedEvent);
	}
}

void AssignmentSystem::OnCardCreated(CardCreatedEvent& cardCreatedEvent)
{
	if (cardCreatedEvent.owner != INVALID_ENTITY_ID)
	{
		this->playerCards.push_back(cardCreatedEvent.serverEntity);
	}
	else
	{
		this->enemyCards.push_back(cardCreatedEvent.serverEntity);
	}
}

void AssignmentSystem::OnCardRemoved(CardRemovedEvent& cardRemovedEvent)
{
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(cardRemovedEvent.serverEntity, OwnerComponent::OwnerComponentType);

	if (ownerComponent->owner != INVALID_ENTITY_ID)
	{
		this->playerCards.remove(cardRemovedEvent.serverEntity);
	}
	else
	{
		this->enemyCards.remove(cardRemovedEvent.serverEntity);
	}
}

void AssignmentSystem::OnEndTurn(EndTurnAction& endTurnAction)
{
	if (this->currentTurnPhase != TurnPhase::Assignment)
	{
		return;
	}

	// Check if enough enemy ships assigned.
	auto reqEnemyAssignments = enemyCards.size() < playerCards.size() ? enemyCards.size() : playerCards.size();

	if (this->currentAssignments.size() < reqEnemyAssignments)
	{
		// Error: Enemy ship not assigned.
		auto errorMessageEvent = std::make_shared<ErrorMessageEvent>("Error_NotAllEnemyShipsAssigned");
		this->game->eventManager->QueueEvent(errorMessageEvent);

		return;
	}

	// Notify listeners.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Fight);
	this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
}

void AssignmentSystem::OnFightResolved(FightResolvedEvent& fightResolvedEvent)
{
	// Remove assignment.
	auto itAssignment = this->currentAssignments.find(fightResolvedEvent.assignedCard);

	if (itAssignment != this->currentAssignments.end())
	{
		this->currentAssignments.erase(itAssignment);
	}

	// Check if all fights resolved.
	if (this->currentAssignments.empty())
	{
		// End fight phase.
		auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Jump);
		this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
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

void AssignmentSystem::OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent)
{
	this->currentTurnPhase = turnPhaseChangedEvent.newTurnPhase;
}