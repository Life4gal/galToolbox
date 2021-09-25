#pragma once

#include <format>
#include <source_location>
#include <string_view>
#include <iostream>

namespace gal::toolbox::utils
{
	constexpr void gal_assert(
			bool cond,
			std::string_view message = {"no detail"},
			const std::source_location& location = std::source_location::current()
			) noexcept
	{
#ifndef NDEBUG
		// these codes should be reasonable
		// if the user calls assert in a constant expression and the result is false
		// then the codes will not compile(like static_assert)
		// since the user calls assert here, it means that the user wants to do so
		if(std::is_constant_evaluated() and cond)
		{
			return;
		}

		// the user wants to assert to be executed at runtime
		if(not cond)
		{
			// todo: output to other places, or you can specify the output location
			std::cerr << std::format("[FILE: {} -> FUNCTION: {} -> LINE: {}] assert failed: {}\n", location.file_name(), location.function_name(), location.line(), message);
//			std::cerr << "[FILE: " << location.file_name() <<
//					" -> FUNCTION: " << location.function_name() <<
//					" -> LINE: " << location.line() <<
//					"] assert failed: " << message << std::endl;
			std::exit(-1);
		}
#endif
	}
}
