#include <gtest/gtest.h>

import gal.toolbox.compile_time_matcher;
// compile_time_matcher.hpp is reserved to test several functions that do not need to be exported in the module
#include "../src/string/compile_time_matcher.hpp"
#include <array>

TEST(TestCompileTimeMatcher, TestCheckSetExist)
{
	using namespace  gal::test;
	
	{
		// matched
		
		constexpr char pattern1[] = "[a]";
		constexpr char pattern2[] = "[abc]";

		constexpr char pattern3[] = "[]]";
		constexpr char pattern4[] = "[]a]";
		constexpr char pattern5[] = "[]abc]";
		constexpr char pattern6[] = "[][]";
		constexpr char pattern7[] = "[][a]";
		constexpr char pattern8[] = "[][abc]";

		constexpr char pattern9[] = "[[]";
		constexpr char pattern10[] = "[[a]";
		constexpr char pattern11[] = "[[abc]";

		// the '-1' is just because of the '\0'
		static_assert(check_set_exist<false>(std::begin(pattern1), std::end(pattern1)) == std::end(pattern1) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern2), std::end(pattern2)) == std::end(pattern2) - 1);

		static_assert(check_set_exist<false>(std::begin(pattern3), std::end(pattern3)) == std::end(pattern3) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern4), std::end(pattern4)) == std::end(pattern4) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern5), std::end(pattern5)) == std::end(pattern5) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern6), std::end(pattern6)) == std::end(pattern6) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern7), std::end(pattern7)) == std::end(pattern7) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern8), std::end(pattern8)) == std::end(pattern8) - 1);

		static_assert(check_set_exist<false>(std::begin(pattern9), std::end(pattern9)) == std::end(pattern9) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern10), std::end(pattern10)) == std::end(pattern10) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern11), std::end(pattern11)) == std::end(pattern11) - 1);
	}
	{
		// matched
		
		constexpr char pattern1[] = "[!a]";
		constexpr char pattern2[] = "[!abc]";

		constexpr char pattern3[] = "[!]]";
		constexpr char pattern4[] = "[!]a]";
		constexpr char pattern5[] = "[!]abc]";
		constexpr char pattern6[] = "[!][]";
		constexpr char pattern7[] = "[!][a]";
		constexpr char pattern8[] = "[!][abc]";

		constexpr char pattern9[] = "[![]";
		constexpr char pattern10[] = "[![a]";
		constexpr char pattern11[] = "[![abc]";

		// the '-1' is just because of the '\0'
		static_assert(check_set_exist<false>(std::begin(pattern1), std::end(pattern1)) == std::end(pattern1) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern2), std::end(pattern2)) == std::end(pattern2) - 1);

		static_assert(check_set_exist<false>(std::begin(pattern3), std::end(pattern3)) == std::end(pattern3) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern4), std::end(pattern4)) == std::end(pattern4) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern5), std::end(pattern5)) == std::end(pattern5) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern6), std::end(pattern6)) == std::end(pattern6) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern7), std::end(pattern7)) == std::end(pattern7) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern8), std::end(pattern8)) == std::end(pattern8) - 1);

		static_assert(check_set_exist<false>(std::begin(pattern9), std::end(pattern9)) == std::end(pattern9) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern10), std::end(pattern10)) == std::end(pattern10) - 1);
		static_assert(check_set_exist<false>(std::begin(pattern11), std::end(pattern11)) == std::end(pattern11) - 1);
	}
	{
		// not matched
		
		constexpr char pattern1[] = "";
		constexpr char pattern2[] = "a";
		constexpr char pattern3[] = "!";

		constexpr char pattern4[] = "[";
		constexpr char pattern5[] = "[a";
		constexpr char pattern6[] = "[]";
		constexpr char pattern7[] = "[]a";

		constexpr char pattern8[] = "[!";
		constexpr char pattern9[] = "[!a";
		constexpr char pattern10[] = "[!]";
		constexpr char pattern11[] = "[!]a";

		static_assert(check_set_exist<false>(std::begin(pattern1), std::end(pattern1)) == std::begin(pattern1));
		static_assert(check_set_exist<false>(std::begin(pattern2), std::end(pattern2)) == std::begin(pattern2));

		static_assert(check_set_exist<false>(std::begin(pattern3), std::end(pattern3)) == std::begin(pattern3));
		static_assert(check_set_exist<false>(std::begin(pattern4), std::end(pattern4)) == std::begin(pattern4));
		static_assert(check_set_exist<false>(std::begin(pattern5), std::end(pattern5)) == std::begin(pattern5));
		static_assert(check_set_exist<false>(std::begin(pattern6), std::end(pattern6)) == std::begin(pattern6));
		static_assert(check_set_exist<false>(std::begin(pattern7), std::end(pattern7)) == std::begin(pattern7));
		static_assert(check_set_exist<false>(std::begin(pattern8), std::end(pattern8)) == std::begin(pattern8));

		static_assert(check_set_exist<false>(std::begin(pattern9), std::end(pattern9)) == std::begin(pattern9));
		static_assert(check_set_exist<false>(std::begin(pattern10), std::end(pattern10)) == std::begin(pattern10));
		static_assert(check_set_exist<false>(std::begin(pattern11), std::end(pattern11)) == std::begin(pattern11));
	}
}

