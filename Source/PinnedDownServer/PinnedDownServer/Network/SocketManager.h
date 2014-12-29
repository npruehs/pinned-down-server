#pragma once

#include "ServerEventWriter.h"
#include "ClientActionReader.h"

#include "..\Diagnotics\ServerLogger.h"

#define DEFAULT_BUFLEN 512
#define MAX_CLIENTS 32


using namespace PinnedDownServer::Diagnostics;


namespace PinnedDownServer
{
	class MasterServer;

	namespace Network
	{
		class SocketManager
		{
		public:
			SocketManager(MasterServer* masterServer, std::shared_ptr<ServerLogger> logger);
			~SocketManager();

			void InitSocketManager();
			void Select(int timeout);
			bool IsInitialized();

			void SendServerEvent(int clientId, Event& serverEvent);

			void RemoveClient(int clientId);

		private:
			// Server to notify of client events.
			MasterServer* masterServer;

			// Whether WinSock has been successfully initialized, or not.
			bool initialized;

			// Result of the last WinSock API call.
			int result;

			// Socket to listen for new connections.
			SOCKET listenSocket;

			// Connected clients.
			SOCKET clients[MAX_CLIENTS];

			ServerEventWriter serverEventWriter;
			ClientActionReader clientActionReader;

			std::shared_ptr<ServerLogger> logger;
		};
	}
}