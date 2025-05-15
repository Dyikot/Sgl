#pragma once

#include "../Graphic/BasicTypes.h"

namespace Sgl::Math
{
	std::vector<int> TriangulateConvexShape(std::span<FPoint> points);
	std::vector<int> TriangulateConvexShape(std::span<FPoint> points, FPoint center);
}