#pragma once

#include <variant>
#include <compare>
#include "Color.h"
#include "../../Render/Texture.h"
#include "../SmartPointers.h"

namespace Sgl
{
	class Brush
	{
	public:
		using Type = size_t;

		static constexpr Type Color = 0;
		static constexpr Type Texture = 1;
	private:
		std::variant<Sgl::Color, Shared<Sgl::Texture>> _value;
	public:
		Brush(Sgl::Color color):
			_value(color)
		{}

		Brush(Shared<Sgl::Texture> texture):
			_value(std::move(texture))
		{}

		Brush(const Brush&) = default;
		Brush(Brush&&) = default;

		template<typename T>
		bool Is() const noexcept
		{
			return std::holds_alternative<T>(_value);
		}

		Type GetType() const noexcept
		{
			return _value.index();
		}

		Sgl::Color AsColor() const
		{
			return std::get<Sgl::Color>(_value);
		}

		const Shared<Sgl::Texture>& AsTexture() const
		{
			return std::get<Shared<Sgl::Texture>>(_value);
		}

		Brush& operator=(Sgl::Color color)
		{
			_value = color;
			return *this;
		}

		Brush& operator=(Shared<Sgl::Texture> texture)
		{
			_value = std::move(texture);
			return *this;
		}

		Brush& operator=(const Brush&) = default;
		Brush& operator=(Brush&&) noexcept = default;
		friend bool operator==(const Brush&, const Brush&) = default;

		explicit operator Sgl::Color() const { return AsColor(); }
		explicit operator Shared<Sgl::Texture>() const { return AsTexture(); }
	};
}