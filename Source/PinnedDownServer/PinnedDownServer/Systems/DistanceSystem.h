#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
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
		};
	}
}