TEST(TestCompileTimeMatcher, TestMatchSet)
{
	using namespace  gal::test;

	{
		constexpr char pattern[] = "[a]";

		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "a";
		constexpr char sequence3[] = "b";
		
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[abc]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "a";
		constexpr char sequence3[] = "b";
		constexpr char sequence4[] = "c";
		constexpr char sequence5[] = "d";
	
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence4), std::end(sequence4), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence5), std::end(sequence5), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[]]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "]";
		constexpr char sequence3[] = "a";
	
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[]abc]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "]";
		constexpr char sequence3[] = "a";
		constexpr char sequence4[] = "b";
		constexpr char sequence5[] = "c";
		constexpr char sequence6[] = "d";
	
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence4), std::end(sequence4), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence5), std::end(sequence5), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence6), std::end(sequence6), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[!a]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "a";
		constexpr char sequence3[] = "b";
	
		// -1 to skip '\0'
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1) - 1, std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[!abc]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "a";
		constexpr char sequence3[] = "b";
		constexpr char sequence4[] = "c";
		constexpr char sequence5[] = "d";
	
		// -1 to skip '\0'
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1) - 1, std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence4), std::end(sequence4), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence5), std::end(sequence5), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[!]]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "]";
		constexpr char sequence3[] = "a";
	
		// -1 to skip '\0'
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1) - 1, std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
	}
	{
		constexpr char pattern[] = "[!]abc]";
	
		constexpr char sequence1[] = "";
		constexpr char sequence2[] = "]";
		constexpr char sequence3[] = "a";
		constexpr char sequence4[] = "b";
		constexpr char sequence5[] = "c";
		constexpr char sequence6[] = "d";
	
		// -1 to skip '\0'
		static_assert(not match_set(std::begin(sequence1), std::end(sequence1) - 1, std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence3), std::end(sequence3), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence4), std::end(sequence4), std::begin(pattern), std::end(pattern)));
		static_assert(not match_set(std::begin(sequence5), std::end(sequence5), std::begin(pattern), std::end(pattern)));
		static_assert(match_set(std::begin(sequence6), std::end(sequence6), std::begin(pattern), std::end(pattern)));
	}
	{
		// not constexpr
		const char pattern1[] = "";
		const char pattern2[] = "a";
		const char pattern3[] = "!";
	
		const char pattern4[] = "[";
		const char pattern5[] = "[a";
		const char pattern6[] = "[]";
		const char pattern7[] = "[]a";
	
		const char pattern8[] = "[!";
		const char pattern9[] = "[!a";
		const char pattern10[] = "[!]";
		const char pattern11[] = "[!]a";
	
		const char sequence1[] = "a";
		const char sequence2[] = "b";

		// even not a legal set, `[` is missing
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern1), std::end(pattern1)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern1), std::end(pattern1)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern2), std::end(pattern2)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern2), std::end(pattern2)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern3), std::end(pattern3)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern3), std::end(pattern3)), std::invalid_argument);

		// maybe it is a legal set, but after comparing with it, it does not match
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern4), std::end(pattern4)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern4), std::end(pattern4)), std::invalid_argument);

		// maybe it is a legal set, and after comparing with it, it does match
		ASSERT_NO_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern5), std::end(pattern5)));
		ASSERT_TRUE(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern5), std::end(pattern5)));
		// maybe it is a legal set, but after comparing with it, it does not match
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern5), std::end(pattern5)), std::invalid_argument);

		// maybe it is a legal set, but after comparing with it, it does not match
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern6), std::end(pattern6)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern6), std::end(pattern6)), std::invalid_argument);

		// although the set does not match, the sequence matches
		ASSERT_NO_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern7), std::end(pattern7)));
		ASSERT_TRUE(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern7), std::end(pattern7)));
		// the set does not match, the sequence also does not matches
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern7), std::end(pattern7)), std::invalid_argument);

		// maybe it is a legal set, but after comparing with it, it does not match
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern8), std::end(pattern8)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern8), std::end(pattern8)), std::invalid_argument);

		// maybe it is a legal set, but after comparing with it, it does not match
		ASSERT_NO_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern9), std::end(pattern9)));
		ASSERT_FALSE(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern9), std::end(pattern9)));
		// maybe it is a legal set, and after comparing with it, it does match, but pattern ended early
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern9), std::end(pattern9)), std::invalid_argument);

		// maybe it is a legal set, but after comparing with it, it does not match
		ASSERT_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern10), std::end(pattern10)), std::invalid_argument);
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern10), std::end(pattern10)), std::invalid_argument);

		// although it seems that this is a legal set, due to the existence of `!`, the following `]` is regarded as an option, not the end of the set
		ASSERT_NO_THROW(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern11), std::end(pattern11)));
		ASSERT_FALSE(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern11), std::end(pattern11)));
		// although it seems that this is a legal set, due to the existence of `!`, the following `]` is regarded as an option, not the end of the set
		// so although it seems to match, because the set terminator(`]`) cannot be found, it is judged to be an illegal set
		ASSERT_THROW(match_set(std::begin(sequence2), std::end(sequence2), std::begin(pattern11), std::end(pattern11)), std::invalid_argument);
	}
}

