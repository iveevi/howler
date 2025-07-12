#include <chrono>
#include <bitset>

#include <fmt/color.h>
#include <fmt/printf.h>
#include <source_location>

#include "howler.hpp"

#ifdef _MSC_VER
#define breakdown	__debugbreak
#else
#define breakdown	__builtin_trap
#endif

namespace howler {

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

} // namespace howler
