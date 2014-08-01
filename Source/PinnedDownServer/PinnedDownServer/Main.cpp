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

int main()
{
	WSADATA wsaData;

	int result;

	// Initialize Winsock.
	result = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (result != 0)
	{
		printf("WSAStartup failed: %d\n", result);

		int i;
		cin >> i;
		return 1;
	}
	else
	{
		printf("Winsock initialized.\n");
	}

	addrinfo hints;
	addrinfo* addressInfo;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;			// IPv4 address family
	hints.ai_socktype = SOCK_STREAM;	// Stream socket.
	hints.ai_protocol = IPPROTO_TCP;	// TCP protocol.
	hints.ai_flags = AI_PASSIVE;		// Returned socket address structure will be used in a call to the bind function.

	// Resolve the local address and port to be used by the server
	result = getaddrinfo(NULL, PINNED_DOWN_SERVER_PORT, &hints, &addressInfo);
	if (result != 0)
	{
		printf("getaddrinfo failed: %d\n", result);
		WSACleanup();

		int i;
		cin >> i;
		return 1;
	}
	else
	{
		printf("Local address resolved.\n");
	}

	// Create a SOCKET for the server to listen for client connections.
	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(addressInfo);
		WSACleanup();

		int i;
		cin >> i;
		return 1;
	}

	// Setup the TCP listening socket.
    result = bind( listenSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

    if (result == SOCKET_ERROR)
	{
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(addressInfo);
        closesocket(listenSocket);
        WSACleanup();

		int i;
		cin >> i;
		return 1;
    }
	else
	{
		printf("TCP socket created.\n");
		freeaddrinfo(addressInfo);
	}

	// Listen for maximum number of connections.
	result = listen(listenSocket, SOMAXCONN);

	if (result == SOCKET_ERROR)
	{
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();

		int i;
		cin >> i;
		return 1;
	}
	else
	{
		printf("Listening for %d connections.\n", SOMAXCONN);
	}

	// Accept a single client socket.
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);

	if (clientSocket == INVALID_SOCKET)
	{
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();

		int i;
		cin >> i;
		return 1;
	}

	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int sendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Send three packets.
	for (int i = 0; i < 3; i++)
	{
		ServerEvent packet = ServerEvent();
		packet.eventType = ServerEventType::LoginSuccess;
		memcpy(&sendbuf, &packet.eventType, sizeof(ServerEventType));

		sendResult = send(clientSocket, sendbuf, sizeof(unsigned int), 0);

		if (sendResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(clientSocket);
			WSACleanup();

			int i;
			cin >> i;
			return 1;
		}
	}

	// Create new game.
	PinnedDownCore::Game* game = new PinnedDownCore::Game();

	// Receive until the peer shuts down the connection.
	do
	{
		result = recv(clientSocket, recvbuf, recvbuflen, 0);

		if (result > 0)
		{
			ClientEvent packet = ClientEvent();
			memcpy(&packet.eventType, &recvbuf, sizeof(ClientEventType));

			switch (packet.eventType)
			{
			case ClientEventType::CardSelected:
				printf("Card selected.\n");
			}
		}
		else if (result == 0)
		{
			printf("Connection closing...\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(clientSocket);
			WSACleanup();

			int i;
			cin >> i;
			return 1;
		}
	}
	while (result > 0);

	// Shutdown the send half of the connection since no more data will be sent.
	result = shutdown(clientSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();

		int i;
		cin >> i;
		return 1;
	}
	else
	{
		printf("Client socket shut down.");
		closesocket(clientSocket);
		WSACleanup();
	}

	int i;
	cin >> i;
	return 0;
}
