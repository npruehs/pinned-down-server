#pragma once

#include <map>
#include <memory>

#include "PinnedDownNet.h"
#include "PinnedDownClientData.h"

#include "Actions\JoinGameAction.h"
#include "Actions\VerifyClientVersionAction.h"

#include "Diagnotics\ServerLogger.h"


using namespace PinnedDownServer::Diagnostics;
using namespace PinnedDownServer::Network;


#define CLIENTS_PER_GAME 1

#define REQUIRED_CLIENT_VERSION_MAJOR 0
#define REQUIRED_CLIENT_VERSION_MINOR 1
#define REQUIRED_CLIENT_VERSION_BUILD 0
#define REQUIRED_CLIENT_VERSION_REVISION 6


namespace PinnedDownServer
{
	class MasterServer;
	class PinnedDownClientData;

	class ServerLobby
	{
	public:
		ServerLobby(MasterServer* masterServer, std::shared_ptr<ServerLogger> logger);

		void AddClient(PinnedDownClientData* client);
		void RemoveClient(PinnedDownClientData* client);

		void OnClientAction(PinnedDownClientData* client, std::shared_ptr<Event> clientAction);

		void OnJoinGame(PinnedDownClientData* client, std::shared_ptr<JoinGameAction> joinGameAction);
		void OnVerifyClientVersion(PinnedDownClientData* client, std::shared_ptr<VerifyClientVersionAction> verifyClientVersionAction);
		
	private:
		MasterServer* masterServer;
		std::shared_ptr<ServerLogger> logger;

		std::list<PinnedDownClientData*> clients;
	};
}