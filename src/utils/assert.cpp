#include <galToolbox/utils/assert.hpp>
#include <galToolbox/utils/macro.hpp>

#include <galToolbox/string/format.hpp>

#include <iostream>

namespace gal
{
	#ifndef GAL_NO_ASSERT
	void gal_assert(
			const bool condition,
			std::string_view message,
			const std_source_location& location) noexcept
	{
		if (not condition)
		{
			// todo: output to other places, or you can specify the output location

			std::cerr << std_format::format("[FILE: {} -> FUNCTION: {} -> LINE: {}] assert failed: {}\n", location.file_name(), location.function_name(), location.line(), message);

			DEBUG_TRAP();
			std::abort();
		}
	}
	#endif
}// namespace gal
