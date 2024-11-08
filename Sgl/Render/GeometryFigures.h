#pragma once

#include <vector>
#include <ranges>
#include <optional>
#include "../Appearance/Brushes.h"

namespace Sgl
{
	struct Line
	{
		SDL_FPoint Start;
		SDL_FPoint End;
		ColorBrush Color;
	};

	struct Lines
	{
		std::vector<SDL_FPoint> Points;
		ColorBrush Color;
	};

	struct Rectangle
	{
		SDL_FRect Rect;
		ColorBrush Color;
	};

	struct Rectangles
	{
		std::vector<SDL_FRect> Rects;
		ColorBrush Color;
	};

	struct FillRectangle
	{
		SDL_FRect Rect;
		ColorBrush Background;
	};

	struct FillRectangles
	{
		std::vector<SDL_FRect> Rects;
		ColorBrush Background;
	};

	struct TextureRectangle
	{
		SDL_FRect Rect;
		TextureBrush Background;
		std::optional<SDL_Rect> Clip;
	};

	struct Figure
	{
		std::vector<SDL_Vertex> Vertices;
		std::vector<int> RenderOrder;
		TextureBrush Background;
	};

	struct Ellipse
	{
	protected:
		static std::vector<float> CalculateAngles(auto&& function)
		{
			constexpr double Max = 2 * M_PI;
			auto f = [step = Max / (PointNumber - 1)](auto i) { return 0 + step * i; };
			auto result = std::views::iota(0ui64, PointNumber)
						| std::views::transform(f)
						| std::views::transform(function);

			return std::vector<float>(result.begin(), result.end());
		}
		static inline const std::vector<float> Cos_t = CalculateAngles(SDL_cos);
		static inline const std::vector<float> Sin_t = CalculateAngles(SDL_sin);
	public:
		static constexpr size_t PointNumber = 100;

		std::vector<SDL_FPoint> _points = std::vector<SDL_FPoint>(PointNumber);
		ColorBrush Color;
	protected:
		SDL_FPoint _center;
		float _width;
		float _height;
	public:
		Ellipse() noexcept:
			Ellipse(SDL_FPoint{}, 0, 0, Colors::White)
		{}

		Ellipse(SDL_FPoint center, float width, float height, ColorBrush color) noexcept:
			_center(center), _width(width), _height(height), Color(color)
		{
			OnSizeChanged();
		}

		void SetCenterPoint(SDL_FPoint value)
		{
			_center = value;
			OnSizeChanged();
		}

		void SetWidth(float value)
		{
			_width = value;
			OnSizeChanged();
		}

		void SetHeight(float value)
		{
			_height = value;
			OnSizeChanged();
		}

		SDL_FPoint GetCenterPoint() const { return _center; }
		float GetWidth() const { return _width; }
		float GetHeight() const { return _height; }
	private:
		void OnSizeChanged()
		{
			for(size_t i = 0; i < PointNumber; i++)
			{
				_points[i].x = _center.x + _width * Cos_t[i];
				_points[i].y = _center.y + _height * Sin_t[i];
			}
		}
	};
}