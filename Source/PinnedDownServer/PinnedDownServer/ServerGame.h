#pragma once

#include <memory>

#include "Game.h"
#include "ServerEventDispatcher.h"

#include "Analytics\ServerAnalytics.h"

#include "Network\HTTPClient.h"


using namespace PinnedDownCore;
using namespace PinnedDownServer;
using namespace PinnedDownServer::Analytics;
using namespace PinnedDownServer::Network;


namespace PinnedDownServer
{
	class MasterServer;
	class PinnedDownClientData;

	class ServerGame
    {
    public:
		ServerGame(MasterServer* masterServer, std::shared_ptr<HTTPClient> httpClient, std::shared_ptr<ServerLogger> logger);

		void AddClient(PinnedDownClientData* client);
		int GetClientCount();
		std::shared_ptr<Game> GetGame();
		void OnClientAction(std::shared_ptr<Event> clientAction);
		void OnServerEvent(Event& serverEvent);

    private:
		MasterServer* masterServer;

		std::list<PinnedDownClientData*> clients;

		std::shared_ptr<Game> game;
		std::shared_ptr<ServerEventDispatcher> serverEventDispatcher;
		std::shared_ptr<HTTPClient> httpClient;
		std::shared_ptr<ServerLogger> logger;

		void Update();
    };
}