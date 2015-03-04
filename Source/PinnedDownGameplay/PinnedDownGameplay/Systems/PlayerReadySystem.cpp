#include "Event.h"
#include "PlayerReadySystem.h"

#include "..\Events\AllPlayersReadyEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;


PlayerReadySystem::PlayerReadySystem()
{
}

void PlayerReadySystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EndTurnAction::EndTurnActionType);
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->AddListener(this, PlayerReadyStateResetEvent::PlayerReadyStateResetEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void PlayerReadySystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EndTurnAction);
	CALL_EVENT_HANDLER(PlayerAddedEvent);
	CALL_EVENT_HANDLER(PlayerReadyStateResetEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(PlayerReadySystem, EndTurnAction)
{
	this->playerReady[data.sender] = true;

	// Check if all players are ready.
	for (auto it = this->playerReady.begin(); it != this->playerReady.end(); ++it)
	{
		if (!it->second)
		{
			return;
		}
	}

	// Notify listeners.
	auto allPlayersReadyEvent = std::make_shared<AllPlayersReadyEvent>();
	this->game->eventManager->QueueEvent(allPlayersReadyEvent);
}

EVENT_HANDLER_DEFINITION(PlayerReadySystem, PlayerAddedEvent)
{
	this->playerReady[data.clientId] = false;
}

EVENT_HANDLER_DEFINITION(PlayerReadySystem, PlayerReadyStateResetEvent)
{
	this->ResetReadyState();
}

EVENT_HANDLER_DEFINITION(PlayerReadySystem, TurnPhaseChangedEvent)
{
	this->ResetReadyState();

	// Notify listeners.
	auto playerReadyStateResetEvent = std::make_shared<PlayerReadyStateResetEvent>();
	this->game->eventManager->QueueEvent(playerReadyStateResetEvent);
}

void PlayerReadySystem::ResetReadyState()
{
	for (auto it = this->playerReady.begin(); it != this->playerReady.end(); ++it)
	{
		it->second = false;
	}
}