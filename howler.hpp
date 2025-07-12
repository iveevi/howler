#pragma once

#include <fmt/color.h>

#include <source_location>

namespace howler {

// Message types
enum class MessageType {
	eReset,
	eError,
	eWarning,
	eInfo,
	eDebug,
	eFatal,
	eAssertion,
	eCount,
};

// Modifying message type display palette
void set_palette(MessageType type, const fmt::text_style &style);

// Logging options
enum class Options : uint8_t {
	eShowTimeStamp,
	eShowSourceLocation,
	eShowSourceLocationForFatal,
	eShowSourceLocationForAssertion,
	eBreakpointOnAssertion,
	eCount,
};

void set_option(Options option, bool enable = true);

// Logging function
void log(MessageType type,
	const std::string &prefix,
	const std::string &message,
	const std::source_location &location = std::source_location::current());

} // namespace howler

#ifndef HOWLER_PREFIX
#define HOWLER_PREFIX "howler"
#endif

#define __hmt	howler::MessageType

// Default prefix
#define __howl_default(type, ...)		\
	howler::log(type,			\
		HOWLER_PREFIX,			\
		fmt::format(__VA_ARGS__))

#define howl_reset(...)	__howl_default(__hmt::eReset, __VA_ARGS__)
#define howl_error(...)	__howl_default(__hmt::eError, __VA_ARGS__)
#define howl_warning(...) __howl_default(__hmt::eWarning, __VA_ARGS__)
#define howl_info(...) __howl_default(__hmt::eInfo, __VA_ARGS__)
#define howl_debug(...) __howl_default(__hmt::eDebug, __VA_ARGS__)
#define howl_fatal(...) __howl_default(__hmt::eFatal, __VA_ARGS__); std::abort()

#define howl_assert(cond, ...)				\
	if (!(cond)) {					\
		howler::log(__hmt::eAssertion,		\
			HOWLER_PREFIX,			\
			fmt::format(__VA_ARGS__));	\
	}

// Explicit prefix
#define __howl_default_prefixed(prefix, type, ...)	\
	howler::log(type, prefix, fmt::format(__VA_ARGS__))

#define howl_reset_prefixed_(...) __howl_default_prefixed(prefix, __hmt::eReset, __VA_ARGS__)
#define howl_error_prefixed(...) __howl_default_prefixed(prefix, __hmt::eError, __VA_ARGS__)
#define howl_warning_prefixed(...) __howl_default_prefixed(prefix, __hmt::eWarning, __VA_ARGS__)
#define howl_info_prefixed(...) __howl_default_prefixed(prefix, __hmt::eInfo, __VA_ARGS__)
#define howl_debug_prefixed(...) __howl_default_prefixed(prefix, __hmt::eDebug, __VA_ARGS__)
#define howl_fatal_prefixed(...) __howl_default_prefixed(prefix, __hmt::eFatal, __VA_ARGS__); std::abort()

#define howl_assert_prefixed(prefix, cond, ...)			\
	if (!(cond)) {						\
		howler::log(__hmt::eAssertion,			\
			prefix,	fmt::format(__VA_ARGS__));	\
	}
