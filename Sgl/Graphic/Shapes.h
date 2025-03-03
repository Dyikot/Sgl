#pragma once

#include "RenderContext.h"
#include "IVisual.h"
#include "../Math/Math.h"
#include <stdexcept>

namespace Sgl
{
	class ShapeBase: public IVisual
	{
	public:
		SDL_FPoint RotationCenter;
	protected:
		static inline const std::array<float, 361> Sin = Math::ComputeSin<361>();
		static inline const std::array<float, 361> Cos = Math::ComputeCos<361>();
	public:
		ShapeBase(SDL_FPoint rotationCenter = {});

		virtual void Translate(float dx, float dy) = 0;
		virtual void Rotate(size_t degree) = 0;
	protected:
		SDL_FPoint RotatePoint(SDL_FPoint point, SDL_FPoint center, size_t degree);
	};

	class Line: public ShapeBase
	{
	public:
		SDL_FPoint Start;
		SDL_FPoint End;
		Color Color;
	public:
		Line(float x1, float y1, float x2, float y2, Sgl::Color color);
		Line(SDL_FPoint start, SDL_FPoint end, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class Polyline: public ShapeBase
	{
	public:
		std::vector<SDL_FPoint> Points;
		Color Color;
	public:
		Polyline(std::span<SDL_FPoint> points, Sgl::Color color);
		Polyline(std::vector<SDL_FPoint>&& points, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class Polygon: public ShapeBase
	{
	public:
		std::vector<SDL_FPoint> Points;
		Color Color;
	public:
		Polygon(std::span<SDL_FPoint> points, Sgl::Color color);
		Polygon(std::vector<SDL_FPoint>&& points, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class FillPolygon: public ShapeBase
	{
	public:
		std::vector<SDL_Vertex> Vertices;
		std::vector<int> Order;
		Color Color;
	public:
		FillPolygon(std::span<SDL_FPoint> points, Sgl::Color color);
		FillPolygon(std::array<SDL_FPoint, 100>&& points, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class Rectangle: public ShapeBase
	{
	public:
		SDL_FRect Rect;
		Color Color;
	public:
		Rectangle(float x, float y, float width, float height, Sgl::Color color);
		Rectangle(SDL_FPoint position, float width, float height, Sgl::Color color);
		Rectangle(SDL_FRect rect, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class FillRectangle: public ShapeBase
	{
	public:
		SDL_FRect Rect;
		Color Color;
	public:
		FillRectangle(float x, float y, float width, float height, Sgl::Color color);
		FillRectangle(SDL_FPoint position, float width, float height, Sgl::Color color);
		FillRectangle(const SDL_FRect& rect, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class TexturedRectangle: public ShapeBase
	{
	public:
		SDL_FRect Rect;
		const Texture& Texture;
		Color Color;
	public:
		TexturedRectangle(float x, float y,
						  float width, float height,
						  const Sgl::Texture& texture,
						  Sgl::Color color);
		TexturedRectangle(SDL_FPoint position,
						  float width, float height,
						  const Sgl::Texture& texture,
						  Sgl::Color color);
		TexturedRectangle(const SDL_FRect& rect, const Sgl::Texture& texture, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
	};

	class Ellipse: public ShapeBase
	{
	public:
		std::array<SDL_FPoint, 100> Points;
		Color Color;
	public:
		Ellipse(SDL_FPoint position, int width, int height, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class FillEllipse: public ShapeBase
	{
	public:
		std::array<SDL_Vertex, 101> Vertices;
		std::vector<int> Order;
		Sgl::Color Color;
	public:
		FillEllipse(SDL_FPoint position, int width, int height, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};

	class TexturedEllipse: public ShapeBase
	{
	public:
		std::array<SDL_Vertex, 101> Vertices;
		std::vector<int> Order;
		const Sgl::Texture& Texture;
		Color Color;
	public:
		TexturedEllipse(SDL_FPoint position, 
						int width, int height, 
						const Sgl::Texture& texture, Sgl::Color color);

		void OnRender(RenderContext& renderContext) const override;
		void Translate(float dx, float dy) override;
		void Rotate(size_t degree) override;
	};
}