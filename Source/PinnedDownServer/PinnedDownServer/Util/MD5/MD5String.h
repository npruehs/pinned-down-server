#pragma once

#include <cstdarg>
#include <string>

#include "MD5.h"

std::string MD5String(std::string string);
std::string MD5String(int stringCount, ...);