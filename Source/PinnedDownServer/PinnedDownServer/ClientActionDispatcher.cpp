#include "ClientActionDispatcher.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;

ClientActionDispatcher::ClientActionDispatcher(std::shared_ptr<Game> game)
{
	this->game = game;
}

void ClientActionDispatcher::DispatchClientAction(ClientAction action)
{
}