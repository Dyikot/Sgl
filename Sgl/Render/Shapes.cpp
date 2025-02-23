#include "Shapes.h"
#include <algorithm>
#include <ranges>
#include <list>
#include <numeric>

namespace Sgl
{
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

	void TexturedRectangle::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawTexture(Texture, Rect);
	}
	
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
									 const Sgl::Texture& texture):
		ShapeBase(position),
		Texture(texture)
	{
		auto points = Math::ComputeEllipsePoints(position, width, height);
		Math::PointsToVertexRange(points, Vertices, texture.Color);
		Vertices.back() = SDL_Vertex(position, static_cast<SDL_Color>(texture.Color), SDL_FPoint());
		Order = Math::TriangulateConvexShape(points, position);
	}

	void TexturedEllipse::OnRender(RenderContext& renderContext)
	{
		renderContext.DrawShape(Vertices, Order, Texture);
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
