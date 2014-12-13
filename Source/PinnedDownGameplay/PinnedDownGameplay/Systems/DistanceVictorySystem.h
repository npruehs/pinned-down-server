#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\CoveredDistanceChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class DistanceVictorySystem : public GameSystem, public IEventListener
		{
		public:
			DistanceVictorySystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			bool lastTurn = false;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(CoveredDistanceChangedEvent);
		};
	}
}