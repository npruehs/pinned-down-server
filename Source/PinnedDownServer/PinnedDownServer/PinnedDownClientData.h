#pragma once

#include "ServerGame.h"

namespace PinnedDownServer
{
	struct PinnedDownClientData
	{
	public:
		int clientId;
		std::string clientGUID;
		std::shared_ptr<ServerGame> game;
	};
}