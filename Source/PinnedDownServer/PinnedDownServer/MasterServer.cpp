#include <stdio.h>

#include "MasterServer.h"
#include "PinnedDownServerVersion.h"
#include "PinnedDownServerAnalytics.h"

#include "Util\GUID\GUID.h"
#include "Util\MD5\MD5String.h"

#include "Logger.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;
using namespace PinnedDownServer::Util::GUID;

bool MasterServer::running = false;

MasterServer::MasterServer()
{
	this->logger = std::make_shared<ServerLogger>(LogLevel::Debug);
	this->socketManager = std::make_shared<SocketManager>(this, this->logger);
	this->httpClient = std::make_shared<HTTPClient>(this->logger);

	SetConsoleCtrlHandler(&PinnedDownServer::MasterServer::OnConsoleCtrlSignal, TRUE);
}

void MasterServer::Start()
{
	// Log server version.
	std::string versionString(PINNED_DOWN_SERVER_VERSION);
	std::wstring versionStringW(versionString.begin(), versionString.end());
	this->logger->LogInfo(L"Pinned Down Server " + versionStringW);

	// Init socket manager.
	this->socketManager->InitSocketManager();
	
	if (this->socketManager->IsInitialized())
	{
		// Start receive loop.
		MasterServer::running = true;

		while (MasterServer::running)
		{
			socketManager->Select(100);
		}
	}
}

void MasterServer::OnClientConnected(int clientId)
{
	this->logger->LogInfo(L"Client added: " + std::to_wstring(clientId));

	// Create new client data object.
	PinnedDownClientData client = PinnedDownClientData();
	client.clientId = clientId;
	client.clientGUID = NewGUID();

	// Start new game.
	std::shared_ptr<ServerGame> newGame = std::make_shared<ServerGame>(this, clientId);
	client.game = newGame;

	// Add to client list.
	this->connectedClients.insert(std::pair<int, PinnedDownClientData>(clientId, client));

	// Send analytics event.
	this->SendGameAnalyticsEvent(client, "PlayerJoined:" + client.clientGUID);
}

void MasterServer::OnClientDisconnected(int clientId)
{
	this->logger->LogInfo(L"Client removed: " + std::to_wstring(clientId));

	// Stop game.
	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		this->connectedClients.erase(iterator);
	}
}

void MasterServer::OnClientAction(int clientId, std::shared_ptr<Event> clientAction)
{
	// Log action.
	this->logger->LogDebug(L"FROM client " + std::to_wstring(clientId) + L": " + clientAction->ToString());

	// Pass to game.
	auto iterator = this->connectedClients.find(clientId);

	if (iterator != this->connectedClients.end())
	{
		auto client = iterator->second;
		client.game->OnClientAction(clientAction);
	}
}

void MasterServer::OnServerEvent(int clientId, Event& serverEvent)
{
	// Log event.
	this->logger->LogDebug(L"TO client " + std::to_wstring(clientId) + L": " + serverEvent.ToString());

	// Pass to socket manager.
	this->socketManager->SendServerEvent(clientId, serverEvent);
}

BOOL WINAPI MasterServer::OnConsoleCtrlSignal(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		MasterServer::running = false;
	}		

	return true;
}

void MasterServer::SendGameAnalyticsEvent(PinnedDownClientData client, std::string eventId)
{
	// Build event JSON.
	// http://support.gameanalytics.com/hc/en-us/articles/200841486-General-event-structure
	// http://support.gameanalytics.com/hc/en-us/articles/200841506-Design-event-structure

	std::string json = "[";
	json.append("{");

	// Unique ID representing the user playing your game. This ID should remain the same across different play sessions.
	json.append("\"user_id\":\"" + client.clientGUID + "\",");

	// Unique ID representing the current play session. A new unique ID should be generated at the start of each session, and used for all events throughout that session.
	json.append("\"session_id\":\"" + client.clientGUID + "\",");

	// Describes the current version of the game being played.
	std::string build = PINNED_DOWN_SERVER_VERSION;
	json.append("\"build\":\"" + build + "\",");

	// Identifies the event. This field can be sub-categorized by using ":" notation. For example, an event_id could be: "PickedUpAmmo:Shotgun" (for design), "Purchase:RocketLauncher" (for business), or "Exception:NullReference" (for quality).
	json.append("\"event_id\":\"" + eventId + "\"");

	json.append("}");
	json.append("]");

	// Generate MD5 of event data and secret key.
	std::string secretKey = PINNED_DOWN_SERVER_ANALYTICS_SECRET_KEY;
	std::string digest = MD5String(2, json, secretKey);

	// Send event to GameAnalytics.
	std::string host = PINNED_DOWN_SERVER_ANALYTICS_HOST;

	std::string apiVersion = PINNED_DOWN_SERVER_ANALYTICS_API_VERSION;
	std::string gameKey = PINNED_DOWN_SERVER_ANALYTICS_GAME_KEY;
	std::string category = "design";

	std::string url = "/" + apiVersion + "/" + gameKey + "/" + category;
	
	auto request = HTTPRequest();
	request.authorization = digest;
	request.content = json;
	request.contentType = "application/json";
	request.host = host;
	request.method = "POST";
	request.port = "80";
	request.url = url;

	auto response = this->httpClient->SendRequest(request);
}
