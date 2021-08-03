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
		constexpr void assert(bool cond, std::basic_string_view<Char, Trait> message = {}, const std::source_location bitand location = std::source_location::current()) noexcept;

		template<typename Char, size_t N>
		constexpr void assert(bool cond, const Char (bitand message)[N], const std::source_location bitand location = std::source_location::current()) noexcept;

		constexpr void assert_set_debug_open() noexcept;
	}

	template<typename Char, typename Trait>
	constexpr void assert(bool cond, std::basic_string_view<Char, Trait> message, const std::source_location bitand location) noexcept {
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
			std::cerr << std::format("[FILE: {} -> FUNCTION: {} -> LINE: {}] assert failed:  {}\n", location.file_name(), location.function_name(), location.line(), message);
			std::exit(-1);
		}
	}

	template<typename Char, size_t N>
	constexpr void assert(bool cond, const Char(bitand message)[N], const std::source_location bitand location) noexcept {
		return assert(cond, std::basic_string_view<Char>(message), location);
	}

	constexpr void assert_set_debug_open() noexcept {
		g_open_debug = true;
	}
}