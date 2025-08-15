#pragma once

#include <string_view>
#include <array>
#include <span>
#include <SDL/SDL_render.h>
#include "../Base/Delegate.h"
#include "../Base/Primitives.h"
#include "../Base/Media/Color.h"
#include "../Base/Media/Layout.h"
#include "Texture.h"

namespace Sgl
{
	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
	public:
		RenderContext();
		
		void SetTarget(const Texture& texture);
		void ResetTarget();

		void SetBackground(Color color);
		void SetBlendMode(SDL_BlendMode mode);

		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<const FPoint> points, Color color);

		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<const FPoint> points, Color color);

		void DrawRectange(FRect rect, Color color);
		void DrawRectangleFill(FRect rect, Color color);

		void DrawRectangles(std::span<const FRect> rects, Color color);
		void DrawRectanglesFill(std::span<const FRect> rects, Color color);

		void DrawEllipse(Rect rect, Color color);
		void DrawEllipseSmooth(Rect rect, Color color);
		void DrawEllipseFill(Rect rect, Color color);
		void DrawEllipseFill(Rect rect, const Texture& texture);

		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order = {}, const Texture* texture = nullptr);

		void DrawTexture(const Texture& texture);
		void DrawTexture(const Texture& texture, FRect target);
		void DrawTexture(const Texture& texture, FRect target, Rect clip);

		void DrawTextureRotated(const Texture& texture, double angle, FPoint center);
		void DrawTextureRotated(const Texture& texture, double angle, FPoint center, FRect target);
		void DrawTextureRotated(const Texture& texture, double angle, FPoint center, FRect target, Rect clip);
		
		void DrawTextureFlipped(const Texture& texture, Orientation flip);
		void DrawTextureFlipped(const Texture& texture, Orientation flip, FRect target);
		void DrawTextureFlipped(const Texture& texture, Orientation flip, FRect target, Rect clip);

		void DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip);
		void DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip, FRect target);
		void DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip, FRect target, Rect clip);
	private:
		void SetColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}
	};
}

