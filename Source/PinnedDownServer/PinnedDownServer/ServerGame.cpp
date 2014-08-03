
#include "ServerGame.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer;

ServerGame::ServerGame()
{
	// Setup game infrastructure.
	this->game = std::make_shared<Game>();
}

void ServerGame::Update()
{
	this->game->Update(1.0f);
}
