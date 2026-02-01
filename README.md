This README was written by an LLM.

# Howler logging utilities

`howler.hpp` provides a small logging layer with colored message types,
configurable formatting options, and a set of convenience macros.

## Quick start

```cpp
#define HOWLER_IMPL
#include "howler.hpp"

howl_info("Hello {}", "world");
howl_warning("Low disk: {}%", 12);
```

## Message types

The logging API categorizes output by `howler::MessageType`:

- `eReset`
- `eError`
- `eWarning`
- `eInfo`
- `eDebug`
- `eFatal`
- `eAssertion`

Each type maps to a color/style entry in the palette. You can customize styles
via `howler::set_palette`.

## Options

Logging formatting and behavior are controlled by `howler::Options` and
`howler::set_option`:

- `eShowTimeStamp` (default: on)
- `eShowSourceLocation` (default: off)
- `eShowSourceLocationForFatal` (default: on)
- `eShowSourceLocationForAssertion` (default: on)
- `eBreakpointOnAssertion` (default: on)

The defaults are set inside `HOWLER_IMPL`.

```cpp
howler::set_option(howler::Options::eShowSourceLocation, true);
howler::set_option(howler::Options::eBreakpointOnAssertion, false);
```

## Basic macros

By default, all macros use the `HOWLER_PREFIX` (defaults to `"howler"`):

```cpp
howl_error("Failed to open {}", path);
howl_info("Connected to {}", host);
```

`howl_fatal` logs and then calls `std::abort()` after triggering the internal
breakdown handler.

## Assertions

Assertion helpers log with the `eAssertion` type when the condition is false:

```cpp
howl_assert(ptr != nullptr, "ptr must not be null");
howl_assert_plain(ptr != nullptr);
```

If `eBreakpointOnAssertion` is enabled, assertions trigger a breakpoint handler
after logging.

## Prefixed logging

When you need a custom prefix per message, use the `_prefixed` variants:

```cpp
howl_info_prefixed("net", "Connected to {}", host);
howl_error_prefixed("db", "Query failed: {}", err);
```

## Custom palettes

Use `howler::set_palette` to change per-type styling.

```cpp
howler::set_palette(howler::MessageType::eWarning,
                    fmt::emphasis::bold | fmt::fg(fmt::color::orange));
```

## Implementation note

Define `HOWLER_IMPL` in exactly one translation unit before including
`howler.hpp` to enable the implementation.
