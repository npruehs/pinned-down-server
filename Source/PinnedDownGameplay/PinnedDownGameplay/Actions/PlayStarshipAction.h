#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct PlayStarshipAction : public Event
		{
			static const HashedString PlayStarshipActionType;

			const HashedString & GetEventType() const
			{
				return PlayStarshipActionType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity shipEntity;

			explicit PlayStarshipAction(Entity shipEntity)
			{
				this->shipEntity = shipEntity;
			}
		};
	}
}