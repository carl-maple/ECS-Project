#pragma once

#include "Types/Types.h"

#include <SDL_pixels.h>

namespace NColor
{
	struct SColor
	{
		constexpr SColor(const uint8 InRed, const uint8 InGreen, const uint8 InBlue, const uint8 InAlpha)
			:Red(InRed), Green(InGreen), Blue(InBlue), Alpha(InAlpha) {}
		uint8 Red;
		uint8 Green;
		uint8 Blue;
		uint8 Alpha;

		SDL_Color GetSDLColor() const
		{
			SDL_Color SDLColor;
			SDLColor.a = Alpha;
			SDLColor.b = Blue;
			SDLColor.g = Green;
			SDLColor.r = Red;
			return SDLColor;
		}
	};

	constexpr SColor Red{ 255, 0, 0, 255 };
	constexpr SColor White{ 255, 255, 255, 255 };
	constexpr SColor Green{ 0, 255, 0, 255 };
	constexpr SColor Magenta{ 255, 0, 255, 255 };
}
