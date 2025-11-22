#pragma once

#include <string>
#include <variant>
#include "../../Render/Texture.h"
#include "../Ref.h"

namespace Sgl
{
	class ImageBrush
	{
	private:
		mutable Ref<Texture> _texture;
		std::string _source;
	public:
		ImageBrush() noexcept = default;
		explicit ImageBrush(std::string source) noexcept;
		ImageBrush(const ImageBrush& other) noexcept;
		ImageBrush(ImageBrush&& other) noexcept;

		void SetSource(const std::string& value);
		const std::string& GetSource() const { return _source; }

		const Texture& GetTexture() const;
		const Texture& GetTexture(SDL_Renderer* renderer) const;

		ImageBrush& operator=(const ImageBrush& other);
		ImageBrush& operator=(ImageBrush&& other) noexcept;
		friend bool operator==(const ImageBrush& left, const ImageBrush& right);
	};

	using Brush = std::variant<Color, ImageBrush>;
}