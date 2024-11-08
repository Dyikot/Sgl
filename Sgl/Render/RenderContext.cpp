#include "RenderContext.h"

namespace Sgl
{
	RenderContext::RenderContext(SDL_Renderer* const renderer) noexcept:
		Renderer(renderer)
	{}

	void RenderContext::Draw(const Line& line) const
	{
		SetRenderColor(line.Color);
		SDL_RenderDrawLineF(Renderer, line.Start.x, line.Start.y, line.End.x, line.End.y);
	}

	void RenderContext::Draw(const Lines& lines) const
	{
		SetRenderColor(lines.Color);
		SDL_RenderDrawLinesF(Renderer, lines.Points.data(), lines.Points.size());
	}

	void RenderContext::Draw(const Rectangle& rectange) const
	{
		SetRenderColor(rectange.Color);
		SDL_RenderDrawRectF(Renderer, &rectange.Rect);
	}

	void RenderContext::Draw(const Rectangles& rectanges) const
	{
		SetRenderColor(rectanges.Color);
		SDL_RenderDrawRectsF(Renderer, rectanges.Rects.data(), rectanges.Rects.size());
	}

	void RenderContext::Draw(const FillRectangle& rectange) const
	{
		SetRenderColor(rectange.Background);
		SDL_RenderFillRectF(Renderer, &rectange.Rect);
	}

	void RenderContext::Draw(const FillRectangles& rectanges) const
	{
		SetRenderColor(rectanges.Background);
		SDL_RenderFillRectsF(Renderer, rectanges.Rects.data(), rectanges.Rects.size());
	}

	void RenderContext::Draw(const TextureRectangle& rectange) const
	{
		SetRenderColor(rectange.Background);
		SDL_RenderCopyF(Renderer,
						rectange.Background.RawTexture,
						rectange.Clip.has_value() ? &rectange.Clip.value() : nullptr, 
						&rectange.Rect);
	}

	void RenderContext::Draw(const Figure& figure) const
	{
		SetRenderColor(figure.Background);
		SDL_RenderGeometry(Renderer, figure.Background.RawTexture,
						   figure.Vertices.data(), figure.Vertices.size(),
						   figure.RenderOrder.data(), figure.RenderOrder.size());
	}

	void RenderContext::Draw(const Ellipse& ellipse) const
	{
		SetRenderColor(ellipse.Color);
		SDL_RenderDrawLinesF(Renderer, ellipse._points.data(), ellipse.PointNumber);
	}

	void RenderContext::DrawSceneBackground(const Brush& background) const
	{
		std::visit([this](const auto& brush) { FillBackground(brush); }, background);
	}

	void RenderContext::FillBackground(const ColorBrush& brush) const noexcept
	{
		SetRenderColor(brush);
		SDL_RenderClear(Renderer);
	}

	void RenderContext::FillBackground(const TextureBrush & brush) const noexcept
	{
		SetRenderColor(brush);
		SDL_RenderCopy(Renderer, brush.RawTexture, nullptr, nullptr);
	}
}
