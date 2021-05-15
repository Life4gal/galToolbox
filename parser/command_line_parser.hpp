/**
 * Inspire form cxxopts(https://github.com/jarro2783/cxxopts)(MIT License)
 *
 * If there is no legal license conflict, the file will be distributed under the [see root project license] license.
 */

#ifndef GAL_TOOLBOX_COMMAND_LINE_PARSER_HPP
#define GAL_TOOLBOX_COMMAND_LINE_PARSER_HPP

#if !defined(GAL_COMMAND_LINE_PARSER_DELIMITER)
#define GAL_COMMAND_LINE_PARSER_DELIMITER ','
#endif

#if !defined(GAL_COMMAND_LINE_PARSER_DEFAULT_STRING)
#if !defined(GAL_COMMAND_LINE_PARSER_NO_STD_STRING) && !__has_include(<string>)
#include <string>
#endif

#if __has_include(<string>)
#define GAL_COMMAND_LINE_PARSER_DEFAULT_STRING std::string
#else
#error "no string type specified"
#endif
#endif

#if !defined(GAL_COMMAND_LINE_PARSER_DEFAULT_STRING_VIEW)
#if !defined(GAL_COMMAND_LINE_PARSER_NO_STD_STRING_VIEW) && !__has_include(<string_view>)
#include <string_view>
#endif

#if __has_include(<string_view>)
#define GAL_COMMAND_LINE_PARSER_DEFAULT_STRING_VIEW std::string_view
#else
#warning "no string_view type specified, using const string& instead"
#define GAL_COMMAND_LINE_PARSER_DEFAULT_STRING_VIEW const GAL_COMMAND_LINE_PARSER_DEFAULT_STRING&
#endif
#endif

#include <exception>
#include <memory>
#include <regex>

namespace gal::toolbox::parser {
	namespace detail {
		using string	  = GAL_COMMAND_LINE_PARSER_DEFAULT_STRING;
		using string_view = GAL_COMMAND_LINE_PARSER_DEFAULT_STRING_VIEW;
#if defined(_WIN32)
//		constexpr string left_quote{"\'"};
//		constexpr string right_quote{"\'"};
#define GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what) ("\'" + (what) + "\'")
#else
//		constexpr string left_quote{"‘"};
//		constexpr string right_quote{"’"};
#define GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what) ("‘" + (what) + "’")
#endif
	}// namespace detail

	class Token : public std::enable_shared_from_this<Token> {
	public:
		using share_type												= std::shared_ptr<Token>;
		using string_type												= detail::string;
		using string_view_type											= detail::string_view;

		virtual share_type		 clone() const							= 0;

		virtual void			 parse(string_view_type text) const		= 0;

		virtual void			 parse() const							= 0;

		virtual bool			 has_default_value() const				= 0;

		virtual bool			 is_container() const					= 0;

		virtual bool			 has_implicit_cast() const				= 0;

		virtual string_type		 get_default_value() const				= 0;

		virtual string_view_type get_default_value_view() const			= 0;

		virtual string_type		 get_implicit_value() const				= 0;

		virtual string_view_type get_implicit_value_view() const		= 0;

		virtual share_type		 default_value(string_view_type value)	= 0;

		virtual share_type		 implicit_value(string_view_type value) = 0;

		virtual bool			 is_boolean() const						= 0;
	};

	class ParserException : public std::exception {
	public:
		using string_type = detail::string;

		explicit ParserException(string_type message) : message(std::move(message)) {}

		explicit ParserException(const char* message) : message(message) {}

		[[nodiscard]] const char* what() const noexcept override {
			return message.c_str();
		}

	private:
		string_type message;
	};

	class SpecifyArgException : public ParserException {
	public:
		using ParserException::ParserException;
	};

	class ParseArgException : public ParserException {
	public:
		using ParserException::ParserException;
	};

	class ArgDuplicateException : public SpecifyArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgDuplicateException(const string_type& what)
			: SpecifyArgException("Arg already exists: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgInvalidException : public SpecifyArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgInvalidException(const string_type& what)
			: SpecifyArgException("Arg has invalid format: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgSyntaxException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgSyntaxException(const string_type& what)
			: ParseArgException("Arg has incorrect syntax: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgFakeException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgFakeException(const string_type& what)
			: ParseArgException("Arg does not exists: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgMissingException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgMissingException(const string_type& what)
			: ParseArgException("Arg is missing an argument: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgNotSatisfiedException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgNotSatisfiedException(const string_type& what)
			: ParseArgException("Arg is not satisfied: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgRejectException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgRejectException(const string_type& what, const string_type& given)
			: ParseArgException("Arg is reject to give: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what) +
								"\n\tbut still given: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(given)) {}
	};

	class ArgNotPresentException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgNotPresentException(const string_type& what)
			: ParseArgException("Arg not present: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgEmptyException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgEmptyException(const string_type& what)
			: ParseArgException("Arg is empty: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	class ArgBadTypeException : public ParseArgException {
	public:
		using string_type = typename SpecifyArgException::string_type;

		explicit ArgBadTypeException(const string_type& what)
			: ParseArgException("Arg has a bad type and failed to parse: " + GAL_COMMAND_LINE_PARSER_ARG_WRAPPER(what)) {}
	};

	namespace detail {
		constexpr const char* integer_pattern = "(-)?(0x)?([0-9a-zA-Z]+)|((0x)?0)";
		constexpr const char* truthy_pattern  = "(t|T)(rue)?|1";
		constexpr const char* falsy_pattern	  = "(f|F)(alse)?|0";
		constexpr const char* arg_matcher	  = "--([[:alnum:]][-_[:alnum:]]+)(=(.*))?|-([[:alnum:]]+)";
		constexpr const char* arg_specifier	  = "(([[:alnum:]]),)?[ ]*([[:alnum:]][-_[:alnum:]]*)?";

		struct IntegerDescriptor {
			using string_type = detail::string;

			string_type negative;
			string_type base;
			string_type value;
		};

		struct ArgumentDescriptor
		{
			using string_type = detail::string;

			string_type arg_name;
			bool grouping;
			bool set_value;
			string_type value;
		};
	}// namespace detail
}// namespace gal::toolbox::parser

#endif//GAL_TOOLBOX_COMMAND_LINE_PARSER_HPP
