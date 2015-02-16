#pragma once

#include <memory>

#include "PinnedDownNet.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	namespace Network
	{
		class ClientActionReader
		{
		public:
			std::shared_ptr<Event> ReadClientAction(char* buffer);
		};
	}
}