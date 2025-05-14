#pragma once

#include "../Graphic/Types.h"

namespace Sgl::Math
{
	std::vector<int> TriangulateConvexShape(FPointsView points);
	std::vector<int> TriangulateConvexShape(FPointsView points, FPoint center);
}