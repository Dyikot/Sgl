#include "RenderContext.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "../Base/Math.h"
#include "../Base/Media/Font.h"

namespace Sgl
{
	static constexpr size_t VerticesNumber = 181;
	static constexpr size_t EllipseVerticesNumber = 91;
	static constexpr size_t EllipseAngleStep = 2;

	static const std::vector<float> SinRange = Math::SinRange(VerticesNumber);
	static const std::vector<float> CosRange = Math::CosRange(VerticesNumber);

	RenderContext::RenderContext(SDL_Renderer* _renderer):
		_renderer(_renderer)
	{}

	void RenderContext::SetTarget(const Texture& texture)
	{
		SDL_SetRenderTarget(_renderer, texture.GetSDLTexture());
	}

	void RenderContext::ResetTarget()
	{
		SDL_SetRenderTarget(_renderer, nullptr);
	}

	void RenderContext::SetClip(Rect clip)
	{
		SDL_SetRenderClipRect(_renderer, &clip);
	}

	void RenderContext::ResetClip()
	{
		SDL_SetRenderClipRect(_renderer, nullptr);
	}

	void RenderContext::FillBackground(Color color)
	{
		SetColor(color);
		SDL_RenderClear(_renderer);
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

		const float radiusX = 0.5f * rect.w;
		const float radiusY = 0.5f * rect.h;
		const FPoint center(rect.x + radiusX, rect.y + radiusY);

		for(size_t i = 0; i < VerticesNumber; i++)
		{
			points[i].x = center.x + radiusX * CosRange[i];
			points[i].y = center.y + radiusY * SinRange[i];
		}

		DrawLines(points, color);
	}

	void RenderContext::DrawEllipseSmooth(FRect rect, Color color)
	{
		if(rect.w <= 0.0f || rect.h <= 0.0f)
		{
			return;
		}

		const float a = rect.w * 0.5f;
		const float b = rect.h * 0.5f;
		const float cx = rect.x + a;
		const float cy = rect.y + b;

		const float a2 = a * a;
		const float b2 = b * b;
		const float inv_a2 = 1.0f / a2;
		const float inv_b2 = 1.0f / b2;
		const uint8_t red = color.Red;
		const uint8_t green = color.Green;
		const uint8_t blue = color.Blue;
		const float alpha = (float)color.Alpha;

		// Precompute constant factors for midpoint algorithm
		const float sigma_y_step1 = 4.0f * a2;
		const float sigma_x_step1 = 4.0f * b2;
		const float sigma_x_step2 = 4.0f * b2;
		const float sigma_y_step2 = 4.0f * a2;

		// First octant (0 to 45 degrees) - restructured for ILP
		float x = 0.0f;
		float y = b;
		float sigma = 2.0f * b2 + a2 * (1.0f - 2.0f * b);

		while(b2 * x <= a2 * y)
		{
			// Geometry
			float t = 1.0f - (x * x) * inv_a2;
			t = t < 0.0f ? 0.0f : t;  // Branchless clamp
			const float exact_y = b * sqrtf(t);
			const float cx_plus_x = cx + x;
			const float cx_minus_x = cx - x;
			const float screen_y_top = cy - exact_y;
			const float screen_y_bottom = cy + exact_y;

			// Alpha values
			const int y_top_floor = (int)floorf(screen_y_top);
			const int y_bottom_floor = (int)floorf(screen_y_bottom);
			const float f_top = screen_y_top - (float)y_top_floor;
			const float f_bottom = screen_y_bottom - (float)y_bottom_floor;
			const uint8_t alpha1_top = (uint8_t)(alpha * (1.0f - f_top));
			const uint8_t alpha2_top = (uint8_t)(alpha * f_top);
			const uint8_t alpha1_bottom = (uint8_t)(alpha * (1.0f - f_bottom));
			const uint8_t alpha2_bottom = (uint8_t)(alpha * f_bottom);

			// Batch draw operations with minimal state changes
			FPoint points[4];
			points[0] = { cx_plus_x, (float)y_top_floor };
			points[1] = { cx_minus_x, (float)y_top_floor };
			points[2] = { cx_plus_x, (float)y_bottom_floor };
			points[3] = { cx_minus_x, (float)y_bottom_floor };

			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha1_top);
			SDL_RenderPoints(_renderer, points, 2);
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha1_bottom);
			SDL_RenderPoints(_renderer, &points[2], 2);

