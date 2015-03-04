#include <iostream>

#include "ClientActionReader.h"

#include "Actions\AssignCardAction.h"
#include "Actions\DisconnectClientAction.h"
#include "Actions\PlayCardAction.h"
#include "Actions\PlayerReadyAction.h"
#include "Actions\ResolveFightAction.h"
#include "Actions\VerifyClientVersionAction.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Network;

std::shared_ptr<Event> ClientActionReader::ReadClientAction(char* buffer)
{
	std::istrstream	in(buffer);

	char eventType[256];
	in >> eventType;

	HashedString hashedEventType = HashedString(eventType);

	if (hashedEventType == AssignCardAction::AssignCardActionType)
	{
		auto assignCardAction = std::make_shared<AssignCardAction>();
		assignCardAction->Deserialize(in);
		return assignCardAction;
	}
	else if (hashedEventType == DisconnectClientAction::DisconnectClientActionType)
	{
		auto disconnectClientAction = std::make_shared<DisconnectClientAction>();
		disconnectClientAction->Deserialize(in);
		return disconnectClientAction;
	}
	else if (hashedEventType == PlayCardAction::PlayCardActionType)
	{
		auto playCardAction = std::make_shared<PlayCardAction>();
		playCardAction->Deserialize(in);
		return playCardAction;
	}
	else if (hashedEventType == PlayerReadyAction::PlayerReadyActionType)
	{
		auto playerReadyAction = std::make_shared<PlayerReadyAction>();
		playerReadyAction->Deserialize(in);
		return playerReadyAction;
	}
	else if (hashedEventType == ResolveFightAction::ResolveFightActionType)
	{
		auto resolveFightAction = std::make_shared<ResolveFightAction>();
		resolveFightAction->Deserialize(in);
		return resolveFightAction;
	}
	else if (hashedEventType == VerifyClientVersionAction::VerifyClientVersionActionType)
	{
		auto verifyClientVersionAction = std::make_shared<VerifyClientVersionAction>();
		verifyClientVersionAction->Deserialize(in);
		return verifyClientVersionAction;
	}

	return nullptr;
}