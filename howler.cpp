#include <chrono>

#include <fmt/color.h>
#include <fmt/printf.h>

#include "howler.hpp"

#ifdef _MSC_VER
#define breakdown	__debugbreak
#else
#define breakdown	__builtin_trap
#endif

// Internal logging state
struct Howler {
	using clk_t = std::chrono::high_resolution_clock;

	clk_t clock;
	clk_t::time_point epoch;
	clk_t::time_point current;

	Howler() {
		epoch = clock.now();
		restart();
	}

	void restart() {
		current = clock.now();
	}

	std::string relative_timestamp() {
		auto now = clock.now();
		auto count = std::chrono::duration_cast <std::chrono::microseconds> (now - current).count();
		return delta_to_stamp(count);
	}
	
	std::string real_timestamp() {
		auto now = clock.now();
		auto count = std::chrono::duration_cast <std::chrono::microseconds> (now - current).count();
		return delta_to_stamp(count);
	}

	static std::string delta_to_stamp(int64_t delta) {
		int32_t milliseconds = delta / 1000.0;

		int32_t seconds = milliseconds / 1000;
		milliseconds %= 1000;

		int32_t minutes = seconds / 60;
		seconds %= 60;

		return fmt::format("{}:{:02d}:{:03d}", minutes, seconds, milliseconds);
	}
} singleton;

namespace howler {

// Message type colors
static constexpr auto color_reset  = fmt::color::green;
static constexpr auto color_error  = fmt::color::red;
static constexpr auto color_warn   = fmt::color::golden_rod;
static constexpr auto color_info   = fmt::color::teal;
static constexpr auto color_debug  = fmt::color::orange;
static constexpr auto color_fatal  = fmt::color::brown;
static constexpr auto color_assert = fmt::color::magenta;

void relative_time_stamp()
{
	fmt::print(fmt::emphasis::faint, "[{}] ", singleton.relative_timestamp());
}

void triggered_from(const std::source_location &loc)
{
	fmt::print(fmt::emphasis::italic, "...triggered from {}:{}\n", loc.file_name(), loc.line());
	fmt::print(fmt::emphasis::italic, "                  {}\n", loc.function_name());
}

void reset(const std::string &prefix, const std::string &message)
{
	fmt::print(fmt::emphasis::faint | fmt::emphasis::bold, "\n[{}] ", singleton.real_timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_reset) | fmt::emphasis::bold, "#{:<9} ", "reset");
	fmt::println("{}", message);

	singleton.restart();
}

void error(const std::string &prefix,
	   const std::string &message,
	   const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_error) | fmt::emphasis::bold, "#{:<9} ", "error");
	fmt::println("{}", message);
}

void warning(const std::string &prefix,
	     const std::string &message,
	     const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_warn) | fmt::emphasis::bold, "#{:<9} ", "warning");
	fmt::println("{}", message);
}

void info(const std::string &prefix,
	  const std::string &message,
	  const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_info) | fmt::emphasis::bold, "#{:<9} ", "info");
	fmt::println("{}", message);
}

void debug(const std::string &prefix,
	   const std::string &message,
	   const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_debug) | fmt::emphasis::bold, "#{:<9} ", "debug");
	fmt::println("{}", message);
}

void assertion(const std::string &prefix,
	       const std::string &message,
	       const std::source_location &loc)
{
	relative_time_stamp();
	
	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_assert) | fmt::emphasis::bold, "#{:<9} ", "assertion");
	fmt::println("{}", message);

	triggered_from(loc);
	
	breakdown();
}

[[noreturn]]
void fatal(const std::string &prefix,
	   const std::string &message,
	   const std::source_location &loc)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(color_fatal) | fmt::emphasis::bold, "#{:<9} ", "fatal");
	fmt::println("{}", message);
	
	triggered_from(loc);

	breakdown();
}

} // namespace howler
