#pragma once

#include <string>

extern "C"
{
#include <Rpc.h>
}

namespace PinnedDownServer
{
	namespace Util
	{
		namespace GUID
		{
			std::string NewGUID()
			{
				UUID uuid;
				UuidCreate(&uuid);

				unsigned char * str;
				UuidToStringA(&uuid, &str);

				std::string s((char*)str);

				RpcStringFreeA(&str);

				return s;
			}
		}
	}
}