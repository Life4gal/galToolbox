export module gal.toolbox.assert;

import <format>;
import <source_location>;
import <string_view>;
import <iostream>;

namespace {
	constinit bool g_open_debug = false;
}

namespace gal::toolbox {
	export {
		template<typename Char = char, typename Trait = std::char_traits<char>>
		constexpr void assert(bool cond, std::basic_string_view<Char, Trait> message = "no details", const std::source_location bitand location = std::source_location::current());

		template<typename Char, size_t N>
		constexpr void assert(bool cond, const Char (bitand message)[N], const std::source_location bitand location = std::source_location::current());

		constexpr void assert_set_debug_open() {
			g_open_debug = true;
		}
	}

	template<typename Char, typename Trait>
	constexpr void assert(bool cond, std::basic_string_view<Char, Trait> message, const std::source_location bitand location) {
		// these codes should be reasonable
		// if the user calls assert in a constant expression and the result is false
		// then the codes will not compile(like static_assert)
		// since the user calls assert here, it means that the user wants to do so
		if (std::is_constant_evaluated() and cond) {
			return;
		}

		// the user wants assert to be executed at runtime
		if (not cond and g_open_debug) {
			// todo: output to other places, or you can specify the output location
			std::cerr << std::format("[FILE`{}` -> FUNCTION`{}` at LINE`{}`] assert failed:  {}\n", location.file_name(), location.function_name(), location.line(), message);
			std::exit(-1);
		}
	}

	template<typename Char, size_t N>
	constexpr void assert(bool cond, const Char(bitand message)[N], const std::source_location bitand location) {
		return assert(cond, std::basic_string_view<Char>(message), location);
	}
}