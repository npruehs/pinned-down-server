#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct FlagshipPlayedEvent : public Event
		{
			static const HashedString FlagshipPlayedEventType;

			const HashedString & GetEventType() const
			{
				return FlagshipPlayedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity shipEntity;

			explicit FlagshipPlayedEvent(Entity shipEntity)
			{
				this->shipEntity = shipEntity;
			}
		};
	}
}