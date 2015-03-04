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
using namespace PinnedDownGameplay::Systems;


AssignmentSystem::AssignmentSystem()
{
}

void AssignmentSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->clientToPlayerEntityIdMap = std::make_shared<BidirectionalMap<int, Entity>>();

	this->game->eventManager->AddListener(this, AssignCardAction::AssignCardActionType);	
	this->game->eventManager->AddListener(this, EndTurnAction::EndTurnActionType);
	this->game->eventManager->AddListener(this, EnemyCardPlayedEvent::EnemyCardPlayedEventType);
	this->game->eventManager->AddListener(this, EntityRemovedEvent::EntityRemovedEventType);
	this->game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
	this->game->eventManager->AddListener(this, FlagshipPlayedEvent::FlagshipPlayedEventType);
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->AddListener(this, ResolveFightAction::ResolveFightActionType);
	this->game->eventManager->AddListener(this, StarshipPlayedEvent::StarshipPlayedEventType);	
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void AssignmentSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(AssignCardAction);
	CALL_EVENT_HANDLER(EndTurnAction);
	CALL_EVENT_HANDLER(EnemyCardPlayedEvent);
	CALL_EVENT_HANDLER(EntityRemovedEvent);
	CALL_EVENT_HANDLER(FightResolvedEvent);
	CALL_EVENT_HANDLER(FlagshipPlayedEvent);
	CALL_EVENT_HANDLER(PlayerAddedEvent);
	CALL_EVENT_HANDLER(ResolveFightAction);
	CALL_EVENT_HANDLER(StarshipPlayedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, AssignCardAction)
{
	if (this->currentTurnPhase != TurnPhase::Assignment)
	{
		return;
	}

	if (data.sender == INVALID_SENDER_ID)
	{
		return;
	}

	auto playerEntity = this->clientToPlayerEntityIdMap->GetValueOrDefault(data.sender, INVALID_ENTITY_ID);

	if (playerEntity == INVALID_ENTITY_ID)
	{
		return;
	}

	// Check owners.
	auto assignedCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(data.assignedCard, OwnerComponent::OwnerComponentType);
	auto targetCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(data.targetCard, OwnerComponent::OwnerComponentType);

	if (assignedCardOwner != nullptr && assignedCardOwner->owner == playerEntity && targetCardOwner != nullptr && targetCardOwner->owner == INVALID_ENTITY_ID)
	{
		// Check for previous assignment of source card.
		auto assignment = this->currentAssignments.find(data.assignedCard);

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
			auto targetCard = std::find(targetCards->begin(), targetCards->end(), data.targetCard);

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
		enemies->push_back(data.targetCard);
		this->currentAssignments.insert(std::pair<Entity, std::shared_ptr<std::list<Entity>>>(data.assignedCard, enemies));

		// Notify listeners.
		auto cardAssignedEvent = std::make_shared<CardAssignedEvent>(data.assignedCard, data.targetCard);
		this->game->eventManager->QueueEvent(cardAssignedEvent);
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, EndTurnAction)
{
	if (this->currentTurnPhase == TurnPhase::Assignment)
	{
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
	else if (this->currentTurnPhase == TurnPhase::Fight)
	{
		// Fight phase is automatically ended in OnFightResolved.
		auto errorMessageEvent = std::make_shared<ErrorMessageEvent>("Error_NotAllFightsResolved");
		this->game->eventManager->QueueEvent(errorMessageEvent);
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, EnemyCardPlayedEvent)
{
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(data.cardEntity, CardComponent::CardComponentType);

	if (cardComponent->cardType == CardType::Starship)
	{
		this->enemyCards.push_back(data.cardEntity);
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, FightResolvedEvent)
{
	// Remove assignment.
	auto itAssignment = this->currentAssignments.find(data.assignedCard);

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

EVENT_HANDLER_DEFINITION(AssignmentSystem, FlagshipPlayedEvent)
{
	this->playerCards.push_back(data.shipEntity);
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, PlayerAddedEvent)
{
	this->clientToPlayerEntityIdMap->Add(data.clientId, data.serverEntity);
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, ResolveFightAction)
{
	// Check owners.
	if (data.sender == INVALID_SENDER_ID)
	{
		return;
	}

	auto playerEntity = this->clientToPlayerEntityIdMap->GetValueOrDefault(data.sender, INVALID_ENTITY_ID);

	if (playerEntity == INVALID_ENTITY_ID)
	{
		return;
	}

	// Get assignment.
	auto assignedCardOwner = this->game->entityManager->GetComponent<OwnerComponent>(data.assignedCard, OwnerComponent::OwnerComponentType);

	if (assignedCardOwner != nullptr && assignedCardOwner->owner == playerEntity)
	{
		auto assignment = this->currentAssignments.find(data.assignedCard);

		if (assignment != this->currentAssignments.end())
		{
			// Start fight.
			auto fightStartedEvent = std::make_shared<FightStartedEvent>(assignment->first, assignment->second);
			this->game->eventManager->QueueEvent(fightStartedEvent);
		}
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, StarshipPlayedEvent)
{
	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(data.shipEntity, OwnerComponent::OwnerComponentType);

	if (ownerComponent->owner != INVALID_ENTITY_ID)
	{
		this->playerCards.push_back(data.shipEntity);
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, EntityRemovedEvent)
{
	auto cardComponent = this->game->entityManager->GetComponent<CardComponent>(data.entity, CardComponent::CardComponentType);

	if (cardComponent == nullptr || cardComponent->cardType != CardType::Starship)
	{
		return;
	}

	auto ownerComponent = this->game->entityManager->GetComponent<OwnerComponent>(data.entity, OwnerComponent::OwnerComponentType);

	if (ownerComponent->owner != INVALID_ENTITY_ID)
	{
		this->playerCards.remove(data.entity);
	}
	else
	{
		this->enemyCards.remove(data.entity);
	}
}

EVENT_HANDLER_DEFINITION(AssignmentSystem, TurnPhaseChangedEvent)
{
	this->currentTurnPhase = data.newTurnPhase;

	if (this->currentTurnPhase == TurnPhase::Assignment)
	{
		// Check if any enemies present.
		if (this->enemyCards.empty())
		{
			// End fight phase.
			auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Jump);
			this->game->eventManager->QueueEvent(turnPhaseChangedEvent);
		}
	}
}
