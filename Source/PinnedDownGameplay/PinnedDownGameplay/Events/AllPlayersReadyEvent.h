#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct AllPlayersReadyEvent : public Event
		{
			static const HashedString AllPlayersReadyEventType;

			const HashedString & GetEventType() const
			{
				return AllPlayersReadyEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}
		};
	}
}