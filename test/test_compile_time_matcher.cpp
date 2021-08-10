#include <gtest/gtest.h>

#include "../src/string/compile_time_matcher.hpp"

TEST(TestCompileTimeMatcher, TestCheckSetExist)
{
	using namespace  gal::test;
	
	{
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

		static_assert(check_set_exist<false>(std::begin(pattern1), std::end(pattern1)) == std::end(pattern1));
		static_assert(check_set_exist<false>(std::begin(pattern2), std::end(pattern2)) == std::end(pattern2));

		static_assert(check_set_exist<false>(std::begin(pattern3), std::end(pattern3)) == std::end(pattern3));
		static_assert(check_set_exist<false>(std::begin(pattern4), std::end(pattern4)) == std::end(pattern4));
		static_assert(check_set_exist<false>(std::begin(pattern5), std::end(pattern5)) == std::end(pattern5));
		static_assert(check_set_exist<false>(std::begin(pattern6), std::end(pattern6)) == std::end(pattern6));
		static_assert(check_set_exist<false>(std::begin(pattern7), std::end(pattern7)) == std::end(pattern7));
		static_assert(check_set_exist<false>(std::begin(pattern8), std::end(pattern8)) == std::end(pattern8));

		static_assert(check_set_exist<false>(std::begin(pattern9), std::end(pattern9)) == std::end(pattern9));
		static_assert(check_set_exist<false>(std::begin(pattern10), std::end(pattern10)) == std::end(pattern10));
		static_assert(check_set_exist<false>(std::begin(pattern11), std::end(pattern11)) == std::end(pattern11));
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

		static_assert(match_set(std::begin(sequence1), std::end(sequence1), std::begin(pattern1), std::end(pattern1)));
	}
}

TEST(TestCompileTimeMatcher, TestMakeMatcher) {
	using namespace  gal::test;

	static_assert(make_matcher("H?llo,*W*!")("Hello, World!"));
	static_assert(make_matcher("H_llo,%W%!", { '%', '_', '\\' })("Hello, World!"));

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