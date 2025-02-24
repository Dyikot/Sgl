#include "Shapes.h"
#include <algorithm>
#include <ranges>
#include <list>
#include <numeric>

namespace Sgl
{
	ShapeBase::ShapeBase(SDL_FPoint rotationCenter):
		RotationCenter(RotationCenter)
	{}

	SDL_FPoint ShapeBase::RotatePoint(SDL_FPoint point, SDL_FPoint center, size_t degree)
	{
		return SDL_FPoint
		{
			.x = (point.x - center.x) * Cos[degree] - (point.y - center.y) * Sin[degree] + center.x,
			.y = (point.x - center.x) * Sin[degree] + (point.y - center.y) * Cos[degree] + center.y
		};
	}

	Line::Line(float x1, float y1, float x2, float y2, Sgl::Color color):
		ShapeBase(SDL_FPoint(x1, x2)),
		Start{ x1, y1 },
		End{ x2, y2 },
		Color(color)
	{}

	Line::Line(SDL_FPoint start, SDL_FPoint end, Sgl::Color color):
		ShapeBase(start),
		Start(start),
		End(end),
		Color(color)
	{}

	void Line::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLine(Start, End, Color);
	}

	void Line::Translate(float dx, float dy)
	{
		Start.x += dx;
		Start.y += dy;
		End.x += dx;
		End.y += dy;
	}

	void Line::Rotate(size_t degree)
	{
		End = RotatePoint(End, RotationCenter, degree);
	}

	Polyline::Polyline(std::span<SDL_FPoint> points, Sgl::Color color):
		ShapeBase(points.front()),
		Points(points.begin(), points.end()),
		Color(color)
	{}

	Polyline::Polyline(std::vector<SDL_FPoint> && points, Sgl::Color color):
		ShapeBase(points.front()),
		Points(std::move(points)),
		Color(color)
	{}

	void Polyline::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLines(Points, Color);
	}

	void Polyline::Translate(float dx, float dy)
	{
		for(auto& point : Points)
		{
			point.x += dx;
			point.y += dy;
		}
	}

	void Polyline::Rotate(size_t degree)
	{
		for(auto& point : Points)
		{
			point = RotatePoint(point, RotationCenter, degree);
		}
	}

	Polygon::Polygon(std::span<SDL_FPoint> points, Sgl::Color color):
		ShapeBase(points.front()),
		Points(points.begin(), points.end()),
		Color(color)
	{}

	Polygon::Polygon(std::vector<SDL_FPoint> && points, Sgl::Color color):
		ShapeBase(points.front()),
		Points(std::move(points)),
		Color(color)
	{}

	void Polygon::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLines(Points, Color);
		renderContext.DrawLine(Points.back(), Points.front(), Color);
	}

	void Polygon::Translate(float dx, float dy)
	{
		for(auto& point : Points)
		{
			point.x += dx;
			point.y += dy;
		}
	}

	void Polygon::Rotate(size_t degree)
	{
		for(auto& point : Points)
		{
			point = RotatePoint(point, RotationCenter, degree);
		}
	}

	FillPolygon::FillPolygon(std::span<SDL_FPoint> points, Sgl::Color color):
		ShapeBase(points.front()),
		Order(Math::TriangulateConvexShape(points)),
		Color(color)
	{
		Vertices.reserve(points.size());
		Math::PointsToVertexRange(points, Vertices, color);
	}

	FillPolygon::FillPolygon(std::array<SDL_FPoint, 100>&& points, Sgl::Color color):
		ShapeBase(points.front()),
		Order(Math::TriangulateConvexShape(points)),
		Color(color)
	{
		Vertices.reserve(points.size());
		Math::PointsToVertexRange(points, Vertices, color);
	}

	void FillPolygon::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawShape(Vertices, Order);
	}

	void FillPolygon::Translate(float dx, float dy)
	{
		for(auto& vertex : Vertices)
		{
			vertex.position.x += dx;
			vertex.position.y += dy;
		}
	}

	void FillPolygon::Rotate(size_t degree)
	{
		for(auto& vertex : Vertices)
		{
			vertex.position = RotatePoint(vertex.position, RotationCenter, degree);
		}
	}

	Rectangle::Rectangle(float x, float y, float width, float height, Sgl::Color color):
		ShapeBase(SDL_FPoint(x + width / 2, y + height / 2)),
		Rect{ x, y, width, height },
		Color(color)
	{}

	Rectangle::Rectangle(SDL_FPoint position, float width, float height, Sgl::Color color):
		ShapeBase(SDL_FPoint(position.x + width / 2, position.y + height / 2)),
		Rect{ position.x, position.y, width, height },
		Color(color)
	{}

	Rectangle::Rectangle(SDL_FRect rect, Sgl::Color color):
		ShapeBase(SDL_FPoint(rect.x + rect.w / 2, rect.y + rect.h / 2)),
		Rect(rect),
		Color(color)
	{}

	void Rectangle::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawRectangle(Rect, Color);
	}

	void Rectangle::Translate(float dx, float dy)
	{
		Rect.x += dx;
		Rect.y += dy;
	}

	void Rectangle::Rotate(size_t degree)
	{
		
	}

	FillRectangle::FillRectangle(float x, float y, float width, float height, Sgl::Color color):
		ShapeBase(SDL_FPoint(x + width / 2, y + height / 2)),
		Rect{ x, y, width, height }, Color(color)
	{}

	FillRectangle::FillRectangle(SDL_FPoint position, float width, float height, Sgl::Color color):
		ShapeBase(SDL_FPoint(position.x + width / 2, position.y + height / 2)),
		Rect{ position.x, position.y, width, height }, Color(color)
	{}

	FillRectangle::FillRectangle(const SDL_FRect & rect, Sgl::Color color):
		ShapeBase(SDL_FPoint(rect.x + rect.w / 2, rect.y + rect.h / 2)),
		Rect(rect), Color(color)
	{}

	void FillRectangle::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawFillRectangle(Rect, Color);
	}

	void FillRectangle::Translate(float dx, float dy)
	{
		Rect.x += dx;
		Rect.y += dy;
	}

	void FillRectangle::Rotate(size_t degree)
	{
		
	}

	TexturedRectangle::TexturedRectangle(float x, float y,
										 float width, float height,
										 const Sgl::Texture& texture, Sgl::Color color):
		ShapeBase(SDL_FPoint(x + width / 2, y + height / 2)),
		Rect{ x, y, width, height },
		Texture(texture),
		Color(color)
	{}

	TexturedRectangle::TexturedRectangle(SDL_FPoint position,
										 float width, float height, 
										 const Sgl::Texture & texture, Sgl::Color color):
		ShapeBase(SDL_FPoint(position.x + width / 2, position.y + height / 2)),
		Rect{ position.x, position.y, width, height }, Texture(texture),
		Color(color)
	{}

	TexturedRectangle::TexturedRectangle(const SDL_FRect & rect, const Sgl::Texture & texture,
										 Sgl::Color color):
		ShapeBase(SDL_FPoint(rect.x + rect.w / 2, rect.y + rect.h / 2)),
		Rect(rect), Texture(texture), Color(color)
	{}

	void TexturedRectangle::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawTexture(Texture, Rect, Color);
	}
	
	Ellipse::Ellipse(SDL_FPoint position, int width, int height, Sgl::Color color):
		ShapeBase(position),
		Points(Math::ComputeEllipsePoints(position, width, height)),
		Color(color)
	{}

	void Ellipse::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawLines(Points, Color);
	}

	void Ellipse::Translate(float dx, float dy)
	{
		for(auto& point : Points)
		{
			point.x += dx;
			point.y += dy;
		}
	}

	void Ellipse::Rotate(size_t degree)
	{
		for(auto& point : Points)
		{
			point = RotatePoint(point, RotationCenter, degree);
		}
	}

	FillEllipse::FillEllipse(SDL_FPoint position, int width, int height, Sgl::Color color):
		ShapeBase(position),
		Color(color)
	{
		auto points = Math::ComputeEllipsePoints(position, width, height);
		Math::PointsToVertexRange(points, Vertices, color);
		Vertices.back() = SDL_Vertex(position, static_cast<SDL_Color>(color), SDL_FPoint());
		Order = Math::TriangulateConvexShape(points, position);		
	}

	void FillEllipse::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawShape(Vertices, Order);
	}

	void FillEllipse::Translate(float dx, float dy)
	{
		for(auto& vertex : Vertices)
		{
			vertex.position.x += dx;
			vertex.position.y += dy;
		}
	}

	void FillEllipse::Rotate(size_t degree)
	{
		for(auto& vertex : Vertices)
		{
			vertex.position = RotatePoint(vertex.position, RotationCenter, degree);
		}
	}

	TexturedEllipse::TexturedEllipse(SDL_FPoint position,
									 int width, int height,
									 const Sgl::Texture& texture, Sgl::Color color):
		ShapeBase(position),
		Texture(texture), Color(color)
	{
		auto points = Math::ComputeEllipsePoints(position, width, height);
		Math::PointsToVertexRange(points, Vertices, color);
		Vertices.back() = SDL_Vertex(position, static_cast<SDL_Color>(color), SDL_FPoint());
		Order = Math::TriangulateConvexShape(points, position);
	}

	void TexturedEllipse::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawShape(Vertices, Order, Texture, Color);
	}

	void TexturedEllipse::Translate(float dx, float dy)
	{
		for(auto& vertex : Vertices)
		{
			vertex.position.x += dx;
			vertex.position.y += dy;
		}
	}

	void TexturedEllipse::Rotate(size_t degree)
	{
		for(auto& vertex : Vertices)
		{
			vertex.position = RotatePoint(vertex.position, RotationCenter, degree);
		}
	}

	
}
