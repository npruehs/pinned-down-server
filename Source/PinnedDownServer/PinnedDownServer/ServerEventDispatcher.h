#pragma once

#include <memory>

#include "Game.h"
#include "PinnedDownNet.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	class ServerGame;

	// Translates game events into server events.
	class ServerEventDispatcher : public IEventListener
	{
	public:
		ServerEventDispatcher(ServerGame* serverGame, std::shared_ptr<Game> game);

	private:
		ServerGame* serverGame;
		std::shared_ptr<Game> game;

		void OnEvent(Event & event);
		void DispatchServerEvent(ServerEvent event);
	};
}