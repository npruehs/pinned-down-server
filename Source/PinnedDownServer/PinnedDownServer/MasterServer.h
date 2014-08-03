#pragma once

#include <memory>

#include "PinnedDownNet.h"

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
		void OnClientAction(int clientId, ClientAction clientAction);

	private:
		std::shared_ptr<SocketManager> socketManager;

	};
}