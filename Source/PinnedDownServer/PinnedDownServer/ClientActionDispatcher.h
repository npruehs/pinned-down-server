#pragma once

#include <memory>

#include "Game.h"
#include "PinnedDownNet.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	// Translates client actions into game events.
	class ClientActionDispatcher
	{
	public:
		ClientActionDispatcher(std::shared_ptr<Game> game);

		void DispatchClientAction(ClientAction action);

	private:
		std::shared_ptr<Game> game;
	};
}