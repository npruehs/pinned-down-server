#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct StarshipPlayedEvent : public Event
		{
			static const HashedString StarshipPlayedEventType;

			const HashedString & GetEventType() const
			{
				return StarshipPlayedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity shipEntity;

			explicit StarshipPlayedEvent(Entity shipEntity)
			{
				this->shipEntity = shipEntity;
			}
		};
	}
}