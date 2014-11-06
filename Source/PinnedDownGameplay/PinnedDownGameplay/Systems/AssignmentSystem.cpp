#include <algorithm>

#include "Event.h"
#include "AssignmentSystem.h"

#include "Components\CardComponent.h"
#include "Components\OwnerComponent.h"

#include "Events\CardAssignedEvent.h"
#include "Events\CardUnassignedEvent.h"
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
	this->game->eventManager->AddListener(this, EndTurnAction::EndTurnActionType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
	this->game->eventManager->AddListener(this, ResolveFightAction::ResolveFightActionType);
	this->game->eventManager->AddListener(this, ShipDefeatedEvent::ShipDefeatedEventType);
	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AssignCardAction::AssignCardActionType)
	{
		auto assignCardAction = static_cast<AssignCardAction&>(newEvent);
		this->OnAssignCard(assignCardAction);
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
	else if (newEvent.GetEventType() == ShipDefeatedEvent::ShipDefeatedEventType)
	{
		auto shipDefeatedEvent = static_cast<ShipDefeatedEvent&>(newEvent);
		this->OnShipDefeated(shipDefeatedEvent);
	}
	else if (newEvent.GetEventType() == StarshipPlayedEvent::StarshipPlayedEventType)
	{
		auto starshipPlayedEvent = static_cast<StarshipPlayedEvent&>(newEvent);
		this->OnStarshipPlayed(starshipPlayedEvent);
	}
	else if (newEvent.GetEventType() == TurnPhaseChangedEvent::TurnPhaseChangedEventType)
	{
		auto turnPhaseChangedEvent = static_cast<TurnPhaseChangedEvent&>(newEvent);
		this->OnTurnPhaseChanged(turnPhaseChangedEvent);
	}
}

void AssignmentSystem::OnAssignCard(AssignCardAction& assignCardAction)
{
	if (this->currentTurnPhase != TurnPhase::Assignment)
	{
		return;
	}

	// Check owners.
	auto assignedCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(assignCardAction.assignedCard, OwnerComponent::OwnerComponentType);
	auto targetCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(assignCardAction.targetCard, OwnerComponent::OwnerComponentType);

	if (assignedCardOwner != nullptr && assignedCardOwner->owner != INVALID_ENTITY_ID && targetCardOwner != nullptr && targetCardOwner->owner == INVALID_ENTITY_ID)
	{
		// Check for previous assignment of source card.
		auto assignment = this->currentAssignments.find(assignCardAction.assignedCard);

		if (assignment != this->currentAssignments.end())
		{
			// Notify listeners.
			auto cardUnassignedEvent = std::make_shared<CardUnassignedEvent>(assignment->first);
			this->game->eventManager->QueueEvent(cardUnassignedEvent);

			this->currentAssignments.erase(assignment);
		}

		// Check for previous assignment of target card.
		auto foundCard = this->currentAssignments.end();

		for (auto it = this->currentAssignments.begin(); it != this->currentAssignments.end(); ++it)
		{
			auto targetCards = it->second;
			auto targetCard = std::find(targetCards->begin(), targetCards->end(), assignCardAction.targetCard);

			if (targetCard != targetCards->end())
			{
				foundCard = it;
			}
		}

		if (foundCard != this->currentAssignments.end())
		{
			// Notify listeners.
			auto cardUnassignedEvent = std::make_shared<CardUnassignedEvent>(foundCard->first);
			this->game->eventManager->QueueEvent(cardUnassignedEvent);

			this->currentAssignments.erase(foundCard);
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

void AssignmentSystem::OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent)
{
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(starshipPlayedEvent.shipEntity, OwnerComponent::OwnerComponentType);

	if (ownerComponent->owner != INVALID_ENTITY_ID)
	{
		this->playerCards.push_back(starshipPlayedEvent.shipEntity);
	}
	else
	{
		this->enemyCards.push_back(starshipPlayedEvent.shipEntity);
	}
}

void AssignmentSystem::OnShipDefeated(ShipDefeatedEvent& shipDefeatedEvent)
{
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(shipDefeatedEvent.shipEntity, OwnerComponent::OwnerComponentType);

	if (ownerComponent->owner != INVALID_ENTITY_ID)
	{
		this->playerCards.remove(shipDefeatedEvent.shipEntity);
	}
	else
	{
		this->enemyCards.remove(shipDefeatedEvent.shipEntity);
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