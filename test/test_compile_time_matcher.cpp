#include <gtest/gtest.h>

#include "../src/string/compile_time_matcher.hpp"

#include <iostream>
#include <format>

TEST(TestCaseName, TestName) {
	using namespace  gal::test;

	static_assert(match("Hell1oWorld", "H*World"));
	// static_assert(make_matcher("[Hh]elloWorld")("HelloWorld"));

	auto result = make_matcher("[Hh]elloWorld")("HelloWorld");//make_matcher("H*World")("HelloWorld");
	
	std::cout << std::format(
		"\n\nH*World and HelloWorld\tmatched:\npattern: {} (size: {})\nsequence: {} (size: {})\nmatch_result: pattern->{}\tsequence->{}\n\n",
		result.pattern_begin, std::distance(result.pattern_begin, result.pattern_end),
		result.sequence_begin, std::distance(result.sequence_begin, result.sequence_end),
		std::distance(result.pattern_begin, result.match_result_pattern), std::distance(result.sequence_begin, result.match_result_sequence)
	);
}