TEST(TestCompileTimeMatcher, TestCheckAltExist)
{
	using namespace  gal::test;

	{
		constexpr char pattern1[] = "()";
		constexpr char pattern2[] = "(a)";
		constexpr char pattern3[] = "(())";
		constexpr char pattern4[] = "((a))";
		constexpr char pattern5[] = "(a()a)";
		constexpr char pattern6[] = "(a(a)a)";

		constexpr char pattern7[] = "(\\()";
		constexpr char pattern8[] = "([(])";

		// the first `\` will skip `*`
		// the second `\` will skip `(`
		// the third `\` will skip `)`
		// the fourth `\` will skip `(`
		// the fifth `\` will skip `)`
		// so we have two nested sets(`[abc]` and `[!abc]`) and a nested alt(`(sin[abc]|[!abc])` (escape removed))
		constexpr char pattern9[] = R"((a\*\(b+c\)/(sin\([abc]\)|[!abc])))";

		// the '-1' is just because of the '\0'
		static_assert(check_alt_exist<false>(std::begin(pattern1), std::end(pattern1)) == std::end(pattern1) - 1);
		static_assert(check_alt_exist<false>(std::begin(pattern2), std::end(pattern2)) == std::end(pattern2) - 1);
		static_assert(check_alt_exist<false>(std::begin(pattern3), std::end(pattern3)) == std::end(pattern3) - 1);
		static_assert(check_alt_exist<false>(std::begin(pattern4), std::end(pattern4)) == std::end(pattern4) - 1);
		static_assert(check_alt_exist<false>(std::begin(pattern5), std::end(pattern5)) == std::end(pattern5) - 1);
		static_assert(check_alt_exist<false>(std::begin(pattern6), std::end(pattern6)) == std::end(pattern6) - 1);

		static_assert(check_alt_exist<false>(std::begin(pattern7), std::end(pattern7)) == std::end(pattern7) - 1);
		static_assert(check_alt_exist<false>(std::begin(pattern8), std::end(pattern8)) == std::end(pattern8) - 1);
		
		static_assert(check_alt_exist<false>(std::begin(pattern9), std::end(pattern9)) == std::end(pattern9) - 1);
	}
	{
		constexpr char pattern1[] = "";
		constexpr char pattern2[] = "a";
		constexpr char pattern3[] = "|";

		constexpr char pattern4[] = "(";
		constexpr char pattern5[] = "(a";
		constexpr char pattern6[] = "(|";
		constexpr char pattern7[] = "(()";
		constexpr char pattern8[] = "((a)";
		constexpr char pattern9[] = "(a()";
		constexpr char pattern10[] = "(a(a)";

		// there is no legal alt at all
		static_assert(check_alt_exist<false>(std::begin(pattern1), std::end(pattern1)) == std::begin(pattern1));
		static_assert(check_alt_exist<false>(std::begin(pattern2), std::end(pattern2)) == std::begin(pattern2));
		static_assert(check_alt_exist<false>(std::begin(pattern3), std::end(pattern3)) == std::begin(pattern3));

		static_assert(check_alt_exist<false>(std::begin(pattern4), std::end(pattern4)) == std::begin(pattern4));
		static_assert(check_alt_exist<false>(std::begin(pattern5), std::end(pattern5)) == std::begin(pattern5));
		static_assert(check_alt_exist<false>(std::begin(pattern6), std::end(pattern6)) == std::begin(pattern6));
		static_assert(check_alt_exist<false>(std::begin(pattern7), std::end(pattern7)) == std::begin(pattern7));
		static_assert(check_alt_exist<false>(std::begin(pattern8), std::end(pattern8)) == std::begin(pattern8));
		static_assert(check_alt_exist<false>(std::begin(pattern9), std::end(pattern9)) == std::begin(pattern9));
		static_assert(check_alt_exist<false>(std::begin(pattern10), std::end(pattern10)) == std::begin(pattern10));
	}
	{
		// not constexpr
		char pattern1[] = "";
		char pattern2[] = "a";
		char pattern3[] = "|";

		char pattern4[] = "(";
		char pattern5[] = "(a";
		char pattern6[] = "(|";
		char pattern7[] = "(()";
		char pattern8[] = "((a)";
		char pattern9[] = "(a()";
		char pattern10[] = "(a(a)";

		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern1), std::end(pattern1)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern2), std::end(pattern2)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern3), std::end(pattern3)), std::invalid_argument);

		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern4), std::end(pattern4)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern5), std::end(pattern5)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern6), std::end(pattern6)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern7), std::end(pattern7)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern8), std::end(pattern8)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern9), std::end(pattern9)), std::invalid_argument);
		ASSERT_THROW(check_alt_exist<true>(std::begin(pattern10), std::end(pattern10)), std::invalid_argument);
	}
}

