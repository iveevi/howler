#pragma once

#pragma once

#include <fmt/format.h>
#include <fmt/printf.h>

namespace howler {

void reset(bool);

void assertion	(const std::string &);
void fatal	(const std::string &);
void error	(const std::string &);
void warning	(const std::string &);
void info	(const std::string &);

}

#define howl_assert(cond, ...)						\
	if (!(cond)) {							\
		howler::assertion(fmt::format(__VA_ARGS__));		\
	}

#define howl_fatal(...)	  howler::fatal(fmt::format(__VA_ARGS__))
#define howl_error(...)	  howler::error(fmt::format(__VA_ARGS__))
#define howl_warning(...) howler::warning(fmt::format(__VA_ARGS__))
#define howl_info(...)	  howler::info(fmt::format(__VA_ARGS__))
