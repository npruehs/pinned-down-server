#pragma once

#include "ServerGame.h"

namespace PinnedDownServer
{
	class PinnedDownClientData
	{
	public:
		int clientId;
		std::string clientGUID;
		std::shared_ptr<ServerGame> game;
		bool versionVerified;
	};
}