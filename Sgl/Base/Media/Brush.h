#pragma once

#include <variant>
#include "Color.h"
#include "../../Render/Texture.h"

namespace Sgl
{
	using Brush = std::variant<Color, Texture>;
}