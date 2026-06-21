#include "RenderContext.h"

#include <cassert>
#include <numbers>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "../Base/Math.h"
#include "../Base/Media/Font.h"
#include "../Base/Logging.h"

namespace Sgl
{
	static constexpr float PI = std::numbers::pi_v<float>;
	
	static constexpr size_t EllipseNormalizedSize = 61;
	static constexpr FPoint EllipseNormalized[] =
	{
		{ 1.000000f,  0.000000f}, { 0.994522f,  0.104528f}, { 0.978148f,  0.207912f},
		{ 0.951057f,  0.309017f}, { 0.913545f,  0.406737f}, { 0.866025f,  0.500000f},
		{ 0.809017f,  0.587785f}, { 0.743145f,  0.669131f}, { 0.669131f,  0.743145f},
		{ 0.587785f,  0.809017f}, { 0.500000f,  0.866025f}, { 0.406737f,  0.913545f},
		{ 0.309017f,  0.951057f}, { 0.207912f,  0.978148f}, { 0.104528f,  0.994522f},
		{ 0.000000f,  1.000000f}, {-0.104528f,  0.994522f}, {-0.207912f,  0.978148f},
		{-0.309017f,  0.951057f}, {-0.406737f,  0.913545f}, {-0.500000f,  0.866025f},
		{-0.587785f,  0.809017f}, {-0.669131f,  0.743145f}, {-0.743145f,  0.669131f},
		{-0.809017f,  0.587785f}, {-0.866025f,  0.500000f}, {-0.913545f,  0.406737f},
		{-0.951057f,  0.309017f}, {-0.978148f,  0.207912f}, {-0.994522f,  0.104528f},
		{-1.000000f,  0.000000f}, {-0.994522f, -0.104528f}, {-0.978148f, -0.207912f},
		{-0.951057f, -0.309017f}, {-0.913545f, -0.406737f}, {-0.866025f, -0.500000f},
		{-0.809017f, -0.587785f}, {-0.743145f, -0.669131f}, {-0.669131f, -0.743145f},
		{-0.587785f, -0.809017f}, {-0.500000f, -0.866025f}, {-0.406737f, -0.913545f},
		{-0.309017f, -0.951057f}, {-0.207912f, -0.978148f}, {-0.104528f, -0.994522f},
		{ 0.000000f, -1.000000f}, { 0.104528f, -0.994522f}, { 0.207912f, -0.978148f},
		{ 0.309017f, -0.951057f}, { 0.406737f, -0.913545f}, { 0.500000f, -0.866025f},
		{ 0.587785f, -0.809017f}, { 0.669131f, -0.743145f}, { 0.743145f, -0.669131f},
		{ 0.809017f, -0.587785f}, { 0.866025f, -0.500000f}, { 0.913545f, -0.406737f},
		{ 0.951057f, -0.309017f}, { 0.978148f, -0.207912f}, { 0.994522f, -0.104528f},
		{ 1.000000f,  0.000000f}
	};

	class Arc
	{
	public:
		static constexpr size_t Segments = 8;
		static constexpr size_t VerticesCount = Segments + 1;
		
		static constexpr FPoint Coordinates[] =
		{
			{ 1.000000f,  0.000000f}, { 0.980785f,  0.195090f}, { 0.923880f,  0.382683f},
			{ 0.831470f,  0.555570f}, { 0.707107f,  0.707107f}, { 0.555570f,  0.831470f},
			{ 0.382683f,  0.923880f}, { 0.195090f,  0.980785f}, { 0.000000f,  1.000000f},
		};

		static std::array<FPoint, VerticesCount> CalculatePoints(float radius)
		{
			std::array<FPoint, VerticesCount> points;
			for(size_t i = 0; i < VerticesCount; i++)
			{
				points[i].x = radius * Coordinates[i].x;
				points[i].y = radius * Coordinates[i].y;
			}

			return points;
		}
	};		

	class VertexFactory
	{
	public:
		explicit VertexFactory(Color color):
			_color(color),
			_uv()
		{}

