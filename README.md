<p align="center">
	<img src="media/logo.png" width="256">
</p>

# Howler

## Overview

Howler is a lightweight logging system for C++20 that provides different levels of logging messages, including assertions, fatal errors, errors, warnings, and informational messages. It leverages `fmt` for formatted output and `std::source_location` for capturing source file details automatically.

## Features

- Provides formatted logging messages with optional timestamps.
- Supports different levels of logging: assertion, fatal, error, warning, and info.
- Captures source location (file, line, function) for assertions and fatal errors.
- Uses `fmt` for efficient string formatting and colored terminal output.
- `__builtin_trap()` is used for assertion failures and fatal errors to halt execution.

## Building

Howler relies on the `fmt` printing library to format and style terminal messages. We recommend users to use Howler via CMake, e.g. with `add_subdirectory(.../howler)`. To this end, if the root project already has an instance of `fmt`, users can add `set(HOWLER_FMT_EXTERNAL TRUE)` to their CMake config to tell Howler not to use its local `fmt` repository.

Finally, simply include `howler.hpp` in your source file:

```cpp
#include "howler.hpp"

// ...
```

## Usage

### Initialization

Before logging, you can enable or disable timestamps:

```cpp
howler::reset(true);  // Enable timestamps
```

### Logging Macros

#### Assertions

```cpp
howl_assert(condition, "Message: {}", value);
```

Triggers an assertion failure if `condition` is false and prints the message
along with the source file details.

#### Fatal Errors

```cpp
howl_fatal("Critical failure: {}", error_code);
```

Logs a fatal error and terminates execution.

#### Errors

```cpp
howl_error("An error occurred: {}", error_code);
```

Logs an error message without terminating execution.

#### Warnings

```cpp
howl_warning("Warning: {}", warning_message);
```

Logs a warning message.

#### Information

```cpp
howl_info("Info: {}", info_message);
```

Logs an informational message.