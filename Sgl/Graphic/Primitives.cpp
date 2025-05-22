#include "Primitives.h"
#include "../Math/Math.h"
#include "../Math/Triangulation.h"
#include <cassert>

namespace Sgl
{
	constexpr size_t MaxAngle = 720;
	const auto SinRange = Math::SinRange<MaxAngle>();
	const auto CosRange = Math::CosRange<MaxAngle>();

	Ellipse::Ellipse(FPoint position, int width, int height)
	{		
		const size_t pointsNumber = DefinePointsNumber(width * height);
		const size_t angleStep = MaxAngle / pointsNumber;
		size_t angle = 0;

		_coordinates.resize(pointsNumber);

		for(size_t i = 0; i < _coordinates.size(); i++)
		{
			angle = i * angleStep;
			_coordinates[i].x = position.x + width * CosRange[angle];
			_coordinates[i].y = position.y + height * SinRange[angle];
		}
	}

	const std::vector<FPoint>& Ellipse::GetCoordinates() const
	{
		return _coordinates;
	}

	size_t Ellipse::DefinePointsNumber(int pixels)
	{
		return pixels <=    400 ? 90
			 : pixels <=  4'000 ? 180
			 : pixels <= 40'000 ? 360
			 : 720;
	}

	FillEllipse::FillEllipse(FPoint position, int width, int height, Color color)
	{
		const size_t pointsNumber = DefinePointsNumber(width * height);
		const size_t angleStep = MaxAngle / pointsNumber;
		size_t angle = 0;

		_vertices.resize(pointsNumber + 1);

		auto& center = _vertices[0];
		center.position.x = position.x + width * CosRange[0];
		center.position.y = position.y + height * SinRange[0];
		center.color = color;

		for(size_t i = 1; i < _vertices.size(); i++)
		{
			angle = (i - 1) * angleStep;
			_vertices[i].position.x = position.x + width * CosRange[angle];
			_vertices[i].position.y = position.y + height * SinRange[angle];
			_vertices[i].color = color;
		}

		_order = Math::TriangulateEllipse(pointsNumber);
	}

	FillEllipse::FillEllipse(FPoint position, int width, int height, Texture& texture, Color color)
	{
		
	}

	const std::vector<Vertex>& FillEllipse::GetVertices() const
	{
		return _vertices;
	}

	const std::vector<int>& FillEllipse::GetVerticesOrder() const
	{
		return _order;
	}

	size_t FillEllipse::DefinePointsNumber(int pixels)
	{
		return 90;
	}
}
