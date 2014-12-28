#include "MD5String.h"

std::string MD5String(std::string string)
{
	return MD5String(1, string);
}

std::string MD5String(int stringCount, ...)
{
	MD5_CTX context;
	unsigned char digest[16];
	va_list arguments;

	// Prepare hash.
	MD5Init(&context);

	// Hash all strings.
	va_start(arguments, stringCount);
	for (int x = 0; x < stringCount; x++)
	{
		auto nextString = va_arg(arguments, std::string);
		const char* stringContent = nextString.c_str();
		unsigned int len = strlen(stringContent);
		MD5Update(&context, (unsigned char *)stringContent, len);
	}
	va_end(arguments);

	// Finalize hash.
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