TEST(TestCompileTimeMatcher, TestMatch)
{
	using namespace  gal::test;

	{
		constexpr char pattern1[] = "";
		constexpr char pattern2[] = R"(\)";

		static_assert(match("", pattern1));
		static_assert(match("", pattern2));
		
		static_assert(not match("we don't care what's here", pattern1));
		static_assert(not match("we don't care what's here", pattern2));
	}
	{
		constexpr char pattern1[] = "A";
		constexpr char pattern2[] = R"(A\)";
		constexpr char pattern3[] = R"(\A)";
		constexpr char pattern4[] = "[A]";
		constexpr char pattern5[] = "(A)";
		constexpr char pattern6[] = R"((\A))";
		constexpr char pattern7[] = R"(([A]))";

		static_assert(match("A", pattern1));
		static_assert(match("A", pattern2));
		static_assert(match("A", pattern3));
		static_assert(match("A", pattern4));
		static_assert(match("A", pattern5));
		static_assert(match("A", pattern6));
		static_assert(match("A", pattern7));

		static_assert(not match("", pattern1));
		static_assert(not match("", pattern2));
		static_assert(not match("", pattern3));
		static_assert(not match("", pattern4));
		static_assert(not match("", pattern5));
		static_assert(not match("", pattern6));
		static_assert(not match("", pattern7));

		static_assert(not match("a", pattern1));
		static_assert(not match("a", pattern2));
		static_assert(not match("a", pattern3));
		static_assert(not match("a", pattern4));
		static_assert(not match("a", pattern5));
		static_assert(not match("a", pattern6));
		static_assert(not match("a", pattern7));

		static_assert(not match("AA", pattern1));
		static_assert(not match("AA", pattern2));
		static_assert(not match("AA", pattern3));
		static_assert(not match("AA", pattern4));
		static_assert(not match("AA", pattern5));
		static_assert(not match("AA", pattern6));
		static_assert(not match("AA", pattern7));

		static_assert(not match("we don't care what's here", pattern1));
		static_assert(not match("we don't care what's here", pattern2));
		static_assert(not match("we don't care what's here", pattern3));
		static_assert(not match("we don't care what's here", pattern4));
		static_assert(not match("we don't care what's here", pattern5));
		static_assert(not match("we don't care what's here", pattern6));
		static_assert(not match("we don't care what's here", pattern7));
	}
	{
		constexpr char pattern1[] = "Hello!";
		constexpr char pattern2[] = R"(Hello!\)";
		constexpr char pattern3[] = R"(\H\e\l\l\o\!)";
		constexpr char pattern4[] = "[H][e][l][l][o]!";
		constexpr char pattern5[] = "(Hello!)";
		constexpr char pattern6[] = R"((\H\e\l\l\o\!))";
		constexpr char pattern7[] = R"(([H][e][l][l][o]!))";

		static_assert(match("Hello!", pattern1));
		static_assert(match("Hello!", pattern2));
		static_assert(match("Hello!", pattern3));
		static_assert(match("Hello!", pattern4));
		static_assert(match("Hello!", pattern5));
		static_assert(match("Hello!", pattern6));
		static_assert(match("Hello!", pattern7));

		static_assert(not match("", pattern1));
		static_assert(not match("", pattern2));
		static_assert(not match("", pattern3));
		static_assert(not match("", pattern4));
		static_assert(not match("", pattern5));
		static_assert(not match("", pattern6));
		static_assert(not match("", pattern7));

		static_assert(not match("Hello!!", pattern1));
		static_assert(not match("Hello!!", pattern2));
		static_assert(not match("Hello!!", pattern3));
		static_assert(not match("Hello!!", pattern4));
		static_assert(not match("Hello!!", pattern5));
		static_assert(not match("Hello!!", pattern6));
		static_assert(not match("Hello!!", pattern7));

		static_assert(not match("Hello!Hello!", pattern1));
		static_assert(not match("Hello!Hello!", pattern2));
		static_assert(not match("Hello!Hello!", pattern3));
		static_assert(not match("Hello!Hello!", pattern4));
		static_assert(not match("Hello!Hello!", pattern5));
		static_assert(not match("Hello!Hello!", pattern6));
		static_assert(not match("Hello!Hello!", pattern7));
	}
	{
		constexpr char pattern1[] = "*";
		constexpr char pattern2[] = R"(*\)";
		constexpr char pattern3[] = R"(\*)";
		constexpr char pattern4[] = "[*]";
		constexpr char pattern5[] = "(*)";
		constexpr char pattern6[] = R"((\*))";
		constexpr char pattern7[] = "([*])";

		static_assert(match("", pattern1));
		static_assert(match("", pattern2));
		static_assert(not match("", pattern3));
		static_assert(not match("", pattern4));
		static_assert(match("", pattern5));
		static_assert(not match("", pattern6));
		static_assert(not match("", pattern7));

		static_assert(match("*", pattern1));
		static_assert(match("*", pattern2));
		static_assert(match("*", pattern3));
		static_assert(match("*", pattern4));
		// matched without '\0'
		static_assert(not match("*", pattern5));
		static_assert(match("*", pattern6));
		static_assert(match("*", pattern7));

		static_assert(match("we don't care what's here", pattern1));
		static_assert(match("we don't care what's here", pattern2));
		static_assert(not match("we don't care what's here", pattern3));
		static_assert(not match("we don't care what's here", pattern4));
		// matched without '\0'
		static_assert(not match("we don't care what's here", pattern5));
		static_assert(not match("we don't care what's here", pattern6));
		static_assert(not match("we don't care what's here", pattern7));
	}
	{
		constexpr char pattern1[] = "?";
		constexpr char pattern2[] = R"(?\)";
		constexpr char pattern3[] = R"(\?)";
		constexpr char pattern4[] = "[?]";
		constexpr char pattern5[] = "(?)";
		constexpr char pattern6[] = R"((\?))";
		constexpr char pattern7[] = R"([?])";

		static_assert(match("A", pattern1));
		static_assert(match("A", pattern2));
		static_assert(not match("A", pattern3));
		static_assert(not match("A", pattern4));
		static_assert(match("A", pattern5));
		static_assert(not match("A", pattern6));
		static_assert(not match("A", pattern7));

		static_assert(match("a", pattern1));
		static_assert(match("a", pattern2));
		static_assert(not match("a", pattern3));
		static_assert(not match("a", pattern4));
		static_assert(match("a", pattern5));
		static_assert(not match("a", pattern6));
		static_assert(not match("a", pattern7));

		static_assert(match("?", pattern1));
		static_assert(match("?", pattern2));
		static_assert(match("?", pattern3));
		static_assert(match("?", pattern4));
		static_assert(match("?", pattern5));
		static_assert(match("?", pattern6));
		static_assert(match("?", pattern7));

		static_assert(not match("", pattern1));
		static_assert(not match("", pattern2));
		static_assert(not match("", pattern3));
		static_assert(not match("", pattern4));
		static_assert(not match("", pattern5));
		static_assert(not match("", pattern6));
		static_assert(not match("", pattern7));

		static_assert(not match("we don't care what's here", pattern1));
		static_assert(not match("we don't care what's here", pattern2));
		static_assert(not match("we don't care what's here", pattern3));
		static_assert(not match("we don't care what's here", pattern4));
		static_assert(not match("we don't care what's here", pattern5));
		static_assert(not match("we don't care what's here", pattern6));
		static_assert(not match("we don't care what's here", pattern7));
	}
	{
		constexpr char pattern1[] = R"(\\\* *\? \*\\)";
		constexpr char pattern2[] = R"([\][*] *[?] [*][\])";

		static_assert(match(R"(\* Hello? *\)", pattern1));
		static_assert(match(R"(\* Hello? *\)", pattern2));

		static_assert(match(R"(\* Hi? *\)", pattern1));
		static_assert(match(R"(\* Hi? *\)", pattern2));

		static_assert(match(R"(\* ? *\)", pattern1));
		static_assert(match(R"(\* ? *\)", pattern2));

		static_assert(not match(R"(\* Hello! *\)", pattern1));
		static_assert(not match(R"(\* Hello! *\)", pattern2));

		static_assert(not match(R"(* Hello? *\)", pattern1));
		static_assert(not match(R"(* Hello? *\)", pattern2));

		static_assert(not match(R"(\ Hello? *\)", pattern1));
		static_assert(not match(R"(\ Hello? *\)", pattern2));

		static_assert(not match(R"( Hello? *\)", pattern1));
		static_assert(not match(R"( Hello? *\)", pattern2));

		constexpr wchar_t pattern_wc[] = L"H?llo,*W*!";
		constexpr char8_t pattern_u8[] = u8"H?llo,*W*!";
		constexpr char16_t pattern_u16[] = u"H?llo,*W*!";
		constexpr char32_t pattern_u32[] = U"H?llo,*W*!";

		static_assert(match(L"Hello, World!", pattern_wc));
		static_assert(match(u8"Hello, World!", pattern_u8));
		static_assert(match(u"Hello, World!", pattern_u16));
		static_assert(match(U"Hello, World!", pattern_u32));
	}
	{
		static_assert(match("aaa", "a[abc]a"));
		static_assert(not match("aaa", "a[bcd]a"));
		static_assert(not match("aaa", "a[a]]a"));
		static_assert(match("aa]a", "a[a]]a"));
		static_assert(match("aaa", "a[]abc]a"));
		static_assert(match("aaa", "a[[a]a"));
		static_assert(match("a[a", "a[[a]a"));
		static_assert(match("a]a", "a[]]a"));
		static_assert(not match("aa", "a[]a"));
		static_assert(match("a[]a", "a[]a"));

		static_assert(not match("aaa", "a[!a]a"));
		static_assert(match("aaa", "a[!b]a"));
		static_assert(not match("aaa", "a[b!b]a"));
		static_assert(match("a!a", "a[b!b]a"));
		static_assert(not match("a!a", "a[!]a"));
		static_assert(match("a[!]a", "a[!]a"));
	}
	{
		static_assert(match("aXb", "a(X|Y)b"));
		static_assert(match("aYb", "a(X|Y)b"));
		static_assert(not match("aZb", "a(X|Y)b"));
		static_assert(match("aXb", "(a(X|Y)b|c)"));
		static_assert(not match("a", "a|b"));
		static_assert(match("a|b", "a|b"));
		static_assert(match("(aa", "(a(a|b)"));
		static_assert(not match("a(a", "(a(a|b)"));
		static_assert(match("a(a", "(a[(]a|b)"));
		static_assert(match("aa", "a()a"));
		static_assert(match("", "(abc|)"));
	}
}

