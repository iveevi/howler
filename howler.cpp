#include <chrono>

#include <fmt/color.h>
#include <fmt/printf.h>

#include "howler.hpp"

#ifndef HOWLER_PREFIX
#define HOWLER_PREFIX "howler"
#endif

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

// TODO: use source location (C++20...)
void reset(bool enable_logging_time_stamps)
{
	timer.enable_logging_time_stamps = enable_logging_time_stamps;
	timer.restart();
}

void assertion(const std::string &message)
{
	// TODO: line numbers...?
	fmt::print(fmt::emphasis::underline | fmt::emphasis::bold, HOWLER_PREFIX);
	fmt::print(" ");

	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::fg(fmt::color::purple) | fmt::emphasis::bold, "(assertion) ");
	fmt::print(fmt::emphasis::faint, "{}\n", message);
	__builtin_trap();
}

void fatal(const std::string &message)
{
	// TODO: line numbers...?
	fmt::print(fmt::emphasis::underline | fmt::emphasis::bold, HOWLER_PREFIX);
	fmt::print(" ");

	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::fg(fmt::color::saddle_brown) | fmt::emphasis::bold, "(fatal) ");
	fmt::print(fmt::emphasis::faint, "{}\n", message);
	__builtin_trap();
}

void error(const std::string &message)
{
	fmt::print(fmt::emphasis::underline | fmt::emphasis::bold, HOWLER_PREFIX);
	fmt::print(" ");

	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "(error) ");
	fmt::print(fmt::emphasis::faint, "{}\n", message);
}

void warning(const std::string &message)
{
	fmt::print(fmt::emphasis::underline | fmt::emphasis::bold, HOWLER_PREFIX);
	fmt::print(" ");

	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "(warning) ");
	fmt::print(fmt::emphasis::faint, "{}\n", message);
}

void info(const std::string &message)
{
	fmt::print(fmt::emphasis::underline | fmt::emphasis::bold, HOWLER_PREFIX);
	fmt::print(" ");

	if (timer.enable_logging_time_stamps)
		fmt::print(fmt::emphasis::faint, "[{}] ", timer.timestamp());

	fmt::print(fmt::fg(fmt::color::blue) | fmt::emphasis::bold, "(info) ");
	fmt::print(fmt::emphasis::faint, "{}\n", message);
}

}
