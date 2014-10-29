#include <iostream>

#include "ClientActionReader.h"

#include "Actions\AssignCardAction.h"
#include "Actions\EndTurnAction.h"
#include "Actions\PlayCardAction.h"
#include "Actions\ResolveFightAction.h"

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
	else if (hashedEventType == EndTurnAction::EndTurnActionType)
	{
		auto endTurnAction = std::make_shared<EndTurnAction>();
		endTurnAction->Deserialize(in);
		return endTurnAction;
	}
	else if (hashedEventType == PlayCardAction::PlayCardActionType)
	{
		auto playCardAction = std::make_shared<PlayCardAction>();
		playCardAction->Deserialize(in);
		return playCardAction;
	}
	else if (hashedEventType == ResolveFightAction::ResolveFightActionType)
	{
		auto resolveFightAction = std::make_shared<ResolveFightAction>();
		resolveFightAction->Deserialize(in);
		return resolveFightAction;
	}

	return nullptr;
}