TEST(TestCompileTimeMatcher, TestMakeMatcher) {
	using namespace  gal::toolbox;

	{
		static_assert(make_matcher("H?llo,*W*!")("Hello, World!"));
		static_assert(make_matcher("H_llo,%W%!", { '%', '_', '\\' })("Hello, World!"));
	}
	{
		struct my_equal_to
		{
			constexpr bool operator()(const int num, const int character) const
			{
				return num + 48 == character;
			}
		};

		constexpr std::string_view str1{ "12*5?" };
		constexpr std::u8string_view str2{ u8"12*5?" };
		constexpr std::u16string_view str3{ u"12*5?" };
		constexpr std::u32string_view str4{ U"12*5?" };
		
		static_assert(make_matcher(str1, my_equal_to{})(std::array{1, 2, 3, 4, 5, 6}));
		static_assert(make_matcher(str2, my_equal_to{})(std::array{1, 2, 3, 4, 5, 6}));
		static_assert(make_matcher(str3, my_equal_to{})(std::array{1, 2, 3, 4, 5, 6}));
		static_assert(make_matcher(str4, my_equal_to{})(std::array{1, 2, 3, 4, 5, 6}));
	}
}

TEST(TestCompileTimeMatcher, TestLiterals)
{
	using namespace  gal::toolbox;
	using namespace gal::toolbox::literals;

	{
		static_assert("12*5?"_sm("123456"));
		static_assert(u8"12*5?"_sm(u8"123456"));
		static_assert(u"12*5?"_sm(u"123456"));
		static_assert(U"12*5?"_sm(U"123456"));
	}
}

