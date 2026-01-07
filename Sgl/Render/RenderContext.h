#pragma once

#include <string_view>
#include <array>
#include <span>

#include "../Base/Delegate.h"
#include "../Base/Primitives.h"
#include "../Base/Media/Color.h"
#include "Texture.h"

namespace Sgl
{
	using Vertex = SDL_Vertex;

	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit RenderContext(SDL_Renderer* renderer);
		
		SDL_Renderer* GetRenderer() const noexcept { return _renderer; }

		void SetTarget(const Texture& texture);
		void ResetTarget();

		void SetClip(Rect clip);
		void ResetClip();

		void FillBackground(Color color);

		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<const FPoint> points, Color color);

		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<const FPoint> points, Color color);

		void DrawRectange(FRect rect, Color color);
		void DrawRectangleFill(FRect rect, Color color);

		void DrawRectangles(std::span<const FRect> rects, Color color);
		void DrawRectanglesFill(std::span<const FRect> rects, Color color);

		void DrawEllipse(FRect rect, Color color);
		void DrawEllipseSmooth(FRect rect, Color color);
		void DrawEllipseFill(FRect rect, Color color);
		void DrawEllipseFill(FRect rect, const Texture& texture);

		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order = {});
		void DrawGeometry(std::span<const Vertex> vertices, const Texture& texture, std::span<const int> order = {});

		void DrawTexture(const Texture& texture);
		void DrawTexture(const Texture& texture, FRect target);
		void DrawTexture(const Texture& texture, FRect target, FRect clip);

		void DrawTextureTransformed(const Texture& texture, double angle, const FPoint* center, SDL_FlipMode flip);
		void DrawTextureTransformed(const Texture& texture, double angle, const FPoint* center, SDL_FlipMode flip, FRect target);
		void DrawTextureTransformed(const Texture& texture, double angle, const FPoint* center, SDL_FlipMode flip, FRect target, FRect clip);

		void DrawText(FPoint position, std::string_view text, float size, Color color, const FontFamily& fontFamily = FontFamily::Default);
	private:
		void SetColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}
	};
}