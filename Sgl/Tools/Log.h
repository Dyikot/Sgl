#pragma once

#include <iostream>
#include <format>
#include "SDL/SDL_error.h"

namespace Sgl
{
	#define PrintSDLError() std::cout << std::format("SDL error: {}\n", SDL_GetError());
}