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

	// Accept a single client socket.
	struct sockaddr_in sockaddr;
	int sockaddrLen = sizeof(sockaddr);

	SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&sockaddr, &sockaddrLen);

	if (clientSocket == INVALID_SOCKET)
	{
		printf("Failed to accept client socket: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();

		pause();
		return 1;
	}
	else
	{
		printf("Client connected: %s\n", inet_ntoa(sockaddr.sin_addr));
	}

	// Send login ACK.
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];

	ServerEvent packet = ServerEvent(ServerEventType::LoginSuccess);
	memcpy(&sendbuf, &packet.eventType, sizeof(ServerEventType));

	result = send(clientSocket, sendbuf, sizeof(ServerEventType), 0);

	if (result == SOCKET_ERROR)
	{
		printf("Failed to send packet: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();

		pause();
		return 1;
	}
	else
	{
		printf("Packet sent.\n");
	}

	// Create new game.
	PinnedDownCore::Game* game = new PinnedDownCore::Game();

	// Receive until the peer shuts down the connection.
	do
	{
		result = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);

		if (result > 0)
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
		else if (result == 0)
		{
			printf("Connection closing...\n");
		}
		else
		{
			printf("Failed to receive packet: %d\n", WSAGetLastError());
			closesocket(clientSocket);
			WSACleanup();

			pause();
			return 1;
		}
	}
	while (result > 0);

	// Shutdown the send half of the connection since no more data will be sent.
	result = shutdown(clientSocket, SD_SEND);

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
	}

	pause();
	return 0;
}
