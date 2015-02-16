#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class DistanceSystem : public GameSystem, public IEventListener
		{
		public:
			DistanceSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			int distanceCovered;
			int distanceMaximum;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);

			void SetDistanceCovered(int distanceCovered);
		};
	}
}