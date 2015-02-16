#pragma once

#include <string>

#include "Logger.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	namespace Diagnostics
	{
		class ServerLogger : public Logger
		{
		public:
			ServerLogger(LogLevel logLevel);
			~ServerLogger();

			void WriteLog(LogLevel logLevel, const wchar_t* const message);
		};
	}
}