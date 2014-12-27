#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#include "SocketManager.h"
#include "..\MasterServer.h"
#include "Events\LoginSuccessEvent.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer;
using namespace PinnedDownServer::Network;


SocketManager::SocketManager(MasterServer* masterServer)
	: initialized(false),
	result(0)
{
	this->masterServer = masterServer;
}

void SocketManager::InitSocketManager()
{
	printf("Initializing Winsock... ");

	// Initialize Winsock.
	WSADATA wsaData;

	result = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (result != 0)
	{
		printf("Failed to initialize Winsock: %d\n", result);
		return;
	}
	else
	{
		printf("Done.\n");
	}

	// Resolve the local address and port to be used by the server
	printf("Resolving local address and port... ");

	addrinfo hints;
	addrinfo* addressInfo;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;			// IPv4 address family
	hints.ai_socktype = SOCK_STREAM;	// Stream socket.
	hints.ai_protocol = IPPROTO_TCP;	// TCP protocol.
	hints.ai_flags = AI_PASSIVE;		// Returned socket address structure will be used in a call to the bind function.

	result = getaddrinfo(NULL, PINNED_DOWN_SERVER_PORT, &hints, &addressInfo);
	if (result != 0)
	{
		printf("Failed to resolve local address and port: %d\n", result);
		WSACleanup();
		return;
	}
	else
	{
		printf("Done.\n");
	}

	// Create a SOCKET for the server to listen for client connections.
	printf("Creating TCP listening socket... ");

	listenSocket = INVALID_SOCKET;
	listenSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (listenSocket == INVALID_SOCKET)
	{
		printf("Failed to create socket: %ld\n", WSAGetLastError());
		freeaddrinfo(addressInfo);
		WSACleanup();
		return ;
	}
	else
	{
		printf("Done.\n");
	}

	// Setup the TCP listening socket.
	printf("Binding TCP listening socket... ");

    result = bind(listenSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

    if (result == SOCKET_ERROR)
	{
        printf("Failed to bind TCP listening socket: %d\n", WSAGetLastError());
        freeaddrinfo(addressInfo);
        closesocket(listenSocket);
        WSACleanup();
		return;
    }
	else
	{
		printf("Done.\n");
		freeaddrinfo(addressInfo);
	}

	// Listen for maximum number of connections.
	printf("Listening for incoming connections... ");

	result = listen(listenSocket, SOMAXCONN);

	if (result == SOCKET_ERROR)
	{
		printf("Failed to listen for incoming connections: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return;
	}
	else
	{
		printf("Done.\n");
	}

	// Clear client sockets.
	for (int i = 0; i < MAX_CLIENTS; i++) 
	{
		clients[i] = INVALID_SOCKET;
	}

	// Setup event reader and writer.
	serverEventWriter = ServerEventWriter();
	clientActionReader = ClientActionReader();

	this->initialized = true;
}

void SocketManager::Select(int milliseconds)
{
	FD_SET fdSet;

	// Clear socket set and add Accept socket.
	FD_ZERO(&fdSet);
	FD_SET(listenSocket, &fdSet);

	// Add all clients.
	for(int i = 0; i<MAX_CLIENTS; i++)
	{
		if (clients[i] != INVALID_SOCKET)
		{
			FD_SET(clients[i], &fdSet);
		}
	}

	// Check all sockets.
	timeval timeout = timeval();
	timeout.tv_usec = milliseconds * 1000;
	result = select(0, &fdSet, NULL, NULL, &timeout);

	if (result == SOCKET_ERROR) 
	{
		printf("Error selecting sockets: %s\n",WSAGetLastError());
		return;
	}
    
	// Check for new clients.
	if (FD_ISSET(listenSocket, &fdSet))
	{
		// Add new client.
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if (clients[i] == INVALID_SOCKET)
			{
				struct sockaddr_in sockaddr;
				int sockaddrLen = sizeof(sockaddr);

				clients[i] = accept(listenSocket, (struct sockaddr*)&sockaddr, &sockaddrLen);

				// Get client IP address.
				char buffer[16];
				inet_ntop(AF_INET, &sockaddr.sin_addr, buffer, sizeof(buffer));
				printf("Client connected: %s\n", buffer);

				// Send login ACK.
				auto serverEvent = std::make_shared<LoginSuccessEvent>(i);
				this->SendServerEvent(i, *serverEvent);

				if (clients[i] != INVALID_SOCKET)
				{
					// Notify master server.
					this->masterServer->OnClientConnected(i);
				}

				break;
			}
		}
	}

	for (int i = 0;  i < MAX_CLIENTS; i++) 
	{
		// Iterate all connected clients.
		if (clients[i] == INVALID_SOCKET)
		{
			continue; 
		}

		// Check for new data or disconnects.
		if (FD_ISSET(clients[i], &fdSet))
		{
			char recvbuf[4];
			result = recv(clients[i], recvbuf, 4, 0);

			// Check for disconnect.
			if (result <= 0)
			{
				if (result == 0)
				{
					printf("Connection closed by client %d.\n", i);
				}
				else
				{
					printf("Error %d receiving packet from client %d.\n", WSAGetLastError(), i);
				}

				// Close socket.
				this->RemoveClient(i);
			}
			else
			{
				// Get packet size.
				int packetSize = ((int*)recvbuf)[0];

				// Receive packet.
				auto packetBuffer = new char[packetSize];
				result = recv(clients[i], packetBuffer, packetSize, 0);

				// Parse client event.
				std::shared_ptr<Event> clientAction = clientActionReader.ReadClientAction(packetBuffer);
				this->masterServer->OnClientAction(i, clientAction);

				delete packetBuffer;
			}
		}
	}
}

bool SocketManager::IsInitialized()
{
	return this->initialized;
}

void SocketManager::SendServerEvent(int clientId, Event& serverEvent)
{
	result = serverEventWriter.WriteServerEvent(clients[clientId], serverEvent);

	if (result == SOCKET_ERROR)
	{
		printf("Error %d sending packet to client %d.\n", WSAGetLastError(), clientId);
		this->RemoveClient(clientId);
	}
}

void SocketManager::RemoveClient(int clientId)
{
	// Close socket.
	closesocket(clients[clientId]);
	clients[clientId] = INVALID_SOCKET;
	
	// Notify master server.
	this->masterServer->OnClientDisconnected(clientId);
}

SocketManager::~SocketManager()
{
	// Close sockets.
	for (int i = 0; i < MAX_CLIENTS; i++) 
	{
		if (clients[i] != INVALID_SOCKET)
		{
			// Shutdown the send half of the connection since no more data will be sent.
			result = shutdown(clients[i], SD_SEND);

			if (result == SOCKET_ERROR)
			{
				printf("Error %d shutting down client socket %d.\n", WSAGetLastError(), i);
			}
			else
			{
				printf("Client socket %d shut down.", i);
			}

			this->RemoveClient(i);
		}
	}

	// Clean up WinSock.
	WSACleanup();
}