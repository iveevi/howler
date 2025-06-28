#pragma once

#include <fmt/format.h>
#include <fmt/printf.h>

#include <source_location>

namespace howler {

void reset	(const std::string &, const std::string &);
void error	(const std::string &, const std::string &, const std::source_location & = std::source_location::current());
void warning	(const std::string &, const std::string &, const std::source_location & = std::source_location::current());
void info	(const std::string &, const std::string &, const std::source_location & = std::source_location::current());
void assertion	(const std::string &, const std::string &, const std::source_location & = std::source_location::current());

[[noreturn]]
void fatal	(const std::string &, const std::string &, const std::source_location & = std::source_location::current());

} // namespace howler

#ifndef HOWLER_PREFIX
#define HOWLER_PREFIX "howler"
#endif

// Default prefix
#define howl_assert(cond, ...)							\
	if (!(cond)) {								\
		howler::assertion(HOWLER_PREFIX, fmt::format(__VA_ARGS__));	\
	}

#define howl_reset(...)	  howler::reset(HOWLER_PREFIX, fmt::format(__VA_ARGS__))
#define howl_error(...)	  howler::error(HOWLER_PREFIX, fmt::format(__VA_ARGS__))
#define howl_warning(...) howler::warning(HOWLER_PREFIX, fmt::format(__VA_ARGS__))
#define howl_info(...)	  howler::info(HOWLER_PREFIX, fmt::format(__VA_ARGS__))
#define howl_fatal(...)	  howler::fatal(HOWLER_PREFIX, fmt::format(__VA_ARGS__))

// Explicit prefix
#define howl_prefixed_assert(prefix, cond, ...)				\
	if (!(cond)) {							\
		howler::assertion(prefix, fmt::format(__VA_ARGS__));	\
	}

#define howl_prefixed_error(prefix, ...)	howler::error(prefix, fmt::format(__VA_ARGS__))
#define howl_prefixed_warning(prefix, ...)	howler::warning(prefix, fmt::format(__VA_ARGS__))
#define howl_prefixed_info(prefix, ...)		howler::info(prefix, fmt::format(__VA_ARGS__))
#define howl_prefixed_fatal(prefix, ...)	howler::fatal(prefix, fmt::format(__VA_ARGS__))