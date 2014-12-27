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

std::string HTTPClient::SendRequest(std::string host, std::string port, std::string method, std::string url)
{
	// Resolve host.
	addrinfo hints;
	addrinfo* addressInfo;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	auto result = getaddrinfo(host.c_str(), port.c_str(), &hints, &addressInfo);
	if (result != 0)
	{
		printf("Error resolving host %s:%s: %d\n", host, port, result);
		return std::string();
	}

	// Setup TCP socket.
	SOCKET tcpSocket = INVALID_SOCKET;
	tcpSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (tcpSocket == INVALID_SOCKET)
	{
		printf("Error creating TCP socket: %ld\n", WSAGetLastError());
		freeaddrinfo(addressInfo);
		return std::string();
	}

	// Connect to server.
	result = connect(tcpSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("Error connecting to server %s:%s: %ld\n", host, port, WSAGetLastError());
		freeaddrinfo(addressInfo);
		closesocket(tcpSocket);
		return std::string();
	}
	else
	{
		freeaddrinfo(addressInfo);
	}

	// Send request.
	std::string request;
	request.append(method + " " + url + " HTTP/1.1\r\n");
	request.append("Host: " + host + "\r\n");
	request.append("Connection: close\r\n\r\n");

	result = send(tcpSocket, request.c_str(), (int)strlen(request.c_str()), 0);
	if (result == SOCKET_ERROR)
	{
		printf("Error sending HTTP request: %d\n", WSAGetLastError());
		closesocket(tcpSocket);
		return std::string();
	}

	// Receive data until the server closes the connection.
	int recvbuflen = 2048;
	char recvbuf[2048];
	int responseLength = 0;

	do
	{
		result = recv(tcpSocket, recvbuf, recvbuflen, 0);

		if (result > 0)
		{
			// Content received.
			responseLength = result;
		}
		else if (result == 0)
		{
			// Connection closed.
		}
		else
		{
			// Error occurred.
			printf("Error receiving server response: %d\n", WSAGetLastError());
		}
	} while (result > 0);

	// Shutdown the send half of the connection since no more data will be sent.
	result = shutdown(tcpSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("Error shuttung down socket: %d\n", WSAGetLastError());
		closesocket(tcpSocket);
		return std::string();
	}

	closesocket(tcpSocket);

	// Return response.
	std::string response(recvbuf, responseLength);
	return response;
}
