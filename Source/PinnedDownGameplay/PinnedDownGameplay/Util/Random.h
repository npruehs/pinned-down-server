#pragma once

namespace PinnedDownServer
{
	namespace Util
	{
		// Implementation of the Ranq1 struct found in Numerical Recipes in C: 3rd Edition.
		// Combined generator (Ranq1 = D1(A1(right-shift first)) with a period of 1.8 x 10^19.
		class Random
		{
		public:
			Random();
			Random(unsigned long long seed);

			unsigned long long NextUnsignedLong();
			unsigned int NextUnsignedInt();
			unsigned int NextUnsignedInt(unsigned int maxExclusive);
			double NextDouble();
			float NextFloat();

		private:
			unsigned long long v;
		};
	}
}