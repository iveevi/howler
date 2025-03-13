#pragma once

#include <fmt/format.h>
#include <fmt/printf.h>

#include <source_location>

namespace howler {

void reset(bool);

void assertion	(const std::string &, const std::source_location & = std::source_location::current());
void fatal	(const std::string &, const std::source_location & = std::source_location::current());
void error	(const std::string &, const std::source_location & = std::source_location::current());
void warning	(const std::string &, const std::source_location & = std::source_location::current());
void info	(const std::string &, const std::source_location & = std::source_location::current());

}

#define howl_assert(cond, ...)						\
	if (!(cond)) {							\
		howler::assertion(fmt::format(__VA_ARGS__));		\
	}

#define howl_fatal(...)	  howler::fatal(fmt::format(__VA_ARGS__))
#define howl_error(...)	  howler::error(fmt::format(__VA_ARGS__))
#define howl_warning(...) howler::warning(fmt::format(__VA_ARGS__))
#define howl_info(...)	  howler::info(fmt::format(__VA_ARGS__))
