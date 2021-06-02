#include <gtest/gtest.h>

#include <parser/string_parser.hpp>

namespace {
	TEST(STRING_PARSER_TEST, PARSE_BOOLEAN) {
		using namespace gal::toolbox::parser::string_parser;

		bool   result;

		string str1{"True"};
		string str2{"true"};
		string str3{"tRuE"};
		string str4{"1"};
		string str5{"truee"};

		do_parse(str1, result);
		ASSERT_TRUE(result);
		do_parse(str2, result);
		ASSERT_TRUE(result);
		do_parse(str3, result);
		ASSERT_TRUE(result);
		do_parse(str4, result);
		ASSERT_TRUE(result);
		try {
			do_parse(str5, result);
			// the result has not changed
			EXPECT_TRUE(result);
		} catch (parser_exception& e) {
			std::cerr << e.what() << std::endl;
		}

		string str6{"False"};
		string str7{"false"};
		string str8{"faLsE"};
		string str9{"0"};
		string str10{"falsee"};

		do_parse(str6, result);
		ASSERT_FALSE(result);
		do_parse(str7, result);
		ASSERT_FALSE(result);
		do_parse(str8, result);
		ASSERT_FALSE(result);
		do_parse(str9, result);
		ASSERT_FALSE(result);
		try {
			do_parse(str10, result);
			// the result has not changed
			EXPECT_FALSE(result);
		} catch (parser_exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

	TEST(STRING_PARSER_TEST, PARSE_CHAR) {
		using namespace gal::toolbox::parser::string_parser;

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
		} catch (parser_exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

	TEST(STRING_PARSER_TEST, PARSE_INTEGER) {
		using namespace gal::toolbox::parser::string_parser;

		int	   result;

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

	TEST(STRING_PARSER_TEST, PARSE_FLOATING_POINT) {
		using namespace gal::toolbox::parser::string_parser;

		string str1{"1.2345"};
		string str2{"-0x3.1415926"};
		string str3{"-0x213123.21321312321312312312"};

		float  float_result;
		do_parse(str1, float_result);
		ASSERT_FLOAT_EQ(float_result, 1.2345f);
		double double_result;
		do_parse(str2, double_result);
		ASSERT_FLOAT_EQ(double_result, -0x3.1415926p0);
		long double long_double_result;
		do_parse(str3, long_double_result);
		ASSERT_FLOAT_EQ(long_double_result, -0x213123.21321312321312312312p0L);
	}
}// namespace
