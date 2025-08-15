#include "RenderContext.h"
#include <SDL/SDL_image.h>
#include "../Base/Log.h"
#include "../Base/Math.h"
#include "../Base/Time/Stopwatch.h"
#include "../Application.h"

namespace Sgl
{
	static constexpr size_t MaxPointsNumber = 180;
	static const auto SinRange = Math::SinRange<MaxPointsNumber>();
	static const auto CosRange = Math::CosRange<MaxPointsNumber>();

	RenderContext::RenderContext():
		_renderer(App->Window.GetSDLRenderer())
	{}

	void RenderContext::SetTarget(const Texture& texture)
	{
		SDL_SetRenderTarget(_renderer, texture.ToSDLTexture());
	}

	void RenderContext::ResetTarget()
	{
		SDL_SetRenderTarget(_renderer, nullptr);
	}

	void RenderContext::SetBackground(Color color)
	{
		SetColor(color);
		SDL_RenderClear(_renderer);
	}

	void RenderContext::SetBlendMode(SDL_BlendMode mode)
	{
		SDL_SetRenderDrawBlendMode(_renderer, mode);
	}

	void RenderContext::DrawPoint(FPoint point, Color color)
	{
		SetColor(color);
		SDL_RenderDrawPointF(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<const FPoint> points, Color color)
	{
		SetColor(color);
		SDL_RenderDrawPointsF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(FPoint start, FPoint end, Color color)
	{
		SetColor(color);
		SDL_RenderDrawLineF(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<const FPoint> points, Color color)
	{	
		SetColor(color);
		SDL_RenderDrawLinesF(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectange(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectF(_renderer, &rect);
	}

	void RenderContext::DrawRectangleFill(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderFillRectF(_renderer, &rect);
	}

	void RenderContext::DrawRectangles(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderDrawRectsF(_renderer, rects.data(), rects.size());
	}	

	void RenderContext::DrawRectanglesFill(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderFillRectsF(_renderer, rects.data(), rects.size());
	}

	void RenderContext::DrawEllipse(Rect rect, Color color)
	{
		auto points = std::array<FPoint, MaxPointsNumber + 1>();
		auto radiusX = 0.5f * rect.w;
		auto radiusY = 0.5f * rect.h;
		auto center = FPoint(rect.x + radiusX, rect.y + radiusY);

		for(size_t i = 0; i < points.size() - 1; i++)
		{
			points[i].x = center.x + radiusX * CosRange[i];
			points[i].y = center.y + radiusY * SinRange[i];
		}

		points[MaxPointsNumber] = points[0];

		DrawLines(points, color);
	}

	void RenderContext::DrawEllipseSmooth(Rect rect, Color color)
	{
		if(rect.w <= 0 || rect.h <= 0) return;

		int a = rect.w * 0.5;
		int b = rect.h * 0.5;
		int cx = rect.x + a;
		int cy = rect.y + b;

		if(a == 0 || b == 0)
		{
			SetColor(color);
			SDL_RenderDrawLine(_renderer, cx, cy - b, cx, cy + b);
			return;
		}

		long a2 = (long)a * a;
		long b2 = (long)b * b;

		// First octant (0 to 45 degrees)
		long x = 0;
		long y = b;
		long sigma = 2 * b2 + a2 * (1 - 2 * b);

		while(b2 * x <= a2 * y)
		{
			// Calculate exact curve position
			float exact_y = (float)b * sqrtf(1.0f - (float)(x * x) / (float)a2);

			// TOP BOUNDARY (screen coordinates)
			float screen_y_top = cy - exact_y;
			int y_int_top = (int)floorf(screen_y_top);
			float f_top = screen_y_top - y_int_top;
			Uint8 alpha1_top = (Uint8)(color.Alpha * (1.0f - f_top));
			Uint8 alpha2_top = (Uint8)(color.Alpha * f_top);

			// Draw top boundary (right and left)
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha1_top);
			SDL_RenderDrawPoint(_renderer, cx + x, y_int_top);
			SDL_RenderDrawPoint(_renderer, cx - x, y_int_top);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_top);
			SDL_RenderDrawPoint(_renderer, cx + x, y_int_top + 1);
			SDL_RenderDrawPoint(_renderer, cx - x, y_int_top + 1);

			// BOTTOM BOUNDARY (screen coordinates)
			float screen_y_bottom = cy + exact_y;
			int y_int_bottom = (int)floorf(screen_y_bottom);
			float f_bottom = screen_y_bottom - y_int_bottom;
			Uint8 alpha1_bottom = (Uint8)(color.Alpha * (1.0f - f_bottom));
			Uint8 alpha2_bottom = (Uint8)(color.Alpha * f_bottom);

			// Draw bottom boundary (right and left)
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha1_bottom);
			SDL_RenderDrawPoint(_renderer, cx + x, y_int_bottom);
			SDL_RenderDrawPoint(_renderer, cx - x, y_int_bottom);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_bottom);
			SDL_RenderDrawPoint(_renderer, cx + x, y_int_bottom + 1);
			SDL_RenderDrawPoint(_renderer, cx - x, y_int_bottom + 1);

			// Midpoint update (unchanged from original)
			if(sigma >= 0)
			{
				sigma += 4 * a2 * (1 - y);
				y--;
			}
			sigma += 4 * b2 * (x + 1);
			x++;
		}

		// Second octant (45 to 90 degrees)
		x = a;
		y = 0;
		sigma = 2 * a2 + b2 * (1 - 2 * a);

		while(a2 * y <= b2 * x)
		{
			// Calculate exact curve position
			float exact_x = (float)a * sqrtf(1.0f - (float)(y * y) / (float)b2);

			// RIGHT BOUNDARY (screen coordinates)
			float screen_x_right = cx + exact_x;
			int x_int_right = (int)floorf(screen_x_right);
			float f_right = screen_x_right - x_int_right;
			Uint8 alpha1_right = (Uint8)(color.Alpha * (1.0f - f_right));
			Uint8 alpha2_right = (Uint8)(color.Alpha * f_right);

			// Draw right boundary (top and bottom)
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha1_right);
			SDL_RenderDrawPoint(_renderer, x_int_right, cy - y);
			SDL_RenderDrawPoint(_renderer, x_int_right, cy + y);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_right);
			SDL_RenderDrawPoint(_renderer, x_int_right + 1, cy - y);
			SDL_RenderDrawPoint(_renderer, x_int_right + 1, cy + y);

			// LEFT BOUNDARY (screen coordinates)
			float screen_x_left = cx - exact_x;
			int x_int_left = (int)floorf(screen_x_left);
			float f_left = screen_x_left - x_int_left;
			Uint8 alpha1_left = (Uint8)(color.Alpha * (1.0f - f_left));
			Uint8 alpha2_left = (Uint8)(color.Alpha * f_left);

			// Draw left boundary (top and bottom)
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha1_left);
			SDL_RenderDrawPoint(_renderer, x_int_left, cy - y);
			SDL_RenderDrawPoint(_renderer, x_int_left, cy + y);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_left);
			SDL_RenderDrawPoint(_renderer, x_int_left + 1, cy - y);
			SDL_RenderDrawPoint(_renderer, x_int_left + 1, cy + y);

			// Midpoint update (unchanged from original)
			if(sigma >= 0)
			{
				sigma += 4 * b2 * (1 - x);
				x--;
			}
			sigma += 4 * a2 * (y + 1);
			y++;
		}
	}

	static void ComputeEllipseVertices(std::span<Vertex> vertices, 
									   size_t angleStep,
									   Rect rect, 
									   Color color)
	{
		auto angle = 0ull;
		auto radiusX = rect.w * 0.5f;
		auto radiusY = rect.h * 0.5f;
		auto center = FPoint(rect.x + radiusX, rect.y + radiusY);

		auto& first = vertices[0];
		first.position.x = center.x + radiusX * CosRange[0];
		first.position.y = center.y + radiusY * SinRange[0];
		first.color = color;

		for(size_t i = 1; i < vertices.size(); i++)
		{
			angle = (i - 1) * angleStep;
			vertices[i].position.x = center.x + radiusX * CosRange[angle];
			vertices[i].position.y = center.y + radiusY * SinRange[angle];
			vertices[i].color = color;
		}
	}

	void RenderContext::DrawEllipseFill(Rect rect, Color color)
	{
		constexpr auto pointNumber = MaxPointsNumber / 2;
		auto vertices = std::array<Vertex, pointNumber + 1>();

		ComputeEllipseVertices(vertices, MaxPointsNumber / pointNumber, rect, color);
		auto order = Math::TriangulateEllipse(pointNumber);
		DrawGeometry(vertices, order);
	}

	void RenderContext::DrawEllipseFill(Rect rect, const Texture& texture)
	{
		constexpr auto pointNumber = MaxPointsNumber / 2;
		auto vertices = std::array<Vertex, pointNumber + 1>();
		auto color = texture.GetColor();

		ComputeEllipseVertices(vertices, MaxPointsNumber / pointNumber, rect, color);
		auto order = Math::TriangulateEllipse(pointNumber);
		DrawGeometry(vertices, order, &texture);
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, 
									 std::span<const int> order, 
									 const Texture* texture)
	{
		SDL_RenderGeometry(_renderer, texture ? texture->ToSDLTexture() : nullptr,
						   vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawTexture(const Texture& texture)
	{
		SDL_RenderCopyF(_renderer, texture.ToSDLTexture(), nullptr, nullptr);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target)
	{
		SDL_RenderCopyF(_renderer, texture.ToSDLTexture(), nullptr, &target);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target, Rect clip)
	{
		SDL_RenderCopyF(_renderer, texture.ToSDLTexture(), &clip, &target);
	}

	void RenderContext::DrawTextureRotated(const Texture& texture, double angle, 
												FPoint center)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), nullptr, nullptr,
						  angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureRotated(const Texture& texture, double angle, FPoint center, FRect target)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), nullptr, &target,
			angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureRotated(const Texture& texture, double angle, FPoint center, FRect target, Rect clip)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), &clip, &target,
			angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureFlipped(const Texture& texture, Orientation flip)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), nullptr, nullptr,
			0, nullptr, static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureFlipped(const Texture& texture, Orientation flip, FRect target)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), nullptr, &target,
			0, nullptr, static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureFlipped(const Texture& texture, Orientation flip, FRect target, Rect clip)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), &clip, &target,
			0, nullptr, static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle,
													   FPoint center, Orientation flip)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), nullptr, nullptr,
			angle, &center, static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip, FRect target)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), nullptr,
			&target, angle, &center, static_cast<SDL_RendererFlip>(flip));
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip, FRect target, Rect clip)
	{
		SDL_RenderCopyExF(_renderer, texture.ToSDLTexture(), &clip,
			&target, angle, &center, static_cast<SDL_RendererFlip>(flip));
	}
}
