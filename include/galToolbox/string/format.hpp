#pragma once

#include <galToolbox/utils/macro.hpp>
DISABLE_WARNING_PUSH
DISABLE_WARNING(4189)

#ifdef GAL_FMT_NOT_SUPPORT
	#include <fmt/format.h>
namespace std_format = fmt;
#else
	#include <format>
namespace std_format = std;
#endif

DISABLE_WARNING_POP
