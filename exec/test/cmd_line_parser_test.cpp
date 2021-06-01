#include <gtest/gtest.h>

#include <parser/cmd_line_parser.hpp>

namespace {
	TEST(CMD_LINE_PARSER_TEST, PARSE_BOOLEAN) {
		using namespace gal::toolbox::parser::detail;

		bool   result;

		string str1{"True"};
		string str2{"true"};
		string str3{"tRue"};
		string str4{"1"};

		do_parse(str1, result);
		ASSERT_TRUE(result);
		do_parse(str2, result);
		ASSERT_TRUE(result);
		try {
			do_parse(str3, result);
			// the result has not changed
			EXPECT_TRUE(result);
		} catch (...) {}
		do_parse(str4, result);
		ASSERT_TRUE(result);

		string str5{"False"};
		string str6{"false"};
		string str7{"faLse"};
		string str8{"0"};

		do_parse(str5, result);
		ASSERT_FALSE(result);
		do_parse(str6, result);
		ASSERT_FALSE(result);
		try {
			do_parse(str7, result);
			// the result has not changed
			EXPECT_FALSE(result);
		} catch (...) {}
		do_parse(str8, result);
		ASSERT_FALSE(result);
	}

	TEST(CMD_LINE_PARSER_TEST, PARSE_CHAR) {
		using namespace gal::toolbox::parser::detail;

		char   result;

		string str1{"A"};
		string str2{"a"};
		string str3{"1"};
		string str4{"123"};

		do_parse(str1, result);
		EXPECT_EQ(result, 'A');
		do_parse(str2, result);
		ASSERT_EQ(result, 'a');
		do_parse(str3, result);
		ASSERT_EQ(result, '1');
		try {
			do_parse(str4, result);
			EXPECT_EQ(result, '1');
		} catch (...) {}
	}

	TEST(CMD_LINE_PARSER_TEST, PARSE_INTEGER)
	{
		using namespace gal::toolbox::parser::detail;

		int result;

		string str1{"-12345"};
		string str2{"-0x12345"};
		string str3{"0X12345"};
		string str4{"0x0"};
		string str5{"-0X0"};

		do_parse(str1, result);
		ASSERT_EQ(result, -12345);
		do_parse(str2, result);
		ASSERT_EQ(result, -0x12345);
		do_parse(str3, result);
		ASSERT_EQ(result, 0x12345);
		do_parse(str4, result);
		ASSERT_EQ(result, -0);
		do_parse(str5, result);
		ASSERT_EQ(result, 0);
	}

	TEST(CMD_LINE_PARSER_TEST, PARSE_FLOATING_POINT)
	{
		using namespace gal::toolbox::parser::detail;

		string str1{"1.2345"};
		string str2{"-3.1415926"};
		string str3{"-213123.21321312321312312312"};

		float float_result;
		do_parse(str1, float_result);
		ASSERT_FLOAT_EQ(float_result, 1.2345f);
		double double_result;
		do_parse(str2, double_result);
		ASSERT_FLOAT_EQ(double_result, -3.1415926);
		long double long_double_result;
		do_parse(str3, long_double_result);
		ASSERT_FLOAT_EQ(long_double_result, -213123.21321312321312312312L);
	}
}// namespace
