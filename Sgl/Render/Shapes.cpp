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
		Filler(
			[&](const Color& color)
			{
				renderContext.DrawFillRectangle(Rect, color);
			},
			[&](const Texture& texture)
			{
				renderContext.DrawTexture(texture, Rect);
			}
		).FillWith(Fill);
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

	void FillEllipse::OnRender(RenderContext& renderContext)
	{
		Filler(
			[&](const Color& color)
			{
				renderContext.DrawShape(Vertices, Order);
			},
			[&](const Texture& texture)
			{
				renderContext.DrawShape(Vertices, Order, texture);
			}
		).FillWith(Fill);
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
}