		VertexFactory(Color color, FPoint uv):
			_color(color),
			_uv(uv)
		{}	

		inline Vertex operator()(float x, float y) const
		{
			return Vertex(FPoint(x, y), _color, _uv);
		}

		inline Vertex operator()(float x, float y, FPoint uv) const
		{
			return Vertex(FPoint(x, y), _color, uv);
		}
	private:
		SDL_FColor _color;
		FPoint _uv;
	};

	static void CalculateRoundedRectVertices(std::span<Vertex> vertices, FRect rect, 
											 float radius, Color color)
	{
		const std::array arcPoints = Arc::CalculatePoints(radius);
		size_t i = 0;

		VertexFactory createVertex(color);

		// Center
		vertices[i++] = createVertex(rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f);

		// Top-Left
		{
			float x = rect.x + radius;
			float y = rect.y + radius;

			for(auto& p : arcPoints)
			{
				vertices[i++] = createVertex(x - p.x, y - p.y);
			}
		}

		// Top-Right
		{
			float x = rect.x + rect.w - radius;
			float y = rect.y + radius;

			for(int j = arcPoints.size() - 1; j >= 0; j--)
			{
				auto& p = arcPoints[j];
				vertices[i++] = createVertex(x + p.x, y - p.y);
			}
		}

		// Bottom-Right
		{
			float x = rect.x + rect.w - radius;
			float y = rect.y + rect.h - radius;

			for(auto& p : arcPoints)
			{
				vertices[i++] = createVertex(x + p.x, y + p.y);
			}
		}

		// Bottom-Left
		{
			float x = rect.x + radius;
			float y = rect.y + rect.h - radius;

			for(int j = arcPoints.size() - 1; j >= 0; j--)
			{
				auto& p = arcPoints[j];
				vertices[i++] = createVertex(x - p.x, y + p.y);
			}
		}
	}

	static void CalculateRoundedRectVertices(std::span<Vertex> vertices, FRect rect,
											 float radius, const Texture& texture)
	{	
		const std::array arcPoints = Arc::CalculatePoints(radius);
		size_t i = 0;

		VertexFactory createVertex(texture.GetColor());

		// Center
		vertices[i++] = createVertex(rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f, FPoint(0.5, 0.5));

		// Top-Left
		{
			float x = rect.x + radius;
			float y = rect.y + radius;

			for(auto& p : arcPoints)
			{
				const float px = x - p.x;
				const float py = y - p.y;
				const float u = (px - rect.x) / rect.w;
				const float v = (py - rect.y) / rect.h;

				vertices[i++] = createVertex(px, py, FPoint(u, v));
			}
		}

		// Top-Right
		{
			float x = rect.x + rect.w - radius;
			float y = rect.y + radius;

			for(int j = arcPoints.size() - 1; j >= 0; j--)
			{
				auto& p = arcPoints[j];
				const float px = x + p.x;
				const float py = y - p.y;
				const float u = (px - rect.x) / rect.w;
				const float v = (py - rect.y) / rect.h;

				vertices[i++] = createVertex(px, py, FPoint(u, v));
			}
		}

		// Bottom-Right
		{
			float x = rect.x + rect.w - radius;
			float y = rect.y + rect.h - radius;

			for(auto& p : arcPoints)
			{
				const float px = x + p.x;
				const float py = y + p.y;
				const float u = (px - rect.x) / rect.w;
				const float v = (py - rect.y) / rect.h;

				vertices[i++] = createVertex(px, py, FPoint(u, v));
			}
		}

		// Bottom-Left
		{
			float x = rect.x + radius;
			float y = rect.y + rect.h - radius;

			for(int j = arcPoints.size() - 1; j >= 0; j--)
			{
				auto& p = arcPoints[j];
				const float px = x - p.x;
				const float py = y + p.y;
				const float u = (px - rect.x) / rect.w;
				const float v = (py - rect.y) / rect.h;

				vertices[i++] = createVertex(px, py, FPoint(u, v));
			}
		}
	}

