#include <chrono>

#include <fmt/color.h>
#include <fmt/printf.h>

#include "howler.hpp"

using clk_t = std::chrono::high_resolution_clock;

struct {
	clk_t clock;
	clk_t::time_point epoch;

	bool enable_logging_time_stamps;

	void restart() {
		epoch = clock.now();
	}

	std::string timestamp() {
		auto now = clock.now();
		auto count = std::chrono::duration_cast <std::chrono::microseconds> (now - epoch).count();

		int32_t milliseconds = count / 1000.0;

		int32_t seconds = milliseconds / 1000;
		milliseconds %= 1000;

		int32_t minutes = seconds / 60;
		seconds %= 60;

		return fmt::format("{}:{:02d}:{:03d}", minutes, seconds, milliseconds);
	}
} timer;

namespace howler {

void reset(bool enable_logging_time_stamps)
{
	timer.enable_logging_time_stamps = enable_logging_time_stamps;
	timer.restart();
}

void assertion(const std::string &prefix, const std::string &message, const std::source_location &loc)
{
	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::purple) | fmt::emphasis::bold, "#assertion ");
	fmt::println("{}", message);
	fmt::print(fmt::emphasis::italic, "...triggered from {}:{}\n", loc.file_name(), loc.line());
	fmt::print(fmt::emphasis::italic, "                  {}\n", loc.function_name());
	__builtin_trap();
}

void fatal(const std::string &prefix, const std::string &message, const std::source_location &loc)
{
	// TODO: line numbers...?
	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::dark_red) | fmt::emphasis::bold, "#fatal ");
	fmt::println("{}", message);
	fmt::print(fmt::emphasis::italic, "...triggered from {}:{}\n", loc.file_name(), loc.line());
	fmt::print(fmt::emphasis::italic, "                  {}\n", loc.function_name());
	__builtin_trap();
}

void error(const std::string &prefix, const std::string &message, const std::source_location &)
{
	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "#error ");
	fmt::println("{}", message);
}

void warning(const std::string &prefix, const std::string &message, const std::source_location &)
{
	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "#warning ");
	fmt::println("{}", message);
}

void info(const std::string &prefix, const std::string &message, const std::source_location &)
{
	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::emphasis::bold, "@{} ", prefix);
	fmt::print(fmt::fg(fmt::color::medium_blue) | fmt::emphasis::bold, "#info ");
	fmt::println("{}", message);
}

}
