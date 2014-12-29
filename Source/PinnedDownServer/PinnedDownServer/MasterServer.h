#pragma once

#include <map>
#include <memory>
#include <string>

#include "PinnedDownNet.h"

#include "PinnedDownClientData.h"

#include "Diagnotics\ServerLogger.h"

#include "Network\HTTPClient.h"
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
		void OnClientAction(int clientId, std::shared_ptr<Event> clientAction);
		void OnServerEvent(int clientId, Event& serverEvent);

	private:
		static bool running;
		static BOOL WINAPI OnConsoleCtrlSignal(DWORD signal);

		std::shared_ptr<ServerLogger> logger;
		std::shared_ptr<SocketManager> socketManager;
		std::shared_ptr<HTTPClient> httpClient;

		std::map<int, PinnedDownClientData> connectedClients;

		void SendGameAnalyticsEvent(PinnedDownClientData client, std::string eventId);
	};
}