	static void CalculateRoundedRectPoints(std::span<FPoint> points, FRect rect, float radius)
	{
		const std::array arcPoints = Arc::CalculatePoints(radius);
		size_t i = 0;

		// Top-Left
		{
			float x = rect.x + radius;
			float y = rect.y + radius;

			for(auto& p : arcPoints)
			{
				points[i++] = { x - p.x, y - p.y };
			}
		}

		// Top-Right
		{
			float x = rect.x + rect.w - radius;
			float y = rect.y + radius;

			for(int j = arcPoints.size() - 1; j >= 0; j--)
			{
				auto& p = arcPoints[j];
				points[i++] = { x + p.x, y - p.y };
			}
		}

		// Bottom-Right
		{
			float x = rect.x + rect.w - radius;
			float y = rect.y + rect.h - radius;

			for(auto& p : arcPoints)
			{
				points[i++] = { x + p.x, y + p.y };
			}
		}

		// Bottom-Left
		{
			float x = rect.x + radius;
			float y = rect.y + rect.h - radius;

			for(int j = arcPoints.size() - 1; j >= 0; j--)
			{
				auto& p = arcPoints[j];
				points[i++] = { x - p.x, y + p.y };
			}
		}
	}

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

	void RenderContext::DrawRectangle(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderRect(_renderer, &rect);
	}

	void RenderContext::DrawRectangle(FRect rect, float thickness, Color color)
	{
		if(thickness < 1.0f)
		{
			return;
		}

		if(thickness == 1.0f)
		{
			DrawRectangle(rect, color);
			return;
		}

		VertexFactory createVertex(color);
		auto [x, y, w, h] = rect;

		const std::array vertices =
		{
			// Outer rect
			createVertex(x, y),
			createVertex(x + w, y),
			createVertex(x + w, y + h),
			createVertex(x, y + h),

			// Inner rect
			createVertex(x + thickness, y + thickness),
			createVertex(x + w - thickness, y + thickness),
			createVertex(x + w - thickness, y + h - thickness),
			createVertex(x + thickness, y + h - thickness),
		};

		const std::array indices =
		{
			0, 1, 4,  4, 5, 1,	// Top
			1, 2, 5,  5, 6, 2,	// Left
			2, 3, 6,  6, 7, 3,	// Bottom
			3, 0, 7,  7, 4, 0	// Right
		};

		DrawGeometry(vertices, nullptr, indices);
	}

	void RenderContext::DrawRoundedRectangle(FRect rect, float cornersRadius, Color color)
	{
		if(rect.w <= 0.0f || rect.h <= 0.0f)
		{
			return;
		}

		cornersRadius = std::clamp(cornersRadius, 0.0f, std::min(rect.w, rect.h) / 2.0f);

		const size_t totalPoints = 4 * Arc::VerticesCount + 1;
		std::array<FPoint, totalPoints> points;
		CalculateRoundedRectPoints(points, rect, cornersRadius);

		points.back() = points.front();

		DrawLines(points, color);
	}

