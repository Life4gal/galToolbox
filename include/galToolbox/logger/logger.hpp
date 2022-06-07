#pragma once

#include <galToolbox/utils/macro.hpp>
DISABLE_WARNING_PUSH
DISABLE_WARNING(4189)

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace gal::toolbox
{
	namespace logger = spdlog;
}

DISABLE_WARNING_POP
