#include "ServerLobby.h"

#include "MasterServer.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;

#include "Events\ClientVersionVerifiedEvent.h"
#include "Events\ClientVersionNotVerifiedEvent.h"
#include "Events\LoginSuccessEvent.h"


ServerLobby::ServerLobby(MasterServer* masterServer, std::shared_ptr<ServerLogger> logger)
	: masterServer(masterServer),
	logger(logger)
{
}

void ServerLobby::AddClient(PinnedDownClientData* client)
{
	this->clients.push_back(client);

	// Send login ACK.
	auto serverEvent = std::make_shared<LoginSuccessEvent>(client->clientId);
	this->masterServer->OnServerEvent(client->clientId, *serverEvent);
}

void ServerLobby::RemoveClient(PinnedDownClientData* client)
{
	this->clients.remove(client);
}

void ServerLobby::OnClientAction(PinnedDownClientData* client, std::shared_ptr<Event> clientAction)
{
	// Check for client version verification.
	if (clientAction->GetEventType() == VerifyClientVersionAction::VerifyClientVersionActionType)
	{
		auto verifyClientVersionAction = std::static_pointer_cast<VerifyClientVersionAction>(clientAction);
		this->OnVerifyClientVersion(client, verifyClientVersionAction);
	}
	// Check for wanting to join game.
	else if (clientAction->GetEventType() == JoinGameAction::JoinGameActionType)
	{
		auto joinGameAction = std::static_pointer_cast<JoinGameAction>(clientAction);
		this->OnJoinGame(client, joinGameAction);
	}
}

void ServerLobby::OnVerifyClientVersion(PinnedDownClientData* client, std::shared_ptr<VerifyClientVersionAction> verifyClientVersionAction)
{
	auto clientUpToDate =
		verifyClientVersionAction->major > REQUIRED_CLIENT_VERSION_MAJOR ||
		(verifyClientVersionAction->major == REQUIRED_CLIENT_VERSION_MAJOR &&
		(verifyClientVersionAction->minor > REQUIRED_CLIENT_VERSION_MINOR ||
		(verifyClientVersionAction->minor == REQUIRED_CLIENT_VERSION_MINOR &&
		(verifyClientVersionAction->build > REQUIRED_CLIENT_VERSION_BUILD ||
		(verifyClientVersionAction->build == REQUIRED_CLIENT_VERSION_BUILD &&
		(verifyClientVersionAction->revision >= REQUIRED_CLIENT_VERSION_REVISION))))));

	client->versionVerified = clientUpToDate;

	if (clientUpToDate)
	{
		this->logger->LogDebug("Client " + std::to_string(client->clientId) + " is up-to-date.");

		auto clientVersionVerifiedEvent = std::make_shared<ClientVersionVerifiedEvent>();
		this->masterServer->OnServerEvent(client->clientId, *clientVersionVerifiedEvent);
	}
	else
	{
		this->logger->LogDebug("Client " + std::to_string(client->clientId) + " is NOT up-of-date.");

		auto clientVersionNotVerifiedEvent = std::make_shared<ClientVersionNotVerifiedEvent>();
		this->masterServer->OnServerEvent(client->clientId, *clientVersionNotVerifiedEvent);
	}
}

void ServerLobby::OnJoinGame(PinnedDownClientData* client, std::shared_ptr<JoinGameAction> joinGameAction)
{
	// Verify client version.
	this->masterServer->OnClientWantsToJoinGame(client);
}