struct point
{
	int x_;
	int y_;

	constexpr bool operator==(const point bitand other) const noexcept
	{
		return x_ == other.x_ and y_ == other.y_;
	}
};

template<std::size_t N>
struct my_point_container
{
	std::array<point, N> points_;

	[[nodiscard]] constexpr auto begin() const noexcept
	{
		return std::cbegin(points_);
	}
	
	[[nodiscard]] constexpr auto end() const noexcept
	{
		return std::cend(points_);
	}
};

template<>
struct gal::toolbox::wildcard_type<point>
{
	using value_type = point;

	// default
	explicit constexpr wildcard_type() = default;

	// basic
	constexpr wildcard_type(
		const value_type customize_anything,
		const value_type customize_single,
		const value_type customize_escape
	)
		:
		anything_(customize_anything),
		single_(customize_single),
		escape_(customize_escape) {}

	// basic + extend
	constexpr wildcard_type(
		const value_type customize_anything,
		const value_type customize_single,
		const value_type customize_escape,

		const value_type customize_set_open,
		const value_type customize_set_close,
		const value_type customize_set_not,

		const value_type customize_alt_open,
		const value_type customize_alt_close,
		const value_type customize_alt_or
	)
		:
		anything_(customize_anything),
		single_(customize_single),
		escape_(customize_escape),
		set_open_(customize_set_open),
		set_close_(customize_set_close),
		set_not_(customize_set_not),
		alt_open_(customize_alt_open),
		alt_close_(customize_alt_close),
		alt_or_(customize_alt_or) {}

