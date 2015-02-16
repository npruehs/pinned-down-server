#pragma once

#include <winsock2.h>

#include "Event.h"
#include "PinnedDownNet.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	namespace Network
	{
		class ServerEventWriter
		{
		public:
			int WriteServerEvent(SOCKET client, Event& serverEvent);
		};
	}
}