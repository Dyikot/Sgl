#pragma once

#include "../Graphic/Primitives.h"
#include <span>

namespace Sgl::Math
{
	std::vector<int> TriangulateConvexShape(std::span<FPoint> points);
	std::vector<int> TriangulateEllipse(size_t pointsNumber);
}