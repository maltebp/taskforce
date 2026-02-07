#include <gtest/gtest.h>

#include "core/expect_handling.hpp"

// We use this to have asserts throw exceptions, so that failing subroutines 
// will fail the calling test
class ThrowListener : public testing::EmptyTestEventListener {
	// See https://google.github.io/googletest/advanced.html#asserting-on-subroutines-with-an-exception

  	void OnTestPartResult(const testing::TestPartResult& result) override {
		if (result.type() == testing::TestPartResult::kFatalFailure) {
	  		throw testing::AssertionException(result);
		}
  	}
};

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);

	testing::UnitTest::GetInstance()->listeners().Append(new ThrowListener);

	return RUN_ALL_TESTS();
}