	// standard
	value_type anything_{ 10, 10 };
	value_type single_{ 20, 20 };
	value_type escape_{ 30, 30 };

	// extended
	value_type set_open_{ 40, 40 };
	value_type set_close_{ 50, 50 };
	value_type set_not_{ 60, 60 };

	value_type alt_open_{ 70, 70 };
	value_type alt_close_{ 80, 80 };
	value_type alt_or_{ 90, 90 };
};

TEST(TestCompileTimeMatcher, TestCustomType)
{
	using namespace  gal::toolbox;

	constexpr my_point_container<10> container1 =  
	{{
			{
				{1, 10},
				{ 20, 20 }, // single
				{2, 20},
				{3, 30},
				{ 10, 10 }, // anything,
				{4, 40},
				{ 30, 30 }, // escape
				{ 10, 10 }, // escaped by escape, does not mean `anything`
				{5, 50},
				{6, 60},
			}
		}};

	constexpr my_point_container<13> container2 =
	{ {
			{
				{10, 1},
				{1234, 5678},
				{20, 2},
				{30, 3},
				{1234, 5678},
				{1234, 5678},
				{1234, 5678},
				{1234, 5678},
				{1234, 5678},
				{40, 4},
				{100, 1},
				{50, 5},
				{60, 6}
			}
		} };

	constexpr auto point_compare = [](const point bitand p1, const point bitand p2) constexpr -> bool
	{
		return p1.x_ * p1.y_ == p2.x_ * p2.y_;
	};

	static_assert(make_matcher(container1, point_compare)(container2));

	ASSERT_TRUE(make_matcher(container1, point_compare)(container2));
}