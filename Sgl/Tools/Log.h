#pragma once

#include <iostream>
#include <format>
#include "SDL/SDL_error.h"

#define PRINT_SDL_ERROR_IF(condition) if(condition) std::cout << std::format("SDL error: {}\n", SDL_GetError())
#define PRINT_SDL_ERROR() std::cout << std::format("SDL error: {}\n", SDL_GetError())
