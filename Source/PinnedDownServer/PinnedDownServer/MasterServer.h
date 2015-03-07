#pragma once

#include <map>
#include <memory>
#include <string>

#include "PinnedDownNet.h"

#include "PinnedDownClientData.h"
#include "ServerLobby.h"

#include "Diagnotics\ServerLogger.h"

#include "Network\SocketManager.h"


using namespace PinnedDownServer::Diagnostics;
using namespace PinnedDownServer::Network;

namespace PinnedDownServer
{
	class MasterServer
	{
	public:
		MasterServer();

		void Start();

		void OnClientConnected(int clientId);
		void OnClientDisconnected(int clientId);
		void OnClientWantsToJoinGame(PinnedDownClientData* client);

		void OnClientAction(int clientId, std::shared_ptr<Event> clientAction);
		void OnServerEvent(int clientId, Event& serverEvent);

	private:
		static bool running;
		static BOOL WINAPI OnConsoleCtrlSignal(DWORD signal);

		int nextGameId;

		std::shared_ptr<ServerLogger> logger;
		std::shared_ptr<SocketManager> socketManager;

		std::map<int, PinnedDownClientData*> connectedClients;

		std::shared_ptr<ServerLobby> lobby;
		std::list<std::shared_ptr<ServerGame>> runningGames;

		void AddClientToGame(PinnedDownClientData* client);
		void RemoveClientFromGame(PinnedDownClientData* client);
		void ShutDownGame(PinnedDownClientData* client);
	};
}