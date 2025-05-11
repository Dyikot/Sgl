#pragma once

#include "../Graphic/Types.h"

namespace Sgl::Math
{
	std::vector<int> TriangulateConvexShape(PointsView points);
	std::vector<int> TriangulateConvexShape(PointsView points, Point center);
}