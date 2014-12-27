#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#include "HTTPClient.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace PinnedDownServer::Network;


HTTPClient::HTTPClient()
{
}

HTTPClient::~HTTPClient()
{
}

void HTTPClient::SendRequest()
{
	// Resolve GameAnalytics URL.
	printf("Resolving GameAnalytics address and port... ");

	addrinfo hints;
	addrinfo* addressInfo;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	auto result = getaddrinfo("www.google.com", "80", &hints, &addressInfo);
	if (result != 0)
	{
		printf("Error: %d\n", result);
		return;
	}
	else
	{
		printf("Success.\n");
	}

	// Setup TCP socket.
	printf("Setting up GameAnalytics socket... ");

	SOCKET gameAnalyticsSocket = INVALID_SOCKET;
	gameAnalyticsSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (gameAnalyticsSocket == INVALID_SOCKET)
	{
		printf("Error: %ld\n", WSAGetLastError());
		freeaddrinfo(addressInfo);
		return;
	}
	else
	{
		printf("Success.\n");
	}

	// Connect to GameAnalytics server.
	printf("Connecting to GameAnalytics server... ");

	result = connect(gameAnalyticsSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("Error: %ld\n", WSAGetLastError());
		freeaddrinfo(addressInfo);
		closesocket(gameAnalyticsSocket);
		gameAnalyticsSocket = INVALID_SOCKET;
	}
	else
	{
		freeaddrinfo(addressInfo);
		printf("Success.\n");
	}

	// Send GameAnalytics message.
	printf("Sending GameAnalytics event... ");

	char *sendbuf = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";

	result = send(gameAnalyticsSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (result == SOCKET_ERROR)
	{
		printf("Error: %d\n", WSAGetLastError());
		closesocket(gameAnalyticsSocket);
		return;
	}
	else
	{
		printf("Bytes sent: %ld\n", result);
	}

	// Receive data until the server closes the connection
	printf("Waiting for GameAnalytics server answer... ");

	int recvbuflen = 2048;
	char recvbuf[2048];

	do
	{
		result = recv(gameAnalyticsSocket, recvbuf, recvbuflen, 0);

		if (result > 0)
		{
			printf("Bytes received: %d\n", result);
			printf(recvbuf);
			printf("\n");
		}
		else if (result == 0)
		{
			printf("Connection to GameAnalytics server closed.\n");
		}
		else
		{
			printf("Error: %d\n", WSAGetLastError());
		}
	} while (result > 0);

	// Shutdown the send half of the connection since no more data will be sent.
	printf("Shutting down GameAnalytics socket... ");

	result = shutdown(gameAnalyticsSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("Error: %d\n", WSAGetLastError());
		closesocket(gameAnalyticsSocket);
		return;
	}
	else
	{
		printf("Success.\n");
	}

	closesocket(gameAnalyticsSocket);
}