	void RenderContext::DrawRoundedRectangle(FRect rect, float cornersRadius, 
											 float thickness, Color color)
	{
		if(rect.w <= 0.0f || rect.h <= 0.0f || thickness <= 0.0f)
		{
			return;
		}

		if(thickness == 1.0f)
		{
			DrawRoundedRectangle(rect, cornersRadius, color);
			return;
		}

		cornersRadius = std::clamp(cornersRadius, 0.0f, std::min(rect.w, rect.h) / 2.0f);

		// Validate thickness
		float maxThickness = std::min(rect.w, rect.h) / 2.0f;
		if(thickness > maxThickness)
		{
			thickness = maxThickness;
		}

		// Inner rect
		float innerRadius = std::max(0.0f, cornersRadius - thickness);
		FRect innerRect = 
		{
			rect.x + thickness,
			rect.y + thickness,
			rect.w - 2.0f * thickness,
			rect.h - 2.0f * thickness
		};

		const size_t cornerPoints = Arc::VerticesCount;
		const size_t perimeterPoints = 4 * cornerPoints;
		const size_t totalVertices = 2 * perimeterPoints;
		const size_t totalIndices = 6 * perimeterPoints;

		// Calculate perimeter pints for outer and inner rects
		std::array<FPoint, perimeterPoints> outerPoints;
		std::array<FPoint, perimeterPoints> innerPoints;
		CalculateRoundedRectPoints(outerPoints, rect, cornersRadius);
		CalculateRoundedRectPoints(innerPoints, innerRect, innerRadius);

		static std::array<Vertex, totalVertices> vertices;
		static std::array<int, totalIndices> indices;

		VertexFactory createVertex(color);

		// Assign vertices
		for(size_t i = 0; i < perimeterPoints; ++i)
		{
			auto& outer = outerPoints[i];
			vertices[i] = createVertex(outer.x, outer.y);

			auto& inner = innerPoints[i];
			vertices[i + perimeterPoints] = createVertex(inner.x, inner.y);
		}

		// Generate indices
		size_t i = 0;
		for(int j = 0; j < perimeterPoints - 1; ++j)
		{
			int next = j + 1;

			int outer = j;
			int outerNext = next;
			int inner = j + perimeterPoints;
			int innerNext = next + perimeterPoints;

			// Triangle 1
			indices[i++] = outer;
			indices[i++] = outerNext;
			indices[i++] = inner;

			// Triangle 2
			indices[i++] = outerNext;
			indices[i++] = innerNext;
			indices[i++] = inner;
		}

		// Connect back to the first point to close the loop
		{
			size_t j = perimeterPoints - 1;
			size_t next = 0;

			int outer = j;
			int outerNext = next;
			int inner = j + perimeterPoints;
			int innerNext = next + perimeterPoints;

			// Triangle 1
			indices[i++] = outer;
			indices[i++] = outerNext;
			indices[i++] = inner;

			// Triangle 2
			indices[i++] = outerNext;
			indices[i++] = innerNext;
			indices[i++] = inner;
		}

		DrawGeometry(vertices, nullptr, indices);
	}

	void RenderContext::DrawRectangles(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderRects(_renderer, rects.data(), rects.size());
	}

	void RenderContext::DrawRectangleFill(FRect rect, Color color)
	{
		SetColor(color);
		SDL_RenderFillRect(_renderer, &rect);
	}

	void RenderContext::DrawRectangleFill(FRect rect, float cornersRadius, Color color)
	{
		DrawRectangleFillCore(rect, cornersRadius, nullptr, color);
	}

	void RenderContext::DrawRectangleFill(FRect rect, float cornersRadius, const Texture& texture)
	{
		DrawRectangleFillCore(rect, cornersRadius, texture, texture.GetColor());
	}

	void RenderContext::DrawRectanglesFill(std::span<const FRect> rects, Color color)
	{
		SetColor(color);
		SDL_RenderFillRects(_renderer, rects.data(), rects.size());
	}

	void RenderContext::DrawEllipse(FRect rect, Color color)
	{
		std::array<FPoint, EllipseNormalizedSize> points;

		const float radiusX = 0.5f * rect.w;
		const float radiusY = 0.5f * rect.h;
		const float x = rect.x + radiusX;
		const float y = rect.y + radiusY;

		for(size_t i = 0; i < EllipseNormalizedSize; i++)
		{
			points[i].x = x + radiusX * EllipseNormalized[i].x;
			points[i].y = y + radiusY * EllipseNormalized[i].y;
		}

		DrawLines(points, color);
	}

	void RenderContext::DrawEllipseFill(FRect rect, Color color)
	{
		DrawEllipseCore(rect, {}, color);
	}

	void RenderContext::DrawEllipseFill(FRect rect, const Texture& texture)
	{
		DrawEllipseCore(rect, texture, texture.GetColor());
	}

	void RenderContext::DrawGeometry(std::span<const Vertex> vertices, 
									 const Texture& texture, 
									 std::span<const int> indices)
	{
		SDL_RenderGeometry(_renderer, texture.GetSDLTexture(),
						   vertices.data(), vertices.size(),
						   indices.data(), indices.size());
	}

