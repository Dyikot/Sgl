#pragma once

#include <span>
#include <string_view>
#include <variant>
#include <array>
#include "SDL/SDL_render.h"
#include "../Graphic/Color.h"
#include "../Graphic/Texture.h"
#include "../Graphic/Surface.h"
#include "../Data/Nullable.h"

namespace Sgl
{
	class RenderContext
	{
	private:
		SDL_Renderer* _renderer;
		static inline Nullable<RenderContext> _instance;
	public:
		explicit RenderContext(SDL_Renderer* renderer) noexcept;
		~RenderContext() noexcept;

		static Texture CreateTexture(std::string_view path);

		void DrawPoint(SDL_FPoint point, Color color);
		void DrawPoints(std::span<SDL_FPoint> points, Color color);
		void DrawLine(SDL_FPoint start, SDL_FPoint end, Color color);
		void DrawLines(std::span<const SDL_FPoint> points, Color color);
		void DrawRectangle(const SDL_FRect& rectange, Color color);
		void DrawRectangles(std::span<const SDL_FRect> rectanges, Color color);
		void DrawFillRectangle(const SDL_FRect& rectange, Color background);
		void DrawFillRectangles(std::span<const SDL_FRect> rectanges, Color background);
		void DrawImage(std::string_view path, const SDL_FRect& rectangle, Color color);
		void DrawImage(std::string_view path, const SDL_FRect& rectangle, const SDL_Rect& clip, Color color);
		void DrawImage(std::string_view path, SDL_FPoint position, int width, int height, Color color);
		void DrawTexture(const Texture& texture, const SDL_FRect& rectangle, Color color);
		void DrawTexture(const Texture& texture, const SDL_FRect& rectangle, const SDL_Rect& clip, Color color);
		void DrawEllipse(SDL_FPoint position, int width, int height, Color color);
		void DrawEllipseFill(SDL_FPoint position, int width, int height, Color color);
		void DrawShape(std::span<const SDL_Vertex> vertices);
		void DrawShape(std::span<const SDL_Vertex> vertices, const Texture& texture, Color color);
		void DrawShape(std::span<const SDL_Vertex> vertices, std::span<const int> order);
		void DrawShape(std::span<const SDL_Vertex> vertices, std::span<const int> order, const Texture& texture, Color color);
		void FillSceneBackgroundWithColor(Color color);
		void FillSceneBackgroundWithTexture(const Texture& texture, Color color);
		void SetBlendMode(SDL_BlendMode mode);
		
		void SetRenderColor(Color color) const noexcept
		{
			SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
		}

		void SetTextureColor(const Texture& texture, Color color) const noexcept
		{
			SDL_SetTextureColorMod(texture, color.R, color.G, color.B);
			SDL_SetTextureAlphaMod(texture, color.A);
		}

		void SetSurfaceColor(const Surface& surface, Color color) const noexcept
		{
			SDL_SetSurfaceColorMod(surface, color.R, color.G, color.B);
			SDL_SetSurfaceAlphaMod(surface, color.A);
		}

		operator SDL_Renderer* () const
		{ 
			return _renderer; 
		}
	};
}

