#pragma once

#include <winsock2.h>

#include "PinnedDownNet.h"

namespace PinnedDownServer
{
	namespace Network
	{
		class ServerEventWriter
		{
		public:
			int WriteServerEvent(SOCKET client, ServerEvent event);
		};
	}
}