	void RenderContext::DrawTexture(const Texture& texture, const FRect* target, const FRect* clip)
	{
		SDL_RenderTexture(_renderer, texture.GetSDLTexture(), clip, target);
	}
	
	void RenderContext::DrawTexture9Grid(const Texture & texture, 
										 float cornersLength, 
										 float scale, 
										 const FRect* target, 
										 const FRect* clip)
	{
		SDL_RenderTexture9Grid(_renderer, texture.GetSDLTexture(), clip,
			cornersLength, cornersLength, cornersLength, cornersLength,
			scale, target);
	}

	void RenderContext::DrawTextureTransformed(const Texture& texture, 
											   double angle, 
											   const FPoint* center, 
											   FlipMode flip, 
											   const FRect* target, 
											   const FRect* clip)
	{
		SDL_RenderTextureRotated(_renderer, texture.GetSDLTexture(), clip,
								 target, angle, center, SDL_FlipMode(flip));
	}

	void RenderContext::DrawText(FPoint position, std::string_view text, float size, 
								 Color color, FontFamily fontFamily)
	{
		TrueTypeFont font(fontFamily, size);
		Texture texture(_renderer, FontQuality::Blended, font, text, color);
		FRect rect(position.x, position.y, texture.GetWidth(), texture.GetHeight());
		DrawTexture(texture, &rect, nullptr);
	}

	void RenderContext::DrawEllipseCore(FRect rect, const Texture& texture, Color color)
	{
		const float radiusX = 0.5f * rect.w;
		const float radiusY = 0.5f * rect.h;
		const float x = rect.x + radiusX;
		const float y = rect.y + radiusY;
		VertexFactory createVertex(color);

		// Vertices
		const size_t totalVertices = EllipseNormalizedSize + 1;
		static std::array<Vertex, totalVertices> vertices;

		if(texture)
		{
			vertices[0] = createVertex(x, y, FPoint(0.5f, 0.5f));

			for(size_t i = 0; i < EllipseNormalizedSize; i++)
			{
				float nx = EllipseNormalized[i].x;
				float ny = EllipseNormalized[i].y;
				float u = (nx + 1.0f) * 0.5f;
				float v = (ny + 1.0f) * 0.5f;

				vertices[i + 1] = createVertex(x + radiusX * nx, y + radiusY * ny, FPoint(u, v));
			}
		}
		else
		{
			vertices[0] = createVertex(x, y);

			for(size_t i = 0; i < EllipseNormalizedSize; i++)
			{
				vertices[i + 1] = createVertex(x + radiusX * EllipseNormalized[i].x,
											   y + radiusY * EllipseNormalized[i].y);
			}
		}

		// Indices
		const int totalIndices = 3 * (totalVertices - 1);
		static const std::array indices = Math::FanTriangulate<totalIndices>(totalVertices - 1);

		DrawGeometry(vertices, texture, indices);
	}

	void RenderContext::DrawRectangleFillCore(FRect rect, float cornersRadius, 
											  const Texture& texture, Color color)
	{
		if(rect.w <= 0.0f || rect.h <= 0.0f)
		{
			return;
		}

		cornersRadius = std::clamp(cornersRadius, 0.0f, std::min(rect.w, rect.h) / 2.0f);

		static constexpr size_t totalVertices = 1 + 4 * Arc::VerticesCount;
		static constexpr size_t totalIndices = 3 * (totalVertices - 1);

		// Vertices
		static std::array<Vertex, totalVertices> vertices;
		// Indices
		static const std::array indices = Math::FanTriangulate<totalIndices>(totalVertices - 1);

		if(texture)
		{
			CalculateRoundedRectVertices(vertices, rect, cornersRadius, texture);
			DrawGeometry(vertices, texture, indices);
		}
		else
		{
			CalculateRoundedRectVertices(vertices, rect, cornersRadius, color);
			DrawGeometry(vertices, nullptr, indices);
		}
	}
}
