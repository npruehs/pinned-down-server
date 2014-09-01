#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Events\FightStartedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class FightSystem : public GameSystem, public IEventListener
		{
		public:
			FightSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			void OnFightStarted(FightStartedEvent& fightStartedEvent);
		};
	}
}