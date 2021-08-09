#include <gtest/gtest.h>

#include "../src/string/compile_time_matcher.hpp"

TEST(TestCaseName, TestName) {
	using namespace  gal::test;

	static_assert(match("Hell1oWorld", "H*World"));
	static_assert(make_matcher( "H*World" )("HelloWorld"));
}