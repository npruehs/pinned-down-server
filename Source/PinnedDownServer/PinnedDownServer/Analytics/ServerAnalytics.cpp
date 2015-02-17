#include "ServerAnalytics.h"

#include "../PinnedDownServerVersion.h"
#include "../PinnedDownServerAnalytics.h"

#include "../Util/MD5/MD5String.h"


using namespace PinnedDownServer::Analytics;


ServerAnalytics::ServerAnalytics(std::shared_ptr<Game> game, std::shared_ptr<HTTPClient> httpClient, std::shared_ptr<ServerLogger> logger, std::string clientGUID)
	: httpClient(httpClient),
	logger(logger),
	clientGUID(clientGUID)
{
	// Will be done via database in the future, in order to allow players to review their data as well.
	//game->eventManager->AddListener(this, CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);
	//game->eventManager->AddListener(this, DefeatEvent::DefeatEventType);
	//game->eventManager->AddListener(this, FightResolvedEvent::FightResolvedEventType);
	//game->eventManager->AddListener(this, VictoryEvent::VictoryEventType);
}

void ServerAnalytics::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(CoveredDistanceChangedEvent);
	CALL_EVENT_HANDLER(DefeatEvent);
	CALL_EVENT_HANDLER(FightResolvedEvent);
	CALL_EVENT_HANDLER(VictoryEvent);
}

EVENT_HANDLER_DEFINITION(ServerAnalytics, CoveredDistanceChangedEvent)
{
	this->SendGameAnalyticsEvent(this->clientGUID, "DistanceCovered:" + std::to_string(data.distanceCovered));
}

EVENT_HANDLER_DEFINITION(ServerAnalytics, DefeatEvent)
{
	this->SendGameAnalyticsEvent(this->clientGUID, "Match:Defeat");
}

EVENT_HANDLER_DEFINITION(ServerAnalytics, FightResolvedEvent)
{
	if (data.fightOutcome == FightOutcome::PlayerVictory)
	{
		this->SendGameAnalyticsEvent(this->clientGUID, "Fight:Victory");
	}
	else
	{
		this->SendGameAnalyticsEvent(this->clientGUID, "Fight:Defeat");
	}
}

EVENT_HANDLER_DEFINITION(ServerAnalytics, VictoryEvent)
{
	this->SendGameAnalyticsEvent(this->clientGUID, "Match:Victory");
}

void ServerAnalytics::SendGameAnalyticsEvent(std::string clientGUID, std::string eventId)
{
	// Build event JSON.
	// http://support.gameanalytics.com/hc/en-us/articles/200841486-General-event-structure
	// http://support.gameanalytics.com/hc/en-us/articles/200841506-Design-event-structure

	std::string json = "[";
	json.append("{");

	// Unique ID representing the user playing your game. This ID should remain the same across different play sessions.
	json.append("\"user_id\":\"" + clientGUID + "\",");

	// Unique ID representing the current play session. A new unique ID should be generated at the start of each session, and used for all events throughout that session.
	json.append("\"session_id\":\"" + clientGUID + "\",");

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

	this->logger->LogInfo("Sending analytics event " + eventId);
	auto response = this->httpClient->SendRequest(request);

	// Verify response.
	if (response.find("{\"status\":\"ok\"}") == std::string::npos)
	{
		this->logger->LogError("Error sending analytics event " + eventId + ": " + response);
	}
}
