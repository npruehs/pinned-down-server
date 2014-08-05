#pragma once

#include <memory>

#include "Game.h"
#include "ServerEventDispatcher.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	class MasterServer;

	class ServerGame
    {
    public:
		ServerGame(MasterServer* masterServer, int clientId);

		void OnClientAction(std::shared_ptr<Event> clientAction);
		void OnServerEvent(Event& serverEvent);

		std::shared_ptr<Game> GetGame() { return std::shared_ptr<Game>(this->game); }

    private:
		MasterServer* masterServer;

		int clientId;

		std::shared_ptr<Game> game;
		std::shared_ptr<ServerEventDispatcher> serverEventDispatcher;

		void Update();
    };
}