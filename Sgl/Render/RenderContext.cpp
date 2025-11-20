#include "RenderContext.h"
#include <SDL3_image/SDL_image.h>
#include "../Base/Math.h"
#include "../Base/Media/Font.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace Sgl
{
	static constexpr size_t VerticesNumber = 181;
	static constexpr size_t EllipseVerticesNumber = 91;
	static constexpr size_t EllipseAngleStep = 2;

	static const std::vector<float> SinRange = Math::SinRange(VerticesNumber);
	static const std::vector<float> CosRange = Math::CosRange(VerticesNumber);

	RenderContext::RenderContext(SDL_Renderer* renderer):
		_renderer(renderer)
	{}

	void RenderContext::SetTarget(const Texture& texture)
	{
		SDL_SetRenderTarget(_renderer, texture.GetSDLTexture());
	}

	void RenderContext::ResetTarget()
	{
		SDL_SetRenderTarget(_renderer, nullptr);
	}

	void RenderContext::FillBackground(Color color)
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
		SDL_RenderPoint(_renderer, point.x, point.y);
	}

	void RenderContext::DrawPoints(std::span<const FPoint> points, Color color)
	{
		SetColor(color);
		SDL_RenderPoints(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawLine(FPoint start, FPoint end, Color color)
	{
		SetColor(color);
		SDL_RenderLine(_renderer, start.x, start.y, end.x, end.y);
	}

	void RenderContext::DrawLines(std::span<const FPoint> points, Color color)
	{	
		SetColor(color);
		SDL_RenderLines(_renderer, points.data(), points.size());
	}

	void RenderContext::DrawRectange(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderRect(_renderer, &rect);
	}

	void RenderContext::DrawRectangleFill(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderFillRect(_renderer, &rect);
	}

	void RenderContext::DrawRectangles(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderRects(_renderer, rects.data(), rects.size());
	}	

	void RenderContext::DrawRectanglesFill(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderFillRects(_renderer, rects.data(), rects.size());
	}

	void RenderContext::DrawEllipse(FRect rect, Color color)
	{
		std::vector<FPoint> points(VerticesNumber);

		float radiusX = 0.5f * rect.w;
		float radiusY = 0.5f * rect.h;
		FPoint center(rect.x + radiusX, rect.y + radiusY);

		for(size_t i = 0; i < VerticesNumber; i++)
		{
			points[i].x = center.x + radiusX * CosRange[i];
			points[i].y = center.y + radiusY * SinRange[i];
		}

		DrawLines(points, color);
	}

	void RenderContext::DrawEllipseSmooth(FRect rect, Color color)
	{
		if(rect.w <= 0 || rect.h <= 0)
		{
			return;
		}

		int a = rect.w * 0.5;
		int b = rect.h * 0.5;
		int cx = rect.x + a;
		int cy = rect.y + b;

		if(a == 0 || b == 0)
		{
			SetColor(color);
			SDL_RenderLine(_renderer, cx, cy - b, cx, cy + b);
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
			SDL_RenderPoint(_renderer, cx + x, y_int_top);
			SDL_RenderPoint(_renderer, cx - x, y_int_top);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_top);
			SDL_RenderPoint(_renderer, cx + x, y_int_top + 1);
			SDL_RenderPoint(_renderer, cx - x, y_int_top + 1);

			// BOTTOM BOUNDARY (screen coordinates)
			float screen_y_bottom = cy + exact_y;
			int y_int_bottom = (int)floorf(screen_y_bottom);
			float f_bottom = screen_y_bottom - y_int_bottom;
			Uint8 alpha1_bottom = (Uint8)(color.Alpha * (1.0f - f_bottom));
			Uint8 alpha2_bottom = (Uint8)(color.Alpha * f_bottom);

			// Draw bottom boundary (right and left)
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha1_bottom);
			SDL_RenderPoint(_renderer, cx + x, y_int_bottom);
			SDL_RenderPoint(_renderer, cx - x, y_int_bottom);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_bottom);
			SDL_RenderPoint(_renderer, cx + x, y_int_bottom + 1);
			SDL_RenderPoint(_renderer, cx - x, y_int_bottom + 1);

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
			SDL_RenderPoint(_renderer, x_int_right, cy - y);
			SDL_RenderPoint(_renderer, x_int_right, cy + y);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_right);
			SDL_RenderPoint(_renderer, x_int_right + 1, cy - y);
			SDL_RenderPoint(_renderer, x_int_right + 1, cy + y);

			// LEFT BOUNDARY (screen coordinates)
			float screen_x_left = cx - exact_x;
			int x_int_left = (int)floorf(screen_x_left);
			float f_left = screen_x_left - x_int_left;
			Uint8 alpha1_left = (Uint8)(color.Alpha * (1.0f - f_left));
			Uint8 alpha2_left = (Uint8)(color.Alpha * f_left);

			// Draw left boundary (top and bottom)
			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha1_left);
			SDL_RenderPoint(_renderer, x_int_left, cy - y);
			SDL_RenderPoint(_renderer, x_int_left, cy + y);

			SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, alpha2_left);
			SDL_RenderPoint(_renderer, x_int_left + 1, cy - y);
			SDL_RenderPoint(_renderer, x_int_left + 1, cy + y);

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

	static std::vector<Vertex> ComputeEllipseVertices(FRect rect, Color color, bool hasTexture)
	{
		std::vector<Vertex> vertices(EllipseVerticesNumber);

		size_t angleStep = 0;
		float radiusX = rect.w * 0.5f;
		float radiusY = rect.h * 0.5f;
		FPoint center(rect.x + radiusX, rect.y + radiusY);

		if(hasTexture)
		{
			for(size_t i = 0; i < EllipseVerticesNumber; i++)
			{
				float cos = CosRange[angleStep];
				float sin = SinRange[angleStep];
				float u = (cos + 1.0f) * 0.5f;
				float v = (sin + 1.0f) * 0.5f;

				vertices[i].position.x = center.x + radiusX * cos;
				vertices[i].position.y = center.y + radiusY * sin;
				vertices[i].color = color;
				vertices[i].tex_coord = FPoint(u, v);

				angleStep += EllipseAngleStep;
			}
		}
		else
		{
			for(size_t i = 0; i < EllipseVerticesNumber; i++)
			{
				float cos = CosRange[angleStep];
				float sin = SinRange[angleStep];

				vertices[i].position.x = center.x + radiusX * cos;
				vertices[i].position.y = center.y + radiusY * sin;
				vertices[i].color = color;

				angleStep += EllipseAngleStep;
			}
		}

		return vertices;
	}

	void RenderContext::DrawEllipseFill(FRect rect, Color color)
	{
		auto vertices = ComputeEllipseVertices(rect, color, false);
		auto order = Math::TriangulateEllipse(EllipseVerticesNumber - 1);

		DrawGeometry(vertices, order);
	}

	void RenderContext::DrawEllipseFill(FRect rect, const Texture& texture)
	{
		auto color = texture.GetColor();
		auto vertices = ComputeEllipseVertices(rect, color, true);
		auto order = Math::TriangulateEllipse(EllipseVerticesNumber - 1);

		DrawGeometry(vertices, texture, order);
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, std::span<const int> order)
	{
		SDL_RenderGeometry(_renderer, nullptr, 
						   vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, 
									 const Texture& texture, 
									 std::span<const int> order)
	{
		SDL_RenderGeometry(_renderer, texture.GetSDLTexture(),
						   vertices.data(), vertices.size(),
						   order.data(), order.size());
	}

	void RenderContext::DrawTexture(const Texture& texture)
	{
		SDL_RenderTexture(_renderer, texture.GetSDLTexture(), nullptr, nullptr);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target)
	{
		SDL_RenderTexture(_renderer, texture.GetSDLTexture(), nullptr, &target);
	}

	void RenderContext::DrawTexture(const Texture& texture, FRect target, FRect clip)
	{
		SDL_RenderTexture(_renderer, texture.GetSDLTexture(), &clip, &target);
	}

	void RenderContext::DrawTextureRotated(const Texture& texture, double angle, FPoint center)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr, nullptr,
			angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureRotated(const Texture& texture, double angle, FPoint center, FRect target)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr, &target,
			angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureRotated(const Texture& texture, double angle, FPoint center, FRect target, FRect clip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), &clip, &target,
			angle, &center, SDL_FLIP_NONE);
	}

	void RenderContext::DrawTextureFlipped(const Texture& texture, Orientation flip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr, nullptr,
			0, nullptr, SDL_FlipMode(flip));
	}

	void RenderContext::DrawTextureFlipped(const Texture& texture, Orientation flip, FRect target)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr, &target,
			0, nullptr, SDL_FlipMode(flip));
	}

	void RenderContext::DrawTextureFlipped(const Texture& texture, Orientation flip, FRect target, FRect clip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), &clip, &target,
			0, nullptr, SDL_FlipMode(flip));
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr, nullptr,
			angle, &center, SDL_FlipMode(flip));
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip, FRect target)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr,
			&target, angle, &center, SDL_FlipMode(flip));
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, FPoint center, Orientation flip, FRect target, FRect clip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), &clip,
			&target, angle, &center, static_cast<SDL_FlipMode>(flip));
	}

	void RenderContext::DrawText(FPoint position, std::string_view text, size_t size, Color color, const FontFamily& fontFamily)
	{
		FontImpl font(fontFamily, size);
		Texture texture(FontQuality::Blended, font, text, color);
		auto [width, height] = texture.GetSize();
		FRect rect(position.x, position.y, width, height);
		DrawTexture(texture, rect);
	}
}
