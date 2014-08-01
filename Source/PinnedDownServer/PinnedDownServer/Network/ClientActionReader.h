#pragma once

#include "PinnedDownNet.h"

namespace PinnedDownServer
{
	namespace Network
	{
		class ClientActionReader
		{
		public:
			ClientAction ReadClientAction(char* buffer);
		};
	}
}