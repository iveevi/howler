#include <chrono>

#include <fmt/color.h>
#include <fmt/printf.h>

#include "howler.hpp"

// Internal logging state
struct State {
	using clk_t = std::chrono::high_resolution_clock;

	clk_t clock;
	clk_t::time_point epoch;
	clk_t::time_point current;

	State() {
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
} state;

namespace howler {

void relative_time_stamp()
{
	fmt::print(fmt::emphasis::faint, "[{}] ", state.relative_timestamp());
}

void reset(const std::string &prefix, const std::string &message)
{
	fmt::print(fmt::emphasis::faint | fmt::emphasis::bold, "\n[{}] ", state.real_timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::green) | fmt::emphasis::bold, "#{:<9} ", "reset");
	fmt::println("{}", message);
	
	state.restart();
}

void error(const std::string &prefix, const std::string &message, const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "#{:<9} ", "error");
	fmt::println("{}", message);
}

void warning(const std::string &prefix, const std::string &message, const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "#{:<9} ", "warning");
	fmt::println("{}", message);
}

void info(const std::string &prefix, const std::string &message, const std::source_location &)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::medium_blue) | fmt::emphasis::bold, "#{:<9} ", "info");
	fmt::println("{}", message);
}

void assertion(const std::string &prefix, const std::string &message, const std::source_location &loc)
{
	relative_time_stamp();
	
	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::purple) | fmt::emphasis::bold, "#{:<9} ", "assertion");
	fmt::println("{}", message);
	fmt::print(fmt::emphasis::italic, "...triggered from {}:{}\n", loc.file_name(), loc.line());
	fmt::print(fmt::emphasis::italic, "                  {}\n", loc.function_name());

	__builtin_trap();
}

void fatal(const std::string &prefix, const std::string &message, const std::source_location &loc)
{
	relative_time_stamp();

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::dark_red) | fmt::emphasis::bold, "#{:<9} ", "fatal");
	fmt::println("{}", message);
	fmt::print(fmt::emphasis::italic, "...triggered from {}:{}\n", loc.file_name(), loc.line());
	fmt::print(fmt::emphasis::italic, "                  {}\n", loc.function_name());

	__builtin_trap();
}

}
