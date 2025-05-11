#pragma once

#include <unordered_map>
#include <string_view>
#include "SDL/SDL_render.h"
#include "../Data/Any.h"

namespace Sgl
{
	class ResourcesManager: public std::unordered_map<std::string_view, Any>
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit ResourcesManager(SDL_Renderer* renderer):
			_renderer(renderer)
		{}

		void AddTexture(std::string_view key, std::string_view path);
	};
}