			points[0].y = (float)(y_top_floor + 1);
			points[1].y = (float)(y_top_floor + 1);
			points[2].y = (float)(y_bottom_floor + 1);
			points[3].y = (float)(y_bottom_floor + 1);

			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha2_top);
			SDL_RenderPoints(_renderer, points, 2);
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha2_bottom);
			SDL_RenderPoints(_renderer, &points[2], 2);

			// Midpoint update with independent components
			const float update_y = (sigma >= 0.0f) ? 1.0f : 0.0f;
			sigma += sigma_x_step1 * (x + 1.0f) + update_y * (sigma_y_step1 * (1.0f - y));
			y -= update_y;
			x += 1.0f;
		}

		// Second octant (45 to 90 degrees) - same ILP principles
		x = a;
		y = 0.0f;
		sigma = 2.0f * a2 + b2 * (1.0f - 2.0f * a);

		while(a2 * y <= b2 * x)
		{
			// Geometry
			float t = 1.0f - (y * y) * inv_b2;
			t = t < 0.0f ? 0.0f : t;  // Branchless clamp
			const float exact_x = a * sqrtf(t);
			const float cy_plus_y = cy + y;
			const float cy_minus_y = cy - y;
			const float screen_x_right = cx + exact_x;
			const float screen_x_left = cx - exact_x;

			// Alpha values
			const int x_right_floor = (int)floorf(screen_x_right);
			const int x_left_floor = (int)floorf(screen_x_left);
			const float f_right = screen_x_right - (float)x_right_floor;
			const float f_left = screen_x_left - (float)x_left_floor;
			const uint8_t alpha1_right = (uint8_t)(alpha * (1.0f - f_right));
			const uint8_t alpha2_right = (uint8_t)(alpha * f_right);
			const uint8_t alpha1_left = (uint8_t)(alpha * (1.0f - f_left));
			const uint8_t alpha2_left = (uint8_t)(alpha * f_left);

			// Batch draw operations
			SDL_FPoint points[4];
			points[0] = { (float)x_right_floor, cy_minus_y };
			points[1] = { (float)x_right_floor, cy_plus_y };
			points[2] = { (float)x_left_floor, cy_minus_y };
			points[3] = { (float)x_left_floor, cy_plus_y };

			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha1_right);
			SDL_RenderPoints(_renderer, points, 2);
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha1_left);
			SDL_RenderPoints(_renderer, &points[2], 2);

			points[0].x = (float)(x_right_floor + 1);
			points[1].x = (float)(x_right_floor + 1);
			points[2].x = (float)(x_left_floor + 1);
			points[3].x = (float)(x_left_floor + 1);

			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha2_right);
			SDL_RenderPoints(_renderer, points, 2);
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha2_left);
			SDL_RenderPoints(_renderer, &points[2], 2);

			// Midpoint update with independent components
			const float update_x = (sigma >= 0.0f) ? 1.0f : 0.0f;
			sigma += sigma_y_step2 * (y + 1.0f) + update_x * (sigma_x_step2 * (1.0f - x));
			x -= update_x;
			y += 1.0f;
		}
	}

	static std::vector<Vertex> ComputeFillEllipseVertices(FRect rect, Color color, bool hasTexture)
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
		auto vertices = ComputeFillEllipseVertices(rect, color, false);
		auto order = Math::TriangulateEllipse(EllipseVerticesNumber - 1);

		DrawGeometry(vertices, order);
	}

	void RenderContext::DrawEllipseFill(FRect rect, const Texture& texture)
	{
		auto color = texture.GetColor();
		auto vertices = ComputeFillEllipseVertices(rect, color, true);
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

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, 
											   const FPoint* center, SDL_FlipMode flip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr, nullptr,
								 angle, center, flip);
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, 
											   const FPoint* center, SDL_FlipMode flip, 
											   FRect target)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), nullptr,
								 &target, angle, center, flip);
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, double angle, 
											   const FPoint* center, SDL_FlipMode flip, 
											   FRect target, FRect clip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), &clip,
								 &target, angle, center, flip);
	}

	void RenderContext::DrawText(FPoint position, std::string_view text, float size, 
								 Color color, const FontFamily& fontFamily)
	{
		FontImpl font(fontFamily, size);
		Texture texture(_renderer, FontQuality::Blended, font, text, color);
		FRect rect(position.x, position.y, texture.GetWidth(), texture.GetHeight());
		DrawTexture(texture, rect);
	}
}
