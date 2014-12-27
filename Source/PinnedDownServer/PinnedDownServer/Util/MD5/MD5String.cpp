#include "MD5String.h"

std::string MD5String(std::string string)
{
	MD5_CTX context;
	unsigned char digest[16];

	const char* stringContent = string.c_str();
	unsigned int len = strlen(stringContent);

	// Create hash.
	MD5Init(&context);
	MD5Update(&context, (unsigned char *)stringContent, len);
	MD5Final(digest, &context);

	// Convert from binary to hex.
	char digestStringBuffer[64];

	unsigned int i;
	unsigned int bytes = 0;

	for (i = 0; i < 16; i++)
	{
		bytes += sprintf_s(digestStringBuffer + bytes, 64 - bytes, "%02x", digest[i]);
	}

	// Convert to string.
	return std::string(digestStringBuffer);
}
