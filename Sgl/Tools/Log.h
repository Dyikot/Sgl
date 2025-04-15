#pragma once

#include <iostream>
#include <format>
#include "SDL/SDL_error.h"

namespace Sgl
{
	inline void PrintSDLErrorIf(bool condition)
	{
		#ifndef NDEBUG
		if(condition)
		{
			std::cout << std::format("SDL error: {}\n", SDL_GetError());
		}
		#endif // NDEBUG
	}

	inline void PrintSDLError()
	{
		#ifndef NDEBUG
		std::cout << std::format("SDL error: {}\n", SDL_GetError());
		#endif // NDEBUG
	}
}
