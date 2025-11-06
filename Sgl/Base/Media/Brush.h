#pragma once

#include <variant>
#include <compare>
#include "Color.h"
#include "../../Render/Texture.h"

namespace Sgl
{
	enum class BrushType
	{
		Color, Texture
	};

	class Brush
	{
	private:
		std::variant<Color, Texture> _value;
	public:
		Brush(Color color):
			_value(color)
		{}

		Brush(Texture texture):
			_value(std::move(texture))
		{}

		Brush(const Brush&) = default;
		Brush(Brush&&) = default;

		template<typename T>
		bool Is() const noexcept
		{
			return std::holds_alternative<T>(_value);
		}

		BrushType GetType() const noexcept
		{
			return static_cast<BrushType>(_value.index());
		}

		Color AsColor() const
		{
			return std::get<Color>(_value);
		}

		const Texture& AsTexture() const
		{
			return std::get<Texture>(_value);
		}

		Brush& operator=(Color color)
		{
			_value = color;
			return *this;
		}

		Brush& operator=(Texture texture)
		{
			_value = std::move(texture);
			return *this;
		}

		Brush& operator=(const Brush&) = default;
		Brush& operator=(Brush&&) noexcept = default;
		friend bool operator==(const Brush&, const Brush&) = default;

		explicit operator Color() const { return AsColor(); }
		explicit operator Texture() const { return AsTexture(); }
	};
}