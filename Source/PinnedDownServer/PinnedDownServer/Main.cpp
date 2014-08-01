#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include "Game.h"
#include "PinnedDownNet.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define PINNED_DOWN_SERVER_PORT "27015"
#define DEFAULT_BUFLEN 512
#define MAX_CLIENTS 32

void pause()
{
	int i;
	cin >> i;
}

int main()
{
	// Initialize Winsock.
	WSADATA wsaData;
	int result;

	result = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (result != 0)
	{
		printf("Failed to initialize Winsock: %d\n", result);

		pause();
		return 1;
	}
	else
	{
		printf("Winsock initialized.\n");
	}

	// Resolve the local address and port to be used by the server
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

		pause();
		return 1;
	}
	else
	{
		printf("Local address and port resolved.\n");
	}

	// Create a SOCKET for the server to listen for client connections.
	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (listenSocket == INVALID_SOCKET)
	{
		printf("Failed to create socket: %ld\n", WSAGetLastError());
		freeaddrinfo(addressInfo);
		WSACleanup();

		pause();
		return 1;
	}
	else
	{
		printf("Socket created.\n");
	}

	// Setup the TCP listening socket.
    result = bind(listenSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

    if (result == SOCKET_ERROR)
	{
        printf("Failed to bind TCP listening socket: %d\n", WSAGetLastError());
        freeaddrinfo(addressInfo);
        closesocket(listenSocket);
        WSACleanup();

		pause();
		return 1;
    }
	else
	{
		printf("TCP socket bound.\n");
		freeaddrinfo(addressInfo);
	}

	// Listen for maximum number of connections.
	result = listen(listenSocket, SOMAXCONN);

	if (result == SOCKET_ERROR)
	{
		printf("Failed to listen for incoming connections: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();

		pause();
		return 1;
	}
	else
	{
		printf("Listening for %d connections.\n", SOMAXCONN);
	}

	// Accept client sockets.
	FD_SET fdSet;
	SOCKET clients[MAX_CLIENTS];

	for(int i = 0; i < MAX_CLIENTS; i++) 
	{
		clients[i] = INVALID_SOCKET;
	}

	while (true)
	{
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

		result = select(0, &fdSet, NULL, NULL, NULL);

		if (result == SOCKET_ERROR) 
		{
			printf("Error selecting sockets: %s\n",WSAGetLastError());

			pause();
			return 1;
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
					printf("Client connected: %s\n", inet_ntoa(sockaddr.sin_addr));

					// Send login ACK.
					char sendbuf[DEFAULT_BUFLEN];
					ServerEvent packet = ServerEvent(ServerEventType::LoginSuccess);
					memcpy(&sendbuf, &packet.eventType, sizeof(ServerEventType));

					result = send(clients[i], sendbuf, sizeof(ServerEventType), 0);

					if (result == SOCKET_ERROR)
					{
						printf("Failed to send packet: %d\n", WSAGetLastError());
						closesocket(clients[i]);
						WSACleanup();

						pause();
						return 1;
					}
					else
					{
						printf("Packet sent.\n");
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
				char recvbuf[DEFAULT_BUFLEN];
				result = recv(clients[i], recvbuf, DEFAULT_BUFLEN, 0);

				// Check for disconnect.
				if (result <= 0)
				{
					// Close socket.
					closesocket(clients[i]);       
					clients[i] = INVALID_SOCKET;

					if (result == 0)
					{
						printf("Connection closed by client %d. \n", i);
					}
					else
					{
						printf("Failed to receive packet: %d\n", WSAGetLastError());
					}
				}
				else
				{
					// Process client action.
					ClientAction action = ClientAction();
					memcpy(&action.actionType, &recvbuf, sizeof(ClientActionType));

					switch (action.actionType)
					{
					case ClientActionType::SelectCard:
						printf("Select card.\n");
					}
				}
			}
		}
	}

	// Shutdown the send half of the connection since no more data will be sent.
	/*result = shutdown(clientSocket, SD_SEND);

	if (result == SOCKET_ERROR)
	{
		printf("Failed to shut down socket: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();

		pause();
		return 1;
	}
	else
	{
		printf("Client socket shut down.");
		closesocket(clientSocket);
		WSACleanup();
	}*/

	pause();
	return 0;
}
