#include <gtest/gtest.h>

#include <parser/command_line_parser.hpp>

namespace {
	TEST(TEST_COMMAND_LINE_PARSER, EXCEPTION_TEST) {
		using namespace gal::toolbox::parser;

		std::cout << ArgDuplicateException{"Duplicate Exception"}.what() << '\n'
				  << ArgInvalidException{"Invalid Exception"}.what() << '\n'
				  << ArgSyntaxException{"Syntax Exception"}.what() << '\n'
				  << ArgFakeException{"Fake Exception"}.what() << '\n'
				  << ArgMissingException{"Missing Exception"}.what() << '\n'
				  << ArgNotSatisfiedException{"Not Satisfied Exception"}.what() << '\n'
				  << ArgRejectException{"Reject Exception", "dummy"}.what() << '\n'
				  << ArgNotPresentException{"Not Present Exception"}.what() << '\n'
				  << ArgEmptyException{"Empty Exception"}.what() << '\n'
				  << ArgBadTypeException{"Bad Type Exception"}.what() << std::endl;
	}
}// namespace
