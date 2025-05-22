#pragma once

#include <unordered_map>
#include <string_view>
#include "SDL/SDL_render.h"
#include "Any.h"
#include "../Graphic/Primitives.h"
#include "../Graphic/Renderer.h"

namespace Sgl
{
	class ResourcesMap: public std::unordered_map<std::string_view, Any>
	{
	private:
		Renderer& _renderer;
	public:
		explicit ResourcesMap(Renderer& renderer):
			_renderer(renderer)
		{}

		void AddTexture(std::string_view key, std::string_view path);
	};
}