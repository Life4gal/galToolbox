#ifndef GAL_TOOLBOX_STRING_PARSER_HPP
#define GAL_TOOLBOX_STRING_PARSER_HPP

#include <charconv>
#include <exception>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace gal::toolbox::parser::string_parser {
	class parser_exception : std::exception {
	public:
		explicit parser_exception(std::string message)
			: message(std::move(message)) {}

		[[nodiscard]] const char* what() const noexcept override {
			return message.c_str();
		}

	private:
		std::string message;
	};

	inline namespace detail {
		using char_type	  = char;

		using string	  = std::basic_string<char_type>;
		using string_view = std::basic_string_view<char_type>;

		template<typename T>
		struct is_container {
			static constexpr bool value = false;
		};

		template<typename T>
		struct is_container<std::vector<T>> {
			static constexpr bool value = true;
		};

		template<typename T>
		constexpr static bool	   is_container_v	   = is_container<T>::value;

		constexpr static char_type container_delimiter = ',';
	}// namespace detail

	namespace impl {
		bool get_text_boolean_result(const string_view text) {
			// todo: need constexpr string
			constexpr static string_view TRUE  = "true";
			constexpr static string_view FALSE = "false";

			if (text.length() == 1) {
				if (text[0] == '1') {
					return true;
				} else if (text[0] == '0') {
					return false;
				}
			} else if ((text.front() == 't' or text.front() == 'T') and text.length() == TRUE.length()) {
				// if text equal to TRUE, then return true
				return std::equal(
						text.cbegin(),
						text.cend(),
						std::cbegin(TRUE),
						[](char a, char b) { return std::tolower(a) == std::tolower(b); });
			} else if ((text.front() == 'f' or text.front() == 'F') and text.length() == FALSE.length()) {
				// if text equal to FALSE, then return false
				return !std::equal(
						text.cbegin(),
						text.cend(),
						std::cbegin(FALSE),
						[](char a, char b) { return std::tolower(a) == std::tolower(b); });
			}

			throw parser_exception{std::string{"Cannot parse `"}.append(text) + "` to boolean!"};
		}

		char get_text_char_result(const string_view text) {
			if (text.length() == 1) {
				return text[0];
			}

			throw parser_exception{std::string{"Cannot parse `"}.append(text) + "` to char!"};
		}

		template<typename T>
		requires std::is_integral_v<T> and(not std::is_same_v<T, bool>) and (not std::is_same_v<T, char>) or std::is_floating_point_v<T> T get_text_integer_or_floating_point_result(const string_view text) {
			if (not text.empty()) {
				auto begin	  = text.data();
				bool negative = false;
				bool hex	  = false;

				if constexpr (std::is_signed_v<T>) {
					if (text[0] == '-') {
						begin += 1;
						negative = true;
					}
				}

				if (text.length() >= 2 and begin[0] == '0' and (begin[1] == 'x' or begin[1] == 'X')) {
					begin += 2;
					hex = true;
				}

				T ret{};
				// std::from_chars cannot hex number like -0x123, it just parse `-` and `0` and return 0
				// we must do it by self
				if constexpr (std::is_floating_point_v<T>) {
					std::from_chars(begin, text.data() + text.length(), ret, hex ? std::chars_format::hex : std::chars_format::general);
				} else {
					std::from_chars(begin, text.data() + text.length(), ret, hex ? 16 : 10);
				}

				return negative ? -ret : ret;
			}

			throw parser_exception{std::string{"Cannot parse `"}.append(text) + "` to integer or floating point!"};
		}

		template<typename T>
		void get_text_container_result(const string_view text, std::vector<T>& result) {
			string_view::size_type it = 0;
			for (;;) {
				auto next = text.find_first_of(container_delimiter, it);

				if (next == string::npos) {
					break;
				}

				result.template emplace_back(text.substr(it, next));
				it = next + sizeof(container_delimiter);
			}

			if (it != text.size()) {
				result.template emplace_back(text.substr(it));
			}
		}
	}// namespace impl

	template<typename T>
	void do_parse(const string_view text, T& out) {
		if constexpr (std::is_same_v<T, bool>) {
			out = impl::get_text_boolean_result(text);
		} else if constexpr (std::is_same_v<T, char>) {
			out = impl::get_text_char_result(text);
		} else if constexpr (std::is_integral_v<T> or std::is_floating_point_v<T>) {
			out = impl::get_text_integer_or_floating_point_result<T>(text);
		} else if constexpr (is_container_v<T>) {
			impl::get_text_container_result(text, out);
		} else {
			out = text;
		}
	}
}// namespace gal::toolbox::parser::string_parser

#endif//GAL_TOOLBOX_STRING_PARSER_HPP
