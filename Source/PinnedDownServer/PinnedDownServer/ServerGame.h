#pragma once

#include <memory>

#include "Game.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	class ServerGame
    {
    public:
		ServerGame();

        void Update();

		std::shared_ptr<Game> GetGame() { return std::shared_ptr<Game>(this->game); }

    private:
		std::shared_ptr<PinnedDownCore::Game> game;
    };
}