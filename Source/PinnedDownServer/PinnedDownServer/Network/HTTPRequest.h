#pragma once

#include <string>

namespace PinnedDownServer
{
	namespace Network
	{
		struct HTTPRequest
		{
			std::string host;
			std::string port;
			std::string method;
			std::string url;
			std::string content;
			std::string contentType;
			std::string authorization;
		};
	}
}