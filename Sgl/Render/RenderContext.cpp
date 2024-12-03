#include "RenderContext.h"
#include "../Tools/Log.h"
#include <SDL/SDL_image.h>

namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* const renderer) noexcept:
		_renderer(renderer)
	{}

	void RenderContext::Draw(const Line& line) const
	{
		SetRenderColor(line.Color);
		SDL_RenderDrawLineF(_renderer, line.Start.x, line.Start.y, line.End.x, line.End.y);
	}

	void RenderContext::Draw(const Lines& lines) const
	{
		SetRenderColor(lines.Color);
		SDL_RenderDrawLinesF(_renderer, lines.Points.data(), lines.Points.size());
	}

	void RenderContext::Draw(const Rectangle& rectange) const
	{
		SetRenderColor(rectange.Color);
		SDL_RenderDrawRectF(_renderer, &rectange.Rect);
	}

	void RenderContext::Draw(const Rectangles& rectanges) const
	{
		SetRenderColor(rectanges.Color);
		SDL_RenderDrawRectsF(_renderer, rectanges.Rects.data(), rectanges.Rects.size());
	}

	void RenderContext::Draw(const FillRectangle& rectange) const
	{
		SetRenderColor(rectange.Background);
		SDL_RenderFillRectF(_renderer, &rectange.Rect);
	}

	void RenderContext::Draw(const FillRectangles& rectanges) const
	{
		SetRenderColor(rectanges.Background);
		SDL_RenderFillRectsF(_renderer, rectanges.Rects.data(), rectanges.Rects.size());
	}

	void RenderContext::Draw(const TextureRectangle& rectange) const
	{
		SetRenderColor(rectange.Background);
		SDL_RenderCopyF(_renderer,
						rectange.Background.RawTexture,
						rectange.Clip.has_value() ? &rectange.Clip.value() : nullptr, 
						&rectange.Rect);
	}

	void RenderContext::Draw(const Figure& figure) const
	{
		SetRenderColor(figure.Background);
		SDL_RenderGeometry(_renderer, figure.Background.RawTexture,
						   figure.Vertices.data(), figure.Vertices.size(),
						   figure.RenderOrder.data(), figure.RenderOrder.size());
	}

	void RenderContext::Draw(const Ellipse& ellipse) const
	{
		SetRenderColor(ellipse.Color);
		SDL_RenderDrawLinesF(_renderer, ellipse._points.data(), ellipse.PointNumber);
	}

	void RenderContext::DrawSceneBackground(const Brush& background) const
	{
		std::visit([this](const auto& brush) { SetSceneBackground(brush); }, background);
	}

	void RenderContext::SetBlendMode(SDL_BlendMode blendMode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, blendMode);
	}

	SDL_Texture* RenderContext::CreateTexture(std::string_view path) const
	{
		auto texture = IMG_LoadTexture(_renderer, path.data());
		if(texture == nullptr)
		{
			PrintSDLError();
		}

		return texture;
	}

	void RenderContext::SetSceneBackground(const ColorBrush& brush) const noexcept
	{
		SetRenderColor(brush);
		SDL_RenderClear(_renderer);
	}

	void RenderContext::SetSceneBackground(const TextureBrush & brush) const noexcept
	{
		SetRenderColor(brush);
		SDL_RenderCopy(_renderer, brush.RawTexture, nullptr, nullptr);
	}
}
