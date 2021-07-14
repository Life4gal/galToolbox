export module gal.toolbox.assert;

import <format>;
import <source_location>;
import <string_view>;
import <iostream>;

namespace gal::toolbox {
	export {
		template<typename Char, typename Trait>
		constexpr void assert(bool cond, std::basic_string_view<Char, Trait> message, const std::source_location& location = std::source_location::current());

		template<typename Char, size_t N>
		constexpr void assert(bool cond, const Char (&message)[N], const std::source_location& location = std::source_location::current());
	}

	template<typename Char, typename Trait>
	constexpr void assert(bool cond, std::basic_string_view<Char, Trait> message, const std::source_location& location) {
		if (not cond) {
			// todo: output to other places, or you can specify the output location
			std::cerr << std::format("[FILE`{}` -> FUNCTION`{}` at LINE`{}`] assert failed:  {}\n", location.file_name(), location.function_name(), location.line(), message);
			std::exit(-1);
		}
	}

	template<typename Char, size_t N>
	constexpr void assert(bool cond, const Char(&message)[N], const std::source_location& location)
	{
		return assert(cond, std::basic_string_view<Char>(message), location);
	}
}