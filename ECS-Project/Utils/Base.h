#pragma once

#include "Types/Types.h"
#include "Types/Vector2f.h"
#include "Types/Vector2i.h"

#include <string>
#include <memory>
#include <math.h>

#include <SDL_log.h>


template<class T>
const T& clamp(const T& Value, const T& Upper, const T& Lower) {
	return min(Upper, max(Value, Lower));
}

constexpr uint32 MILLISECONDS_TO_SECONDS_VALUE = 1000;
constexpr uint32 MillisecondsToSeconds(const uint32 Milliseconds)
{
	return Milliseconds / MILLISECONDS_TO_SECONDS_VALUE;
}

constexpr uint32 SecondsToMilliSeconds(const uint32 Seconds)
{
	return Seconds * MILLISECONDS_TO_SECONDS_VALUE;
}