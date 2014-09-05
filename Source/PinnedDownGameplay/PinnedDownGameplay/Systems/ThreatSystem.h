#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Actions\AddThreatAction.h"
#include "..\Events\EnemyCardPlayedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownServer::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class ThreatSystem : public GameSystem, public IEventListener
		{
		public:
			ThreatSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			int threat;

			void OnEvent(Event & event);

			void OnAddThreat(AddThreatAction& addThreatAction);
			void OnEnemyCardPlayed(EnemyCardPlayedEvent& enemyCardPlayedEvent);

			void SetThreat(int newThreat);
		};
	}
}