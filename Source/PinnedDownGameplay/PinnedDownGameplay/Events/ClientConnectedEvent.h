#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct ClientConnectedEvent : public Event
		{
			static const HashedString ClientConnectedEventType;

			const HashedString & GetEventType() const
			{
				return ClientConnectedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			int clientId;

			explicit ClientConnectedEvent(int clientId)
			{
				this->clientId = clientId;
			}
		};
	}
}