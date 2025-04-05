#pragma once

#include <span>
#include "SDL/SDL_rect.h"
#include "SDL/SDL_render.h"

namespace Sgl
{
	using PointsView = std::span<const SDL_FPoint>;
	using RectanglesView = std::span<const SDL_FRect>;
	using VerticesView = std::span<const SDL_Vertex>;
}