#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <memory>

#include "HTTPClient.h"

#pragma comment(lib, "Ws2_32.lib")


using namespace PinnedDownServer::Diagnostics;
using namespace PinnedDownServer::Network;


HTTPClient::HTTPClient(std::shared_ptr<ServerLogger> logger)
	: logger(logger)
{
}

HTTPClient::~HTTPClient()
{
}

std::string HTTPClient::SendRequest(HTTPRequest request)
{
	// Resolve host.
	addrinfo hints;
	addrinfo* addressInfo;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	auto result = getaddrinfo(request.host.c_str(), request.port.c_str(), &hints, &addressInfo);
	if (result != 0)
	{
		this->logger->LogError("Error resolving host " + request.host + ":" + request.port + ": " + std::to_string(result));
		return std::string();
	}

	// Setup TCP socket.
	SOCKET tcpSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (tcpSocket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		this->logger->LogError("Error creating TCP socket: " + std::to_string(error));

		freeaddrinfo(addressInfo);
		return std::string();
	}

	// Connect to server.
	result = connect(tcpSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		this->logger->LogError("Error connecting to server " + request.host + ":" + request.port + ": " + std::to_string(error));

		freeaddrinfo(addressInfo);
		closesocket(tcpSocket);
		return std::string();
	}
	else
	{
		freeaddrinfo(addressInfo);
	}

	// Build request string.
	std::string requestString;
	requestString.append(request.method + " " + request.url + " HTTP/1.1\r\n");
	requestString.append("Host: " + request.host + "\r\n");
	requestString.append("Connection: close\r\n");
	requestString.append("Content-Type: " + request.contentType + "\r\n");
	requestString.append("Content-Length: " + std::to_string(request.content.length()) + "\r\n");
	
	if (!request.authorization.empty())
	{
		requestString.append("Authorization: " + request.authorization + "\r\n");
	}
	
	requestString.append("\r\n");

	if (!request.content.empty())
	{
		requestString.append(request.content);
	}

	// Send request.
	result = send(tcpSocket, requestString.c_str(), (int)strlen(requestString.c_str()), 0);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		this->logger->LogError("Error sending HTTP request: " + std::to_string(error));

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
			int error = WSAGetLastError();
			this->logger->LogError("Error receiving server response: " + std::to_string(error));
		}
	}
	while (result > 0);

	// Shutdown the send half of the connection since no more data will be sent.
	result = shutdown(tcpSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		this->logger->LogError("Error shuttung down socket: " + std::to_string(error));

		closesocket(tcpSocket);
		return std::string();
	}

	closesocket(tcpSocket);

	// Return response.
	std::string response(recvbuf, responseLength);
	return response;
}
