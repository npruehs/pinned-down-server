#pragma once

#include <string>

#include "HTTPRequest.h"

namespace PinnedDownServer
{
	namespace Network
	{
		class HTTPClient
		{
		public:
			HTTPClient();
			~HTTPClient();

			std::string SendRequest(HTTPRequest request);
		private:
		};
	}
}