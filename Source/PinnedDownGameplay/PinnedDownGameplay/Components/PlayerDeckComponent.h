#pragma once

#include <list>

#include "EntityManager.h"
#include "IEntityComponent.h"

#include "..\Data\Deck.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Data;

namespace PinnedDownGameplay
{
	namespace Components
	{
		class PlayerDeckComponent : public PinnedDownCore::IEntityComponent
		{
		public:
			static const HashedString PlayerDeckComponentType;

			const HashedString & GetComponentType() const
			{
				return PlayerDeckComponentType;
			}

			std::shared_ptr<Deck> drawDeck;
			std::list<Entity> hand;
		};
	}
}