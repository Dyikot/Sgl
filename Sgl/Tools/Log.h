#pragma once

#include <iostream>
#include <format>
#include "SDL/SDL_error.h"

class Log
{
public:
	static void PrintSDLError()
	{
		std::cout << std::format("SDL error: {}\n", SDL_GetError());
	}
};