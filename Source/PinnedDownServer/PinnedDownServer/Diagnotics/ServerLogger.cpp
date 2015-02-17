#include <Windows.h>

#include "ServerLogger.h"

using namespace PinnedDownServer::Diagnostics;

ServerLogger::ServerLogger(LogLevel logLevel) : Logger(logLevel)
{
}

ServerLogger::~ServerLogger()
{
}

void ServerLogger::WriteLog(LogLevel logLevel, const char* const message)
{
	// Get current time.
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	// Format string.
	char systemTimeBuffer[256];
	sprintf_s
		(systemTimeBuffer,
		"[%02d-%02d-%d %02d:%02d:%02d.%03d] ",
		systemTime.wYear,
		systemTime.wMonth,
		systemTime.wDay,
		systemTime.wHour,
		systemTime.wMinute,
		systemTime.wSecond,
		systemTime.wMilliseconds);

	std::string logMessage(systemTimeBuffer + this->LogLevelToString(logLevel) + " - " + message);
	printf(logMessage.c_str());
}
