#pragma once

#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	namespace Events
	{
		struct AddThreatAction : public PinnedDownCore::Event
		{
			static const HashedString AddThreatActionType;

			const HashedString & GetEventType() const
			{
				return AddThreatActionType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			int threat;

			explicit AddThreatAction() : AddThreatAction(0)
			{
			}

			explicit AddThreatAction(int threat)
			{
				this->threat = threat;
			}
		};
	}
}