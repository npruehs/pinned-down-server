#include <stdio.h>

#include "gtest/gtest.h"

// The following lines pull in the real gtest *.cc files.
#include "src/gtest.cc"
#include "src/gtest-death-test.cc"
#include "src/gtest-filepath.cc"
#include "src/gtest-port.cc"
#include "src/gtest-printers.cc"
#include "src/gtest-test-part.cc"
#include "src/gtest-typed-test.cc"

GTEST_API_ int main(int argc, char **argv) {
	printf("Running main() from gtest_main.cc\n");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
