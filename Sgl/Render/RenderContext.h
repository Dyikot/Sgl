#pragma once

#include <string_view>
#include <array>
#include <span>
#include <SDL/SDL_render.h>
#include "../Base/Delegate.h"
#include "../Base/Primitives.h"
#include "../Base/Media/Color.h"
#include "../Base/Media/Layout.h"

namespace Sgl
{
	class Texture;	

	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit RenderContext(SDL_Renderer* renderer) noexcept;
		
		void SetTarget(const Texture& texture);
		void ResetTarget();

		void SetBackground(Color color);
		void SetBlendMode(SDL_BlendMode mode);

		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<const FPoint> points, Color color);

		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<const FPoint> points, Color color);

		void DrawRectange(FRect rect, Color color);
		void DrawRectangleFill(FRect rect, Color fill);

		void DrawRectangles(std::span<const FRect> rects, Color color);
		void DrawRectanglesFill(std::span<const FRect> rects, Color fill);

		void DrawEllipse(Rect rect, Color color);
		void DrawEllipseSmooth(Rect rect, Color color);
		void DrawEllipseFill(Rect rect, Color color);
		void DrawEllipseFill(Rect rect, const Texture& texture, Color color);

		void DrawGeometry(std::span<const Vertex> vertices);
		void DrawGeometry(std::span<const Vertex> vertices, const Texture& texture);
		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order);
		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order, const Texture& texture);

		void DrawTexture(const Texture& texture);
		void DrawTexture(const Texture& texture, FRect target);
		void DrawTexture(const Texture& texture, FRect target, Rect clip);

		void DrawTextureWithRotation(const Texture& texture, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, FRect target, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, FRect target, Rect clip, double angle, FPoint center);
		
		void DrawTextureWithFlip(const Texture& texture, Orientation flip);
		void DrawTextureWithFlip(const Texture& texture, FRect target, Orientation flip);
		void DrawTextureWithFlip(const Texture& texture, FRect target, Rect clip, Orientation flip);

		void DrawTextureWithRotationAndFlip(const Texture& texture, double angle, FPoint center, Orientation flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, FRect target, double angle, FPoint center, Orientation flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, FRect target, Rect clip, double angle, FPoint center, Orientation flip);
	private:
		void SetColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}
	};

	using RenderFragment = Action<RenderContext>;
}

