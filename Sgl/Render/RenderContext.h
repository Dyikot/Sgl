#pragma once

#include <string_view>
#include <array>
#include <span>
#include <SDL/SDL_render.h>
#include "../Base/Delegate.h"
#include "../Base/Primitives.h"
#include "Surface.h"
#include "Texture.h"

namespace Sgl
{
	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
	public:
		explicit RenderContext(SDL_Renderer* renderer) noexcept;
		
		void SetTarget(const Texture& texture);
		void ResetTarget();

		void DrawPoint(FPoint point, Color color);
		void DrawPoints(std::span<const FPoint> points, Color color);

		void DrawLine(FPoint start, FPoint end, Color color);
		void DrawLines(std::span<const FPoint> points, Color color);

		void DrawRectange(FRect rect, Color color);
		void DrawRectangles(std::span<const FRect> rects, Color color);

		void DrawFillRectangle(FRect rect, Color fill);
		void DrawFillRectangles(std::span<const FRect> rects, Color fill);

		void DrawTexture(const Texture& texture);
		void DrawTexture(const Texture& texture, Color fill);
		void DrawTexture(const Texture& texture, FRect target);
		void DrawTexture(const Texture& texture, FRect target, Color fill);
		void DrawTexture(const Texture& texture, FRect target, Rect source);
		void DrawTexture(const Texture& texture, FRect target, Rect source, Color fill);

		void DrawTextureWithRotation(const Texture& texture, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, Color fill, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, FRect target, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, FRect target, Color fill, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, FRect target, Rect source, double angle, FPoint center);
		void DrawTextureWithRotation(const Texture& texture, FRect target, Rect source, Color fill, double angle, FPoint center);
		
		void DrawTextureWithFlip(const Texture& texture, TextureFlip flip);
		void DrawTextureWithFlip(const Texture& texture, Color fill, TextureFlip flip);
		void DrawTextureWithFlip(const Texture& texture, FRect target, TextureFlip flip);
		void DrawTextureWithFlip(const Texture& texture, FRect target, Color fill, TextureFlip flip);
		void DrawTextureWithFlip(const Texture& texture, FRect target, Rect source, TextureFlip flip);
		void DrawTextureWithFlip(const Texture& texture, FRect target, Rect source, Color fill, TextureFlip flip);

		void DrawTextureWithRotationAndFlip(const Texture& texture, double angle, FPoint center, TextureFlip flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, Color fill, double angle, FPoint center, TextureFlip flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, FRect target, double angle, FPoint center, TextureFlip flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, FRect target, Color fill, double angle, FPoint center, TextureFlip flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, FRect target, Rect source, double angle, FPoint center, TextureFlip flip);
		void DrawTextureWithRotationAndFlip(const Texture& texture, FRect target, Rect source, Color fill, double angle, FPoint center, TextureFlip flip);

		void DrawCircle(FPoint position, int diameter, Color color);
		void DrawCircleFill(FPoint position, int diameter, Color color);
		void DrawCircleFill(FPoint position, int diameter, const Texture& texture, Color color);

		void DrawEllipse(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(FPoint position, int width, int height, Color color);
		void DrawEllipseFill(FPoint position, int width, int height, const Texture& texture, Color color);

		void DrawGeometry(std::span<const Vertex> vertices);
		void DrawGeometry(std::span<const Vertex> vertices, const Texture& texture);
		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order);
		void DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order, const Texture& texture);

		void FillBackground(Color color);
		void SetBlendMode(SDL_BlendMode mode);
	private:
		void SetColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
		}
	};

	using RenderFragment = Action<RenderContext>;
}

