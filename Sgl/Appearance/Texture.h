#pragma once

#include "Color.h"
#include <utility>

namespace Sgl
{
	class Texture
	{
	private:
		SDL_Texture* _texture = nullptr;
	public:
		Color Color = Colors::White;
	public:
		Texture() = delete;
		Texture(SDL_Texture* texture) noexcept;
		Texture(Texture&& texture) noexcept;
		Texture(const Texture&) = delete;
		~Texture() noexcept;

		std::pair<int, int> Size() const;
		Texture& operator=(SDL_Texture* texture);
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& texture) noexcept;
		operator SDL_Texture* () const { return _texture; }
	};
}