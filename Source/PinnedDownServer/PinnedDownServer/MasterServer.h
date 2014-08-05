#pragma once

#include <map>
#include <memory>

#include "PinnedDownNet.h"

#include "ServerGame.h"
#include "Network\SocketManager.h"

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

		std::shared_ptr<SocketManager> socketManager;
		
		std::map<int, std::shared_ptr<ServerGame>> runningGames;
	};
}