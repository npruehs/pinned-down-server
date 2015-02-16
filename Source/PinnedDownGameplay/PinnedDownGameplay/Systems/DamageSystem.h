#pragma once

#include <list>

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Data\CardData.h"
#include "Data\CardFactory.h"
#include "..\Data\Deck.h"

#include "..\Events\ShipDefeatedEvent.h"
#include "EntityRemovedEvent.h"

#include "Damage\DamageData.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Data;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems::Damage;
using namespace PinnedDownNet::Data;


namespace PinnedDownGameplay
{
	namespace Systems
	{
		class DamageSystem : public GameSystem, public IEventListener
		{
		public:
			DamageSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<CardFactory> cardFactory;
			std::shared_ptr<Deck> damageDeck;
			std::list<DamageData> damageList;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(EntityRemovedEvent);
			EVENT_HANDLER_DECLARATION(ShipDefeatedEvent);

			void PrepareDamageDeck();
		};
	}
}