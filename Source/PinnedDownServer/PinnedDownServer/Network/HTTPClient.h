#pragma once

#include <string>

namespace PinnedDownServer
{
	namespace Network
	{
		class HTTPClient
		{
		public:
			HTTPClient();
			~HTTPClient();

			std::string SendRequest(std::string host, std::string port, std::string method, std::string url);

		private:
		};
	}
}