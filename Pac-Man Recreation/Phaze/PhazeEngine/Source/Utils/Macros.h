#pragma once

/// \file Macros.h
/// \brief Contains useful macros and other defines for engine use

/// Exports a function for game use
#define PHAZE_API __declspec(dllexport)

/// Exposes a function to lua
#define LuaFunction(...)

/// \brief used for pixels to meter conversion
constexpr float kPixelsPerMeter = 1.f / 32.f;
constexpr float kMetersPerPixel = 32.f;