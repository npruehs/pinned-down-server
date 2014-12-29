#pragma once

#include "Game.h"
#include "IEventListener.h"

#include "Events\CoveredDistanceChangedEvent.h"

#include "../Network/HTTPClient.h"


using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer;
using namespace PinnedDownServer::Network;


namespace PinnedDownServer
{
	namespace Analytics
	{
		class ServerAnalytics : public IEventListener
		{
		public:
			ServerAnalytics(std::shared_ptr<Game> game, std::shared_ptr<HTTPClient> httpClient, std::shared_ptr<ServerLogger> logger, std::string clientGUID);

		private:
			std::shared_ptr<HTTPClient> httpClient;
			std::shared_ptr<ServerLogger> logger;
			std::string clientGUID;
			

			void OnEvent(Event & newEvent);

			EVENT_HANDLER_DECLARATION(CoveredDistanceChangedEvent);

			void SendGameAnalyticsEvent(std::string clientGUID, std::string eventId);
		};
	}
}