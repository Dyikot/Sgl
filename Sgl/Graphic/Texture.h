#pragma once

#include <utility>
#include "SDL/SDL_render.h"

namespace Sgl
{
	class Texture
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		explicit Texture(SDL_Texture* texture) noexcept;
		Texture(Texture&& texture) = delete;
		Texture(const Texture&) = delete;
		~Texture() noexcept;

		std::pair<size_t, size_t> Size() const;
		operator SDL_Texture* () const { return _texture; }
	};
}