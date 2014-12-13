#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "..\Events\EffectPlayedEvent.h"
#include "EntityRemovedEvent.h"
#include "Events\ShipDamagedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class EffectPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			EffectPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			TurnPhase currentTurnPhase;

			void OnEvent(Event & event);

			std::list<Entity> damagedShips;

			void OnEffectPlayed(EffectPlayedEvent& effectPlayedEvent);
			void OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent);
			void OnShipDamaged(ShipDamagedEvent& shipDamagedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}