#pragma once

#include <variant>
#include "../Base/Ref.h"
#include "../Base/Delegate.h"
#include "RenderContext.h"

namespace Sgl
{
	class TextureSource
	{
	private:
		mutable std::variant<Ref<Texture>, std::string_view> _texture;
	public:
		explicit TextureSource(std::string_view filePath) noexcept;
		TextureSource(const TextureSource& other) noexcept;
		TextureSource(TextureSource&& other) noexcept;

		bool IsCreated() const noexcept;
		const Ref<Texture>& GetTexture() const;
		const Ref<Texture>& CreateTexture(RenderContext context) const;

		TextureSource& operator=(const TextureSource& other);
		TextureSource& operator=(TextureSource&& other) noexcept;
		bool operator==(const TextureSource&) const = default;
		bool operator==(const Texture& texture) const;
	};

	using Brush = std::variant<Color, TextureSource>;

	constexpr bool operator==(const Brush& brush, Color color)
	{
		return brush.index() == 0 && std::get<Color>(brush) == color;
	}

	constexpr bool operator==(const Brush& brush, const Texture& texture)
	{
		return brush.index() == 1 && std::get<TextureSource>(brush) == texture;
	}
}