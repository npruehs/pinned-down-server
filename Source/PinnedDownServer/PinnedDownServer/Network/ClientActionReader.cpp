#include <iostream>

#include "ClientActionReader.h"

using namespace PinnedDownServer::Network;

ClientAction ClientActionReader::ReadClientAction(char* buffer)
{
	ClientAction action = ClientAction();
	memcpy(&action.actionType, buffer, sizeof(ClientActionType));

	switch (action.actionType)
	{
	case ClientActionType::SelectCard:
		printf("Select card.\n");
	}

	return action;
}