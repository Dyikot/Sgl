#pragma once

#include "Drawing.h"
#include <vector>
#include <ranges>
#include <optional>

namespace Sgl
{
	class Line: public Drawing
	{
	public:
		SDL_FPoint Start;
		SDL_FPoint End;
		ColorBrush Color;
	public:
		Line() noexcept;
		Line(const SDL_FPoint& start, const SDL_FPoint& end, ColorBrush color) noexcept;

		void Draw() const override
		{
			SetRenderColor(Color);
			SDL_RenderDrawLineF(Renderer, Start.x, Start.y, End.x, End.y);
		}
	};

	class Lines: public Drawing
	{
	public:
		ColorBrush Color;
		std::vector<SDL_FPoint> Points;
	public:
		Lines() noexcept;
		Lines(const std::vector<SDL_FPoint>& points, ColorBrush color) noexcept;
		Lines(std::vector<SDL_FPoint>&& points, ColorBrush color) noexcept;

		void Draw() const override
		{
			SetRenderColor(Color);
			SDL_RenderDrawLinesF(Renderer, Points.data(), Points.size());
		}
	};

	class Rectangle: public Drawing
	{
	public:
		SDL_FRect Properties;
		ColorBrush Color;
	public:
		Rectangle() noexcept;
		Rectangle(const SDL_FRect& properties, ColorBrush color) noexcept;

		void Draw() const override
		{
			SetRenderColor(Color);
			SDL_RenderDrawRectF(Renderer, &Properties);
		}
	};

	class Rectangles: public Drawing
	{
	public:
		std::vector<SDL_FRect> Properties;
		ColorBrush Color;
	public:
		Rectangles() noexcept;
		Rectangles(const std::vector<SDL_FRect>& properties, ColorBrush color) noexcept;
		Rectangles(std::vector<SDL_FRect>&& properties, ColorBrush color) noexcept;

		void Draw() const override
		{
			SetRenderColor(Color);
			SDL_RenderDrawRectsF(Renderer, Properties.data(), Properties.size());
		}
	};

	class FillRectangle: public Drawing
	{
	public:
		SDL_FRect Properties;
		ColorBrush Background;
	public:
		FillRectangle() noexcept;
		FillRectangle(const SDL_FRect& properties, ColorBrush background) noexcept;

		void Draw() const override
		{
			SetRenderColor(Background);
			SDL_RenderFillRectF(Renderer, &Properties);
		}
	};

	class FillRectangles: public Drawing
	{
	public:
		std::vector<SDL_FRect> Properties;
		ColorBrush Color;
	public:
		FillRectangles() noexcept;
		FillRectangles(const std::vector<SDL_FRect>& properties, ColorBrush color) noexcept;
		FillRectangles(std::vector<SDL_FRect>&& properties, ColorBrush color) noexcept;

		void Draw() const override
		{
			SetRenderColor(Color);
			SDL_RenderFillRectsF(Renderer, Properties.data(), Properties.size());
		}
	};

	class TextureRectangle: public Drawing
	{
	public:
		SDL_FRect Properties;
		SDL_Texture* Texture;
		std::optional<SDL_Rect> Clip;
	public:
		TextureRectangle() noexcept;
		TextureRectangle(const SDL_FRect& properties, SDL_Texture* texture) noexcept;
		TextureRectangle(const SDL_FRect& properties, 
						 const SDL_Rect& clip,
						 SDL_Texture* texture) noexcept;

		void Draw() const override
		{
			SDL_RenderCopyF(Renderer, Texture,
							Clip.has_value() ? &Clip.value() : nullptr, &Properties);
		}
	};

	class Figure: public Drawing
	{
	public:
		std::vector<SDL_Vertex> Vertices;
		std::vector<int> RenderOrder;
		SDL_Texture* FigureTexture = nullptr;
	public:
		Figure() noexcept;
		Figure(const std::vector<SDL_Vertex>& vertices,
			   const std::vector<int>& renderOrder,
			   SDL_Texture* texture) noexcept;
		Figure(std::vector<SDL_Vertex>&& vertices,
			   const std::vector<int>& renderOrder,
			   SDL_Texture* texture) noexcept;

		void Draw() const override
		{
			SDL_RenderGeometry(Renderer, FigureTexture,
							   Vertices.data(), Vertices.size(),
							   RenderOrder.data(), RenderOrder.size());
		}
	};

	class Ellipse: public Drawing
	{
	public:
		ColorBrush Color;
	private:
		static std::vector<float> CalculateAngles(auto function)
		{
			constexpr double Max = 2 * M_PI;
			auto f = [step = Max / (PointNumber - 1)](auto i) { return 0 + step * i; };
			auto result = std::views::iota(size_t(0), PointNumber)
				| std::views::transform(f)
				| std::views::transform(function);

			return std::vector<float>(result.begin(), result.end());
		}
		static constexpr size_t PointNumber = 100;
		static inline const std::vector<float> Cos_t = CalculateAngles(SDL_cos);
		static inline const std::vector<float> Sin_t = CalculateAngles(SDL_sin);

		std::vector<SDL_FPoint> _points = std::vector<SDL_FPoint>(PointNumber);
		SDL_FPoint _center;
		float _width;
		float _height;
	public:
		Ellipse() noexcept;
		Ellipse(const SDL_FPoint& center, 
				float width, 
				float height, 
				ColorBrush color) noexcept;

		void SetCenterPoint(const SDL_FPoint& value);
		void SetWidth(float value);
		void SetHeight(float value);

		const SDL_FPoint& GetCenterPoint() const { return _center; }
		float GetWidth() const { return _width; }
		float GetHeight() const { return _height; }

		void Draw() const override
		{			
			SetRenderColor(Color);
			SDL_RenderDrawLinesF(Renderer, _points.data(), PointNumber);
		}

	private:
		void OnSizeChanged();
	};
}