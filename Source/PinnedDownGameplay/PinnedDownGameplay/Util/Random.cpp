#include <ctime>

#include "Random.h"

using namespace PinnedDownGameplay::Util;

Random::Random() : Random((unsigned long long)time(0))
{
}

Random::Random(unsigned long long seed) :
	v(4101842887655102017LL)
{
	v ^= seed;
	v = this->NextUnsignedLong();
}

unsigned long long Random::NextUnsignedLong()
{
	v ^= v >> 21;
	v ^= v << 35;
	v ^= v >> 4;

	return v * 2685821657736338717LL;
}

unsigned int Random::NextUnsignedInt()
{
	return (unsigned int)this->NextUnsignedLong();
}

unsigned int Random::NextUnsignedInt(unsigned int maxExclusive)
{
	return (unsigned int)(this->NextUnsignedLong() % (unsigned long long)maxExclusive);
}

double Random::NextDouble()
{
	return 5.42101086242752217E-20 * this->NextUnsignedLong();
}

float Random::NextFloat()
{
	return (float)this->NextDouble();
}
