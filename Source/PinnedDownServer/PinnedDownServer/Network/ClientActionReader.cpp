#include <iostream>

#include "ClientActionReader.h"

#include "Actions\EndTurnAction.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Network;

std::shared_ptr<Event> ClientActionReader::ReadClientAction(char* buffer)
{
	std::istrstream	in(buffer);

	char eventType[256];
	in >> eventType;

	HashedString hashedEventType = HashedString(eventType);

	if (hashedEventType == EndTurnAction::EndTurnActionType)
	{
		auto endTurnAction = std::make_shared<EndTurnAction>();
		endTurnAction->Deserialize(in);
		return endTurnAction;
	}

	return nullptr;
}