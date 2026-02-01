/*
 * MIT License
 *
 * Copyright (c) 2026 Venkataram Edavamadathil Sivaram
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#ifdef _MSC_VER
#define breakdown	__debugbreak
#else
#define breakdown	__builtin_trap
#endif

#include <bitset>
#include <chrono>
#include <source_location>

#include <fmt/color.h>
#include <fmt/printf.h>

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
void log(
	MessageType type,
	const std::string &prefix,
	const std::string &message,
	const std::source_location &location = std::source_location::current()
);

#ifdef HOWLER_IMPL

// Message color palette
static std::array <fmt::text_style, uint32_t(MessageType::eCount)> g_palette {
	fmt::emphasis::bold | fmt::fg(fmt::color::sea_green),
	fmt::emphasis::bold | fmt::fg(fmt::color::red),
	fmt::emphasis::bold | fmt::fg(fmt::color::golden_rod),
	fmt::emphasis::bold | fmt::fg(fmt::color::teal),
	fmt::emphasis::bold | fmt::fg(fmt::color::blue_violet),
	fmt::emphasis::bold | fmt::fg(fmt::color::brown),
	fmt::emphasis::bold | fmt::fg(fmt::color::hot_pink),
};

void set_palette(MessageType type, const fmt::text_style &style)
{
	g_palette[uint32_t(type)] = style;
}

// Formatting for the prefix
static fmt::text_style g_prefix_style = fmt::emphasis::bold;

std::string format_prefix(const std::string &prefix)
{
	return fmt::format(g_prefix_style, "{}:", prefix);
}

// Formatting for the message type
static std::array <const char *const, uint32_t(MessageType::eCount)> g_type_strings {
	"reset",
	"error",
	"warning",
	"info",
	"debug",
	"fatal",
	"assertion",
};

std::string format_type(MessageType type)
{
	return fmt::format(g_palette[uint32_t(type)], "{}:",
			g_type_strings[uint32_t(type)]);
}

// Formatting for the message itself
static fmt::text_style g_message_style = fmt::fg(fmt::color::white);

std::string format_message(const std::string &message)
{
	return fmt::format(g_message_style, "{}", message);
}

// Formatting for the timestamp
using Clock = std::chrono::high_resolution_clock;

static Clock g_clock;
static Clock::time_point g_epoch = g_clock.now();

static std::string delta_to_stamp(int64_t delta)
{
	int32_t milliseconds = delta / 1000.0;

	int32_t seconds = milliseconds / 1000;
	milliseconds %= 1000;

	int32_t minutes = seconds / 60;
	seconds %= 60;

	return fmt::format("[{}:{:02d}:{:03d}]", minutes, seconds, milliseconds);
}

std::string timestamp()
{
	auto now = g_clock.now();
	auto count = std::chrono::duration_cast
		<std::chrono::microseconds>
			(now - g_epoch).count();
	return delta_to_stamp(count);
}

static fmt::text_style g_time_style = fmt::fg(fmt::color::gray);

std::string format_timestamp()
{
	return fmt::format(g_time_style, "{}", timestamp());
}

// Formatting for the source location
static fmt::text_style g_location_style = fmt::emphasis::faint | fmt::emphasis::italic;

std::string format_location(const std::source_location &location)
{
	return fmt::format(g_location_style, "\n  from {}:{}\n  in {}",
		location.file_name(),
		location.line(),
		location.function_name());
}

// Logging and formatting options
static auto g_options = []() {
	std::bitset <uint8_t(Options::eCount)> defaults = false;

	defaults[uint8_t(Options::eShowTimeStamp)] = true;
	defaults[uint8_t(Options::eShowSourceLocation)] = false;
	defaults[uint8_t(Options::eShowSourceLocationForFatal)] = true;
	defaults[uint8_t(Options::eShowSourceLocationForAssertion)] = true;
	defaults[uint8_t(Options::eBreakpointOnAssertion)] = true;

	return defaults;
} ();

bool on(Options a)
{
	return g_options[uint8_t(a)];
}

void set_option(Options a, bool enable)
{
	g_options[uint8_t(a)] = enable;
}

// Final formatted logging
std::string default_formatter(MessageType type,
	const std::string &prefix,
	const std::string &message,
	const std::source_location &location)
{
	std::string result;

	if (on(Options::eShowTimeStamp))
		result += format_timestamp() += " ";

	result += format_prefix(prefix);
	result += " " + format_type(type);
	result += " " + format_message(message);

	if (on(Options::eShowSourceLocation)
			|| (on(Options::eShowSourceLocationForFatal)
				&& type == MessageType::eFatal)
			|| (on(Options::eShowSourceLocationForAssertion)
				&& type == MessageType::eAssertion))
		result += format_location(location);

	return result;
}

void log(MessageType type,
	const std::string &prefix,
	const std::string &message,
	const std::source_location &location)
{
	fmt::println("{}", default_formatter(type, prefix, message, location));

	if (type == MessageType::eFatal)
		breakdown();
	if (on(Options::eBreakpointOnAssertion)
			&& type == MessageType::eAssertion)
		breakdown();
}

#endif

} // namespace howler

#ifndef HOWLER_PREFIX
#define HOWLER_PREFIX "howler"
#endif

#define howl_enable(option) howler::set_option(option, true)
#define howl_disable(option) howler::set_option(option, false)

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

#define howl_assert_plain(cond) howl_assert(cond, #cond)

// Explicit prefix
#define __howl_default_prefixed(prefix, type, ...)	\
	howler::log(type, prefix, fmt::format(__VA_ARGS__))

#define howl_reset_prefixed_(prefix, ...) __howl_default_prefixed(prefix, __hmt::eReset, __VA_ARGS__)
#define howl_error_prefixed(prefix, ...) __howl_default_prefixed(prefix, __hmt::eError, __VA_ARGS__)
#define howl_warning_prefixed(prefix, ...) __howl_default_prefixed(prefix, __hmt::eWarning, __VA_ARGS__)
#define howl_info_prefixed(prefix, ...) __howl_default_prefixed(prefix, __hmt::eInfo, __VA_ARGS__)
#define howl_debug_prefixed(prefix, ...) __howl_default_prefixed(prefix, __hmt::eDebug, __VA_ARGS__)
#define howl_fatal_prefixed(prefix, ...) __howl_default_prefixed(prefix, __hmt::eFatal, __VA_ARGS__); std::abort()

#define howl_assert_prefixed(prefix, cond, ...)			\
	if (!(cond)) {						\
		howler::log(__hmt::eAssertion,			\
			prefix,	fmt::format(__VA_ARGS__));	\
	}

#define howl_assert_plain_prefixed(prefix, cond) howl_assert_prefixed(prefix, cond, #cond)
