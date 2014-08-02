#pragma once

#include "ServerEventWriter.h"
#include "ClientActionReader.h"

#define PINNED_DOWN_SERVER_PORT "27015"
#define DEFAULT_BUFLEN 512
#define MAX_CLIENTS 32

namespace PinnedDownServer
{
	namespace Network
	{
		class SocketManager
		{
		public:
			~SocketManager();

			void InitSocketManager();
			void Select(int timeout);
			bool IsInitialized();

		private:
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
		};
	}
}