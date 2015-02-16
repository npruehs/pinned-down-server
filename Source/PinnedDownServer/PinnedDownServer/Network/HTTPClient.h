#pragma once

#include <string>

#include "HTTPRequest.h"

#include "..\Diagnotics\ServerLogger.h"


using namespace PinnedDownServer::Diagnostics;


namespace PinnedDownServer
{
	namespace Network
	{
		class HTTPClient
		{
		public:
			HTTPClient(std::shared_ptr<ServerLogger> logger);
			~HTTPClient();

			std::string SendRequest(HTTPRequest request);

		private:
			std::shared_ptr<ServerLogger> logger;
		};
	}
}