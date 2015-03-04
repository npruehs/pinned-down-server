#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct TurnPhaseEndedEvent : public Event
		{
			static const HashedString TurnPhaseEndedEventType;

			const HashedString & GetEventType() const
			{
				return TurnPhaseEndedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}
		};
	}
}