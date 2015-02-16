#include <Windows.h>

#include "ServerLogger.h"

using namespace PinnedDownServer::Diagnostics;

ServerLogger::ServerLogger(LogLevel logLevel) : Logger(logLevel)
{
}

ServerLogger::~ServerLogger()
{
}

void ServerLogger::WriteLog(LogLevel logLevel, const wchar_t* const message)
{
	// Get current time.
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	// Format string.
	wchar_t systemTimeBuffer[256];
	swprintf
		(systemTimeBuffer,
		256,
		L"[%02d-%02d-%d %02d:%02d:%02d.%03d] ",
		systemTime.wYear,
		systemTime.wMonth,
		systemTime.wDay,
		systemTime.wHour,
		systemTime.wMinute,
		systemTime.wSecond,
		systemTime.wMilliseconds);

	std::wstring logMessageW(systemTimeBuffer + this->LogLevelToString(logLevel) + L" - " + message);
	std::string logMessage(logMessageW.begin(), logMessageW.end());
	printf(logMessage.c_str());
}
