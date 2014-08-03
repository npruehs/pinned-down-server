#pragma once

#include <memory>

#include "Game.h"
#include "ClientActionDispatcher.h"
#include "ServerEventDispatcher.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	class MasterServer;

	class ServerGame
    {
    public:
		ServerGame(MasterServer* masterServer, int clientId);

		void OnClientAction(ClientAction clientAction);
		void OnServerEvent(ServerEvent serverEvent);

		std::shared_ptr<Game> GetGame() { return std::shared_ptr<Game>(this->game); }

    private:
		MasterServer* masterServer;

		int clientId;

		std::shared_ptr<Game> game;
		std::shared_ptr<ClientActionDispatcher> clientActionDispatcher;
		std::shared_ptr<ServerEventDispatcher> serverEventDispatcher;

		void Update();
    };
}